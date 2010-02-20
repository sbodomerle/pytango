#include "attribute.h"

#include "defs.h"
#include "pytgutils.h"

#include <string>
#include <boost/python.hpp>
#include <boost/python/return_value_policy.hpp>
#include <tango.h>

using namespace boost::python;

# ifdef WIN32
#   define PYTG_TIME_FROM_DOUBLE(dbl, tv) \
            if (true) { \
                tv.time = (time_t)floor(dbl); \
                tv.millitm = (unsigned short)((dbl - tv.time)*1.0e3); \
            } else (void)0
#   define PYTG_NEW_TIME_FROM_DOUBLE(dbl, tv) \
            struct _timeb tv; PYTG_TIME_FROM_DOUBLE(dbl, tv)
# else
#   define PYTG_TIME_FROM_DOUBLE(dbl, tv) double2timeval(tv, dbl);
#   define PYTG_NEW_TIME_FROM_DOUBLE(dbl, tv) \
            struct timeval tv; PYTG_TIME_FROM_DOUBLE(dbl, tv)
#endif


inline static void throw_wrong_python_data_type(const std::string &att_name,
                                         const char *method)
{
    TangoSys_OMemStream o;
    o << "Wrong Python type for attribute " << att_name << ends;
    Tango::Except::throw_exception(
            (const char *)"PyDs_WrongPythonDataTypeForAttribute",
            o.str(), method);
}

inline static void throw_wrong_python_data_type_in_array(const std::string &att_name,
                                                  long idx,
                                                  const char *method)
{
    TangoSys_OMemStream o;
    o << "Wrong Python type for attribute " << att_name
      << ".\nElement with index " << idx << " in sequence does not "
      << "have a correct type." << ends;

    Tango::Except::throw_exception(
            (const char *)"PyDs_WrongPythonDataTypeForAttribute",
            o.str(), method);
}

namespace PyAttribute
{
    /**
     * Tango Attribute set_value wrapper for scalar attributes
     *
     * @param att attribute reference
     * @param value new attribute value
     */
    template<long tangoTypeConst>
    inline void __set_value_scalar(Tango::Attribute &att,
                                   boost::python::object &value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;

        /*
           I hate doing this because tango inside is doing a new again when
           set_value_date_quality is invoked with the release flag to true
           the other option would be to use per thread tango data like it was 
           done in v3.0.4
           I prefer this one since it decouples TangoC++ from PyTango and creating
           a scalar is not so expensive after all
        */
        std::auto_ptr<TangoScalarType> cpp_val(new TangoScalarType);
        
        from_py<tangoTypeConst>::convert(value, *cpp_val);
        att.set_value(cpp_val.release(), 1, 0, true);
    }

    
    /**
     * Tango Attribute set_value wrapper for DevEncoded attributes
     *
     * @param att attribute reference
     * @param data_str new attribute data string
     * @param data new attribute data
     */
    inline void __set_value(Tango::Attribute &att,
                            boost::python::str &data_str,
                            boost::python::str &data)
    {
        extract<Tango::DevString> val_str(data_str.ptr());
        if (!val_str.check())
        {
            throw_wrong_python_data_type(att.get_name(), "set_value()");
        }
        extract<Tango::DevString> val(data.ptr());
        if (!val.check())
        {
            throw_wrong_python_data_type(att.get_name(), "set_value()");
        }

        Tango::DevString val_str_real = val_str;
        Tango::DevString val_real = val;
        att.set_value(&val_str_real, (Tango::DevUChar*)val_real, (long)len(data));
    }


    /**
     * Tango Attribute set_value_date_quality wrapper for scalar attributes
     *
     * @param att attribute reference
     * @param value new attribute value
     * @param t timestamp
     * @param quality attribute quality
     */
    template<long tangoTypeConst>
    inline void __set_value_date_quality_scalar(Tango::Attribute &att,
                                                boost::python::object &value,
                                                double t, Tango::AttrQuality quality)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;

        PYTG_NEW_TIME_FROM_DOUBLE(t, tv);
        
        /*
           I hate doing because tango inside is doing a new again when
           set_value_date_quality is invoked with the release flag to true
           the other option would be to use per thread tango data like it was 
           done in v3.0.4
           I prefer this one since it decouples TangoC++ from PyTango and creating
           a scalar is not so expensive after all
        */
        std::auto_ptr<TangoScalarType> cpp_val(new TangoScalarType);
        
        from_py<tangoTypeConst>::convert(value, *cpp_val);
        att.set_value_date_quality(cpp_val.release(), tv, quality, 1, 0, true);
    }


    /**
     * Tango Attribute set_value_date_quality wrapper for DevEncoded attributes
     *
     * @param att attribute reference
     * @param data_str new attribute data string
     * @param data new attribute data
     * @param t timestamp
     * @param quality attribute quality
     */
    inline void __set_value_date_quality(Tango::Attribute &att,
                                         boost::python::str &data_str,
                                         boost::python::str &data,
                                         double t, Tango::AttrQuality quality)
    {
        extract<Tango::DevString> val_str(data_str.ptr());
        if (!val_str.check())
        {
            throw_wrong_python_data_type(att.get_name(), "set_value()");
        }
        extract<Tango::DevUChar*> val(data.ptr());
        if (!val.check())
        {
            throw_wrong_python_data_type(att.get_name(), "set_value()");
        }

        PYTG_NEW_TIME_FROM_DOUBLE(t, tv);
        Tango::DevString val_str_real = val_str;
        Tango::DevUChar *val_real = val;
        att.set_value_date_quality(&val_str_real, val_real, (long)len(data), tv, quality);
    }


    template<long tangoTypeConst>
    inline void __delete_sequence(typename TANGO_const2type(tangoTypeConst)* data_buffer, long processedElements)
    {
        delete [] data_buffer;
    }

    template<>
    inline void __delete_sequence<Tango::DEV_STRING>(Tango::DevString* data_buffer, long processedElements)
    {
        for (long i =0; i < processedElements; ++i) {
            delete [] data_buffer[i];
        }
        delete [] data_buffer;
    }
    
    template<long tangoTypeConst>
    inline typename TANGO_const2type(tangoTypeConst)*
        __pyvalue_to_array_sequence(PyObject* py_val, long* pdim_x, long *pdim_y, const std::string &fname, bool isImage, long& res_dim_x, long& res_dim_y)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        long dim_x;
        long dim_y = 0;
        long len = PySequence_Size(py_val);;
        bool expectFlatSource;

        if (isImage) {
            if (pdim_y) {
                expectFlatSource = true;
                dim_x = *pdim_x;
                dim_y = *pdim_y;
                long len2 = dim_x*dim_y;
                if (len2 < len)
                    len = len2;
            } else {
                expectFlatSource = false;

                if (len > 0) {
                    PyObject* py_row0 = PySequence_ITEM(py_val, 0);
                    if (!py_row0 || !PySequence_Check(py_row0)) {
                        Py_XDECREF(py_row0);
                        Tango::Except::throw_exception(
                            "PyDs_WrongParameters",
                            "Expecting a sequence of sequences.",
                            fname + "()");
                    }

                    dim_y = len;
                    dim_x = PySequence_Size(py_row0);
                    Py_XDECREF(py_row0);
                } else {
                    dim_x = 0;
                }
            }
            len = dim_x*dim_y;
        } else {
            expectFlatSource = true;
            if (pdim_x) {
                if (*pdim_x > len)
                    Tango::Except::throw_exception(
                        "PyDs_WrongParameters",
                        "Specified dim_x is larger than the sequence size",
                        fname + "()");
                len = *pdim_x;
            }
            if (pdim_y && (*pdim_y!=0))
                Tango::Except::throw_exception(
                        "PyDs_WrongParameters",
                        "You should not specify dim_y for an spectrum attribute!",
                        fname + "()");
            dim_x = len;
        }

        res_dim_x = dim_x;
        res_dim_y = dim_y;

        if (!PySequence_Check(py_val))
            Tango::Except::throw_exception(
                "PyDs_WrongParameters",
                "Expecting a sequence!",
                fname + "()");

        /// @bug Why not TangoArrayType::allocbuf(len)? Because
        /// I will use it in set_value(tg_ptr,...,release=true).
        /// Tango API makes delete[] tg_ptr instead of freebuf(tg_ptr).
        /// This is usually the same, but for Tango::DevStringArray the
        /// behaviour seems different and causes troubles...
        TangoScalarType *tg_ptr = new TangoScalarType[len];

        // The boost extract could be used:
        // TangoScalarType val = boost::python::extract<TangoScalarType>(elt_ptr);
        // instead of the code below.
        // the problem is that extract is considerably slower than our
        // convert function which only has to deal with the specific tango
        // data types

        PyObject * py_el = 0;
        PyObject * py_row = 0;
        TangoScalarType tg_scalar;
        long idx = 0;
        try {
            if (expectFlatSource) {
                for (idx = 0; idx < len; ++idx)
                {
                    py_el = PySequence_ITEM(py_val, idx);
                    if (!py_el)
                            throw_error_already_set();

                    from_py<tangoTypeConst>::convert(py_el, tg_scalar);
                    tg_ptr[idx] = tg_scalar;
                    
                    Py_DECREF(py_el);
                    py_el = 0;
                }
            } else {
                for (long y=0; y < dim_y; ++y) {
                    py_row = PySequence_ITEM(py_val, y);
                    if (!py_row)
                            throw_error_already_set();
                    if (!PySequence_Check(py_row)) {
                        Tango::Except::throw_exception(
                            "PyDs_WrongParameters",
                            "Expecting a sequence of sequences!",
                            fname + "()");
                    }
                    for (long x=0; x < dim_x; ++x, ++idx) {
                        py_el = PySequence_ITEM(py_row, x);
                        if (!py_el)
                            throw_error_already_set();
                        
                        from_py<tangoTypeConst>::convert(py_el, tg_scalar);
                        tg_ptr[x + y*dim_x] = tg_scalar;
                        
                        Py_DECREF(py_el);
                        py_el = 0;
                    }
                    Py_DECREF(py_row);
                    py_row = 0;
                }
            }
        } catch(...) {
            Py_XDECREF(py_el);
            Py_XDECREF(py_row);
            __delete_sequence<tangoTypeConst>(tg_ptr, idx);
            throw;
        }
        return tg_ptr;
    }

    template<>
    inline TANGO_const2type(Tango::DEV_ENCODED)*
        __pyvalue_to_array_sequence<Tango::DEV_ENCODED>(PyObject*, long*, long*, const std::string & fname, bool isImage, long& res_dim_x, long& res_dim_y)
    {
        TangoSys_OMemStream o;
        o << "DevEncoded is only supported for SCALAR attributes." << ends;
        Tango::Except::throw_exception(
                "PyDs_WrongPythonDataTypeForAttribute",
                o.str(), fname + "()");
        return 0;
    }

}


#ifndef DISABLE_PYTANGO_NUMPY
#   include "attribute_numpy.hpp"
#endif


namespace PyAttribute
{

    template<long tangoTypeConst>
    void __set_value_date_quality_array(
            Tango::Attribute& att,
            boost::python::object &value,
            double time,
            Tango::AttrQuality* quality,
            long* x,
            long* y,
            const std::string &fname,
            bool isImage)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        if (!PySequence_Check(value.ptr()))
        {
            // avoid bug in tango 7.0 to 7.1.1: DevEncoded is not defined in CmdArgTypeName
            const char *arg_type = tangoTypeConst == Tango::DEV_ENCODED ? 
                "DevEncoded" : 
                Tango::CmdArgTypeName[tangoTypeConst];
                
            TangoSys_OMemStream o;
            o << "Wrong Python type for attribute " << att.get_name()
                << "of type " << arg_type << ". Expected a sequence." << ends;

            Tango::Except::throw_exception(
                    "PyDs_WrongPythonDataTypeForAttribute",
                    o.str(),
                    fname + "()");
        }

        TangoScalarType* data_buffer;

        long res_dim_x=0, res_dim_y=0;
#       ifdef DISABLE_PYTANGO_NUMPY
            data_buffer =__pyvalue_to_array_sequence<tangoTypeConst>(
                    value.ptr(), x, y, fname, isImage, res_dim_x, res_dim_y);
#       else
            data_buffer = __pyvalue_to_array_numpy<tangoTypeConst>(
                    value.ptr(), x, y, fname, isImage, res_dim_x, res_dim_y);
#       endif


        static const bool release = true;

        if (quality) {
            PYTG_NEW_TIME_FROM_DOUBLE(time, tv);
            att.set_value_date_quality(
                data_buffer, tv, *quality, res_dim_x, res_dim_y, release);
        } else {
            att.set_value(data_buffer, res_dim_x, res_dim_y, release);
        }
    }



    inline void __set_value(const std::string & fname, Tango::Attribute &att, boost::python::object &value, long* x, long *y, double t = 0.0, Tango::AttrQuality* quality = 0)
    {
        long type = att.get_data_type();
        Tango::AttrDataFormat format = att.get_data_format();

        const bool isScalar = (format == Tango::SCALAR);
        const bool isImage = (format == Tango::IMAGE);

        if (isScalar) {
            if ((x && ((*x) > 1)) || (y && (*y) > 0)) {
                TangoSys_OMemStream o;
                o << "Cannot call " << fname;
                if (y)
                    o << "(data, dim_x, dim_y) on scalar attribute ";
                else
                    o << "(data, dim_x) on scalar attribute ";

                if (quality)
                    o << att.get_name() << ". Use set_value_date_quality(data) instead" << ends;
                else
                    o << att.get_name() << ". Use set_value(data) instead" << ends;

                Tango::Except::throw_exception(
                        "PyDs_WrongPythonDataTypeForAttribute",
                        o.str(),
                        fname + "()");
            } else {
                if (quality)
                    TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(type, __set_value_date_quality_scalar, att, value, t, *quality);
                else
                    TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(type, __set_value_scalar, att, value);
            }
        } else {
            TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(type,
                __set_value_date_quality_array,
                    att, value, t, quality, x, y, fname, isImage);
        }
    }

    inline void __set_value(const std::string & fname, Tango::Attribute &att, boost::python::str &data_str, boost::python::str &data, double t = 0.0, Tango::AttrQuality* quality = 0)
    {
        if (quality)
            __set_value_date_quality(att, data_str, data, t, *quality);
        else
            __set_value(att, data_str, data);
    }

    inline void set_value(Tango::Attribute &att, boost::python::object &value)
    { __set_value("set_value", att, value, 0, 0); }

    inline void set_value(Tango::Attribute &att, boost::python::str &data_str, boost::python::str &data)
    { __set_value("set_value", att, data_str, data); }

    inline void set_value(Tango::Attribute &att, boost::python::object &value, long x)
    { __set_value("set_value", att, value, &x, 0); }

    inline void set_value(Tango::Attribute &att, boost::python::object &value, long x, long y)
    { __set_value("set_value", att, value, &x, &y); }

    inline void set_value_date_quality(Tango::Attribute &att,
                                       boost::python::object &value, double t,
                                       Tango::AttrQuality quality)
    { __set_value("set_value_date_quality", att, value, 0, 0, t, &quality); }

    inline void set_value_date_quality(Tango::Attribute &att,
                                       boost::python::str &data_str,
                                       boost::python::str &data,
                                       double t,
                                       Tango::AttrQuality quality)
    { __set_value("set_value_date_quality", att, data_str, data, t, &quality); }

    inline void set_value_date_quality(Tango::Attribute &att,
                                       boost::python::object &value,
                                       double t, Tango::AttrQuality quality,
                                       long x)
    { __set_value("set_value_date_quality", att, value, &x, 0, t, &quality); }

    inline void set_value_date_quality(Tango::Attribute &att,
                                       boost::python::object &value,
                                       double t, Tango::AttrQuality quality,
                                       long x, long y)
    { __set_value("set_value_date_quality", att, value, &x, &y, t, &quality); }
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_quality_overloads,
                                       Tango::Attribute::set_quality, 1, 2);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_change_event_overloads,
                                       Tango::Attribute::set_change_event, 1, 2);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_archive_event_overloads,
                                       Tango::Attribute::set_archive_event, 1, 2);

void export_attribute()
{
    enum_<Tango::Attribute::alarm_flags>("alarm_flags")
        .value("min_level", Tango::Attribute::min_level)
        .value("max_level", Tango::Attribute::max_level)
        .value("rds", Tango::Attribute::rds)
        .value("min_warn", Tango::Attribute::min_warn)
        .value("max_warn", Tango::Attribute::max_warn)
        .value("numFlags", Tango::Attribute::numFlags)
    ;

    class_<Tango::Attribute>("Attribute", no_init)
        .def("is_write_associated", &Tango::Attribute::is_writ_associated)
        .def("is_min_alarm", &Tango::Attribute::is_min_alarm)
        .def("is_max_alarm", &Tango::Attribute::is_max_alarm)
        .def("is_min_warning", &Tango::Attribute::is_min_warning)
        .def("is_max_warning", &Tango::Attribute::is_max_warning)
        .def("is_rds_alarm", &Tango::Attribute::is_rds_alarm)
        //TODO .def("is_alarmed", &Tango::Attribute::is_alarmed)
        .def("is_polled", &Tango::Attribute::is_polled)
        .def("check_alarm", &Tango::Attribute::check_alarm)
        .def("get_writable", &Tango::Attribute::get_writable)
        .def("get_name", &Tango::Attribute::get_name,
            return_value_policy<copy_non_const_reference>())
        .def("get_data_type", &Tango::Attribute::get_data_type)
        .def("get_data_format", &Tango::Attribute::get_data_format)
        .def("get_assoc_name", &Tango::Attribute::get_assoc_name, 
            return_value_policy<copy_non_const_reference>())
        .def("get_assoc_ind", &Tango::Attribute::get_assoc_ind)
        .def("set_assoc_ind", &Tango::Attribute::set_assoc_ind)
        .def("get_date", &Tango::Attribute::get_date, 
            return_internal_reference<>())
        .def("set_date",
             (void (Tango::Attribute::*) (Tango::TimeVal &))
             &Tango::Attribute::set_date)
        .def("get_label", &Tango::Attribute::get_label, 
            return_value_policy<copy_non_const_reference>())
        .def("get_quality", &Tango::Attribute::get_quality, 
            return_value_policy<copy_non_const_reference>())
        .def("set_quality", &Tango::Attribute::set_quality, 
            set_quality_overloads())
        .def("get_data_size", &Tango::Attribute::get_data_size)
        .def("get_x", &Tango::Attribute::get_x)
        .def("get_max_dim_x", &Tango::Attribute::get_max_dim_x)
        .def("get_y", &Tango::Attribute::get_y)
        .def("get_max_dim_y", &Tango::Attribute::get_max_dim_y)
        .def("get_polling_period", &Tango::Attribute::get_polling_period)
        .def("set_attr_serial_model", &Tango::Attribute::set_attr_serial_model)
        .def("get_attr_serial_model", &Tango::Attribute::get_attr_serial_model)
        
        .def("set_value",
            (void (*) (Tango::Attribute &, boost::python::object &))
            &PyAttribute::set_value)
        .def("set_value",
            (void (*) (Tango::Attribute &, boost::python::str &, boost::python::str &))
            &PyAttribute::set_value)
        .def("set_value",
            (void (*) (Tango::Attribute &, boost::python::object &, long))
            &PyAttribute::set_value)
        .def("set_value",
            (void (*) (Tango::Attribute &, boost::python::object &, long, long))
            &PyAttribute::set_value)
        .def("set_value_date_quality",
            (void (*) (Tango::Attribute &, boost::python::object &, double t, Tango::AttrQuality quality))
            &PyAttribute::set_value_date_quality)
        .def("set_value_date_quality",
            (void (*) (Tango::Attribute &, boost::python::str &, boost::python::str &, double t, Tango::AttrQuality quality))
            &PyAttribute::set_value_date_quality)
        .def("set_value_date_quality",
            (void (*) (Tango::Attribute &, boost::python::object &, double t, Tango::AttrQuality quality, long))
            &PyAttribute::set_value_date_quality)
        .def("set_value_date_quality",
            (void (*) (Tango::Attribute &, boost::python::object &, double t, Tango::AttrQuality quality, long, long))
            &PyAttribute::set_value_date_quality)
        
        .def("set_change_event", &Tango::Attribute::set_change_event, 
            set_change_event_overloads())
        .def("set_archive_event", &Tango::Attribute::set_archive_event, 
            set_archive_event_overloads())
            
        .def("is_change_event", &Tango::Attribute::is_change_event)
        .def("is_check_change_criteria", &Tango::Attribute::is_check_change_criteria)
        .def("is_archive_event", &Tango::Attribute::is_archive_event)
        .def("is_check_archive_criteria", &Tango::Attribute::is_check_archive_criteria)
        .def("remove_configuration", &Tango::Attribute::remove_configuration)
    ;
}
