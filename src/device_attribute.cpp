#include "device_attribute.h"

#include "pytgutils.h"
#include "tango_numpy.h"

#include <iostream>
#include <string>
#include <memory>
#include <boost/python.hpp>
#include <tango.h>

using namespace boost::python;

extern const char *non_valid_image;
extern const char *non_valid_spectrum;

// Why am I storing 'type' as a python attribute with object::attr
// instead of as a property calling DeviceAttribute::get_type here?
// Because after 'extract'ing, any call to get_type() will fail. Same
// for "value" and "w_value". And for has_failed and is_empty...
static const char* value_attr_name = "value";
static const char* w_value_attr_name = "w_value";
static const char* type_attr_name = "type";
static const char* is_empty_attr_name = "is_empty";
static const char* has_failed_attr_name = "has_failed";


template<long tangoTypeConst>
struct python_tangocpp {
    typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;

    static inline void to_cpp(const object & py_value, TangoScalarType & result)
    {
        result = extract<TangoScalarType>(py_value);
    }

    static inline object to_python(const TangoScalarType & value)
    {
        return object(value);
    }
};

template<>
struct python_tangocpp<Tango::DEV_STRING> {
    static const long tangoTypeConst = Tango::DEV_STRING;
    typedef TANGO_const2type(tangoTypeConst) TangoScalarType;

    static inline void to_cpp(const object & py_value, TangoScalarType & result)
    {
        result = CORBA::string_dup(extract<TangoScalarType>(py_value));
    }

    static inline object to_python(const TangoScalarType & value)
    {
        return object(std::string(value));
    }
};

#ifndef DISABLE_PYTANGO_NUMPY
#   include "device_attribute_numpy.hpp"
#endif

namespace PyDeviceAttribute {

    template<long tangoTypeConst>
    static inline void _update_scalar_values(Tango::DeviceAttribute &self, object py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;

        if (self.get_written_dim_x() > 0) {
            std::vector<TangoScalarType> val;
            self >> val;
            // In the following lines, the cast is absolutely necessary because
            // vector<TangoScalarType> may not be a vector<TangoScalarType> at
            // compile time. For example, for vector<DevBoolean>, the compiler
            // may create a std::_Bit_reference type.
            py_value.attr(value_attr_name) = object((TangoScalarType)val[0]);
            py_value.attr(w_value_attr_name) = object((TangoScalarType)val[1]);
        } else {
            TangoScalarType rvalue;
            self >> rvalue;
            py_value.attr(value_attr_name) = object(rvalue);
            py_value.attr(w_value_attr_name) = object();
        }
    }

    template<>
    inline void _update_scalar_values<Tango::DEV_ENCODED>(Tango::DeviceAttribute &self, object py_value)
    {
        ///@todo @test this SHIT. Moreover, does DevEncoded really make sense with spectrums?

        Tango::DevVarEncodedArray* value;
        self >> value;
        std::auto_ptr<Tango::DevVarEncodedArray> guard(value);

        Tango::DevEncoded* buffer = value->get_buffer();

        /// @todo I am making a copy of encoded_data as string. This
        /// is far from ideal, but it's what we have now...
        using namespace boost::python;
        if (self.get_written_dim_x() > 0) {
            str encoded_format(buffer[0].encoded_format);
            str w_encoded_format(buffer[1].encoded_format);

            str encoded_data(buffer[0].encoded_data.get_buffer());
            str w_encoded_data(buffer[1].encoded_data.get_buffer());

            py_value.attr(value_attr_name) = make_tuple(encoded_format, encoded_data);
            py_value.attr(w_value_attr_name) = make_tuple(w_encoded_format, w_encoded_data);
        } else {
            str encoded_format(buffer[0].encoded_format);
            str encoded_data(buffer[0].encoded_data.get_buffer());

            py_value.attr(value_attr_name) = make_tuple(encoded_format, encoded_data);
            py_value.attr(w_value_attr_name) = object();
        }
    }

    template<>
    inline void _update_scalar_values<Tango::DEV_STRING>(Tango::DeviceAttribute &self, object py_value)
    {
        if (self.get_written_dim_x() > 0) { /// @todo not the same as old pytango, 'writable' missing...
            std::vector<std::string> val;
            self >> val;

            py_value.attr(value_attr_name) = object(val[0]);
            py_value.attr(w_value_attr_name) = object(val[1]);
        } else {
            std::string rvalue;
            self >> rvalue;

            py_value.attr(value_attr_name) = object(rvalue);
            py_value.attr(w_value_attr_name) = object();
        }
    }

    template<long tangoTypeConst>
    static inline void _update_array_values_as_lists(Tango::DeviceAttribute &self, bool isImage, object py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        // Extract the actual data from Tango::DeviceAttribute (self)
        TangoArrayType* value_ptr = 0;
        try {
            self >> value_ptr;
        } catch (Tango::DevFailed &e ) {
            if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
                throw;
        }
        std::auto_ptr<TangoArrayType> guard_value_ptr(value_ptr);

        if (value_ptr == 0) {
            // Empty device attribute
            py_value.attr(value_attr_name) = boost::python::list();
            py_value.attr(w_value_attr_name) = object();
            return;
        }

        TangoScalarType* buffer = value_ptr->get_buffer();

        // Convert to a list of lists
        long offset = 0;
        for(int it=1; it>=0; --it) { // 2 iterations: read part/write part
            boost::python::list result;
            if (isImage) {
                const int dim_x = it? self.get_dim_x() : self.get_written_dim_x();
                const int dim_y = it? self.get_dim_y() : self.get_written_dim_y();

                for (int y=0; y < dim_y; ++y) {
                    boost::python::list row;
                    for (int x=0; x < dim_x; ++x)
                        row.append(python_tangocpp<tangoTypeConst>::to_python(buffer[offset + x + y*dim_x]));
                    result.append(row);
                }
                offset += dim_x*dim_y;
            } else {
                const int dim_x = it? self.get_dim_x() : self.get_written_dim_x();
                for (int x=0; x < dim_x; ++x)
                    result.append(python_tangocpp<tangoTypeConst>::to_python(buffer[x]));
                offset += dim_x;
            }
            py_value.attr(it? value_attr_name : w_value_attr_name) = result;
        }
    }

    template<long tangoTypeConst>
    static inline void _update_array_values_as_pytango3(Tango::DeviceAttribute &self, bool isImage, object py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        // Extract the actual data from Tango::DeviceAttribute (self)
        TangoArrayType* value_ptr = 0;
        try {
            self >> value_ptr;
        } catch (Tango::DevFailed &e ) {
            if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
                throw;
        }
        std::auto_ptr<TangoArrayType> guard_value_ptr(value_ptr);

        if (value_ptr == 0) {
            // Empty device attribute
            py_value.attr(value_attr_name) = boost::python::list();
            py_value.attr(w_value_attr_name) = object();
            return;
        }

        TangoScalarType* buffer = value_ptr->get_buffer();

        long sz = value_ptr->length();
        boost::python::list res;
        for (long x =0; x<sz; ++x) {
            res.append(python_tangocpp<tangoTypeConst>::to_python(buffer[x]));
        }

        py_value.attr(value_attr_name) = res;
        py_value.attr(w_value_attr_name) = object();
    }

    template<>
    inline void _update_array_values_as_pytango3<Tango::DEV_ENCODED>(Tango::DeviceAttribute &self, bool isImage, object py_value)
    {
        /// @todo DevEncoded didn't even exist in PyTango3...
        _update_array_values_as_tuples<Tango::DEV_ENCODED>(self, isImage, py_value);
    }

    template<long tangoTypeConst>
    void _update_array_values_as_tuples(Tango::DeviceAttribute &self, bool isImage, object py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        // Extract the actual data from Tango::DeviceAttribute (self)
        TangoArrayType* value_ptr = 0;
        try {
            self >> value_ptr;
        } catch (Tango::DevFailed &e ) {
            if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
                throw;
        }
        std::auto_ptr<TangoArrayType> guard_value_ptr(value_ptr);

        if (value_ptr == 0) {
            // Empty device attribute
            py_value.attr(value_attr_name) = boost::python::tuple();
            py_value.attr(w_value_attr_name) = object();
            return;
        }

        TangoScalarType* buffer = value_ptr->get_buffer();

        // Convert to a list of lists
        long offset = 0;
        for(int it=1; it>=0; --it) { // 2 iterations: read part/write part
            //boost::python::list result;
            object result_guard;
            if (isImage) {
                const int dim_x = it? self.get_dim_x() : self.get_written_dim_x();
                const int dim_y = it? self.get_dim_y() : self.get_written_dim_y();

                PyObject * result = PyTuple_New(dim_y);
                if (!result)
                    boost::python::throw_error_already_set();
                result_guard = object(handle<>(result));

                for (int y=0; y < dim_y; ++y) {
                    //boost::python::list row;
                    PyObject * row = PyTuple_New(dim_x);
                    if (!row)
                        boost::python::throw_error_already_set();
                    object row_guard = object(handle<>(row));
                    for (int x=0; x < dim_x; ++x) {
                        object el = python_tangocpp<tangoTypeConst>::to_python(buffer[offset + x + y*dim_x]);
                        PyTuple_SetItem(row, x, el.ptr());
                        incref(el.ptr());
                    }
                    PyTuple_SetItem(result, y, row);
                    incref(row);
                }
                offset += dim_x*dim_y;
            } else {
                const int dim_x = it? self.get_dim_x() : self.get_written_dim_x();

                PyObject * result = PyTuple_New(dim_x);
                if (!result)
                    boost::python::throw_error_already_set();
                result_guard = object(handle<>(result));

                for (int x=0; x < dim_x; ++x) {
                    object el = python_tangocpp<tangoTypeConst>::to_python(buffer[x]);
                    PyTuple_SetItem(result, x, el.ptr());
                    incref(el.ptr());
                }
                offset += dim_x;
            }
            py_value.attr(it? value_attr_name : w_value_attr_name) = result_guard;
        }
    }

    template<long tangoTypeConst>
    static inline void _update_value_as_string(Tango::DeviceAttribute &self, object py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        // Extract the actual data from Tango::DeviceAttribute (self)
        TangoArrayType* value_ptr = 0;
        try {
            self >> value_ptr;
        } catch (Tango::DevFailed &e ) {
            if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
                throw;
        }
        std::auto_ptr<TangoArrayType> guard_value_ptr(value_ptr);

        if (value_ptr == 0) {
            py_value.attr(value_attr_name) = boost::python::str();
            py_value.attr(w_value_attr_name) = object();
            return;
        }

        TangoScalarType* buffer = value_ptr->get_buffer();

        const char *ch_ptr = reinterpret_cast<char *>(buffer);
        size_t nb_bytes = value_ptr->length() * sizeof(TangoScalarType);

        py_value.attr(value_attr_name) = str(ch_ptr, (size_t)nb_bytes);
        py_value.attr(w_value_attr_name) = object();
    }

    void update_values(Tango::DeviceAttribute &self, object& py_value, PyTango::ExtractAs extract_as/*=ExtractAsNumpy*/)
    {
        // We do not want is_empty to launch an exception!!
        self.reset_exceptions(Tango::DeviceAttribute::isempty_flag);
        // self.get_type() already does self.is_empty()
        const int data_type = self.get_type();
        const bool is_empty = data_type < 0; /*self.is_empty()*/
        const bool has_failed = self.has_failed();
        Tango::AttrDataFormat data_format = self.get_data_format();

        py_value.attr(is_empty_attr_name) = object(is_empty);
        py_value.attr(has_failed_attr_name) = object(has_failed);
        py_value.attr(type_attr_name) = object(static_cast<Tango::CmdArgType>(data_type));

        if (has_failed || is_empty) {
            // In none of this cases 'data_type' is valid so we cannot extract
            py_value.attr(value_attr_name) = object();
            py_value.attr(w_value_attr_name) = object();
            return;
        }

        bool is_image = false;
        switch (data_format) {
            case Tango::SCALAR:
                switch (extract_as)
                {
                    default:
                    case PyTango::ExtractAsNumpy:
                    case PyTango::ExtractAsTuple:
                    case PyTango::ExtractAsList:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_scalar_values, self, py_value);
                        break;
                    case PyTango::ExtractAsString:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_value_as_string, self, py_value);
                        break;
                    case PyTango::ExtractAsPyTango3:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_scalar_values, self, py_value);
                        py_value.attr("w_scalar_value") = py_value.attr(w_value_attr_name);
                        break;
                    case PyTango::ExtractAsNothing:
                        break;
                }
                break;
            case Tango::IMAGE:
                is_image = true;
            case Tango::SPECTRUM:
                switch (extract_as)
                {
                    default:
                    case PyTango::ExtractAsNumpy:
#                   ifndef DISABLE_PYTANGO_NUMPY
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_array_values,
                                           self, is_image, py_value);
                        break;
#                   endif
                    case PyTango::ExtractAsTuple:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_array_values_as_tuples,
                                           self, is_image, py_value);
                        break;
                    case PyTango::ExtractAsList:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_array_values_as_lists,
                                           self, is_image, py_value);
                        break;
                    case PyTango::ExtractAsString:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_value_as_string,
                                           self, py_value);
                        break;
                    case PyTango::ExtractAsPyTango3:
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE(data_type, _update_array_values_as_pytango3,
                                           self, is_image, py_value);
                        break;
                    case PyTango::ExtractAsNothing:
                        break;
                }
                break;
            case Tango::FMT_UNKNOWN:
            default:
                raise_(PyExc_ValueError, "Can't extract data because: self.get_data_format()=FMT_UNKNOWN");
                assert(false);
        }
    }

    template<long tangoTypeConst>
    static inline void _fill_scalar_attribute(Tango::DeviceAttribute & dev_attr, const object & py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;

	TangoScalarType value;
	from_py<tangoTypeConst>::convert(py_value.ptr(), value);
        dev_attr << const_cast<TangoScalarType&>(value);
    }

    template<>
    inline void _fill_scalar_attribute<Tango::DEV_STRING>(Tango::DeviceAttribute & dev_attr, const object & py_value)
    {
        std::string value = extract<std::string>(py_value);
        dev_attr << value;
    }

    template<>
    inline void _fill_scalar_attribute<Tango::DEV_ENCODED>(Tango::DeviceAttribute & dev_attr, const object & py_value)
    {
        static const long tangoTypeConst = Tango::DEV_ENCODED;
        typedef TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        /// @todo test it!!

        /// @todo Now I am accepting 2 strings: encoded_format, encoded_data. This
        /// is far from a good solution, but its something...

        if (boost::python::len(py_value) != 2) {
            raise_(PyExc_TypeError, "Expecting a tuple of strings: encoded_format, encoded_data");
        }

        boost::python::object encoded_format_str = py_value[0];
        boost::python::object encoded_data_str = py_value[1];

        /// @todo not sure... second parameter of insert is a reference, does it
        /// mean anything? Does he pretend to take ownership of the pointer or
        /// is he making another copy? what should I do?
        char* encoded_format = const_cast<char*>((extract<const char*>(encoded_format_str)()));
        unsigned int encoded_data_len = boost::python::len(encoded_data_str);
        unsigned char* encoded_data = reinterpret_cast<unsigned char*>(const_cast<char*>((extract<const char*>(encoded_data_str)())));
        // void insert(char *&,unsigned char *&,unsigned int);
        dev_attr.insert(encoded_format, encoded_data, encoded_data_len);

        std::string value = extract<std::string>(py_value);
        dev_attr << value;
    }

    template<long tangoTypeConst>
    static inline void _fill_list_attribute(Tango::DeviceAttribute & dev_attr, const bool isImage, const object & py_value)
    {
        typedef typename TANGO_const2type(tangoTypeConst) TangoScalarType;
        typedef typename TANGO_const2arraytype(tangoTypeConst) TangoArrayType;

        long dim_x, dim_y, nelems;

        // -- Check the dimensions
        if (isImage) {
            dim_y = len(py_value);
            dim_x = len(py_value[0]);
            nelems = dim_x * dim_y;
        } else {
            dim_x = len(py_value);
            dim_y = 0;
            nelems = dim_x;
        }

        // -- Allocate memory
        std::auto_ptr<TangoArrayType> value;
        TangoScalarType* buffer = TangoArrayType::allocbuf(nelems);
        try {
            value.reset(new TangoArrayType(nelems, nelems, buffer, true));
        } catch(...) {
            TangoArrayType::freebuf(buffer);
            throw;
        }

        // -- Copy the sequence to the newly created buffer
        if (isImage) {
            for(long y = 0; y < dim_y; ++y) {
                object py_sub = py_value[y];
                if (len(py_sub) != dim_x)
                    raise_(PyExc_TypeError, non_valid_image);
                for(long x = 0; x < dim_x; ++x) {
                    python_tangocpp<tangoTypeConst>::to_cpp(py_sub[x], buffer[x + y*dim_x]);
                }
            }
        } else {
            for(long x = 0; x < dim_x; ++x) {
                python_tangocpp<tangoTypeConst>::to_cpp(py_value[x], buffer[x]);
            }
        }

        // -- Insert it into the dev_attr
        dev_attr.insert(value.get(), dim_x, dim_y);

        // -- Final cleaning
        value.release(); // Do not delete value, it is handled by dev_attr now!
    }

    template<>
    inline void _fill_list_attribute<Tango::DEV_ENCODED>(Tango::DeviceAttribute & dev_attr, const bool isImage, const object & py_value)
    {
        /// @todo really? This is really not gonna happen?
        // Unsupported
        assert(false);
    }

    static inline object undefined_attribute(Tango::DeviceAttribute* self)
    {
        return object(); // None
    }

    void reset_values(Tango::DeviceAttribute & self, int data_type, Tango::AttrDataFormat data_format, object py_value)
    {
        bool isImage = false;
        switch(data_format)
        {
            case Tango::SCALAR:
                TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE( data_type, _fill_scalar_attribute, self, py_value );
                break;
            case Tango::IMAGE:
                isImage = true;
            case Tango::SPECTRUM:
                // Why on earth? Why do we define _fill_numpy_attribute instead
                // of just using _fill_list_attribute, if the latter accepts
                // anything with operators "[]" and "len" defined?
                // Well it seems that PyArray_GETITEM does something diferent
                // and I get a value transformed into a python basic type,
                // while py_value[y][x] gives me a numpy type (ej: numpy.bool_).
                // Then the conversions between numpy types to c++ are not
                // defined by boost while the conversions between python
                // standard types and C++ are.
#               ifdef DISABLE_PYTANGO_NUMPY
                {
                    TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE( data_type, _fill_list_attribute, self, isImage, py_value );
                }
#               else
                {
                    if (PyArray_Check(py_value.ptr()))
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE( data_type, _fill_numpy_attribute, self, isImage, py_value );
                    else
                        TANGO_CALL_ON_ATTRIBUTE_DATA_TYPE( data_type, _fill_list_attribute, self, isImage, py_value );
                    break;
                }
#               endif
            default:
                raise_(PyExc_TypeError, "unsupported data_format.");
        }
    }

    void reset(Tango::DeviceAttribute & self, const Tango::AttributeInfo &attr_info, object py_value)
    {
        self.set_name(const_cast<std::string&>(attr_info.name));
        reset_values(self, attr_info.data_type, attr_info.data_format, py_value);
    }

    void reset(Tango::DeviceAttribute & self, const std::string &attr_name, Tango::DeviceProxy &dev_proxy, object py_value)
    {
        Tango::AttributeInfoEx attr_info;
        {
            AutoPythonAllowThreads guard;
            attr_info = dev_proxy.get_attribute_config(attr_name);
        }
        reset(self, attr_info, py_value);
    }

};

void export_device_attribute()
{
    class_<Tango::DeviceAttribute> DeviceAttribute("DeviceAttribute",
        init<>())
    ;

    scope da_scope = DeviceAttribute;

    enum_<Tango::DeviceAttribute::except_flags>("except_flags",
        "an enumerated for the exception flags")
        .value("isempty_flag", Tango::DeviceAttribute::isempty_flag)
        .value("wrongtype_flag", Tango::DeviceAttribute::wrongtype_flag)
        .value("failed_flag", Tango::DeviceAttribute::failed_flag)
        .value("numFlags", Tango::DeviceAttribute::numFlags)
    ;

    DeviceAttribute
        .def(init<const Tango::DeviceAttribute &>())

        .def_readwrite("name", &Tango::DeviceAttribute::name)

        .def_readonly("quality", &Tango::DeviceAttribute::quality)

        .def_readonly("time", &Tango::DeviceAttribute::time)

        .add_property("dim_x", &Tango::DeviceAttribute::get_dim_x)

        .add_property("dim_y", &Tango::DeviceAttribute::get_dim_y)

        .add_property("w_dim_x", &Tango::DeviceAttribute::get_written_dim_x)

        .add_property("w_dim_y", &Tango::DeviceAttribute::get_written_dim_y)

        .add_property("r_dimension", &Tango::DeviceAttribute::get_r_dimension)

        .add_property("w_dimension", &Tango::DeviceAttribute::get_w_dimension)

        .add_property("nb_read", &Tango::DeviceAttribute::get_nb_read)

        .add_property("nb_written", &Tango::DeviceAttribute::get_nb_written)

        .add_property("data_format", &Tango::DeviceAttribute::get_data_format)
            
        .def("get_date", &Tango::DeviceAttribute::get_date,
            return_internal_reference<>())

        .def("get_err_stack", &Tango::DeviceAttribute::get_err_stack,
            return_value_policy<copy_const_reference>())

    ;
}
