static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy3.cpp,v 1.10 2008/05/06 11:52:27 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy3.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a first subset   of Tango base types
//			(AttributeInfo, AttributeValue, .. etc)
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.10 $
//
// $Log: py_device_proxy3.cpp,v $
// Revision 1.10  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.9  2008/01/22 09:21:18  tiagocoutinho
// - fix bug in data structure access. Some members were defined as read-only but in fact they can be changed so their access as been changed to readwrite.
//
// Revision 1.8  2008/01/16 11:11:21  tiagocoutinho
// fix newlines at end of file
//
// Revision 1.7  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.6  2007/03/12 16:54:20  tiagocoutinho
// -added missing AttributeInfoEx API. Fix some bugs
//
// Revision 1.5  2007/03/07 10:50:55  tiagocoutinho
// - Support for AttributeInfoEx
// - Support for attribute configuration events
//
// Revision 1.4  2007/03/06 09:52:05  tiagocoutinho
// - fixed bug 1669037
//
// Revision 1.3  2006/04/13 13:40:03  taurel
// Several changes:
// - Add wrapping of the new set/get_transparency_reconnection AttributeProxy class methods
// - Add wrapping of the new AttributeProxy class ctor (with DeviceProxy)
// - Fix bug: Now all Pthon exceptions inherit from PyTango.DevFailed
// - Exception name from WrongSynTax to WrongNameSyntax and Communication to CommunicationFailed to be coherent with C++
// - Change the way attribute data type are managed in AttributeProxy class to speed up read() and write() methods
// - Change the R/W type for some elements of the AttributeInfo class. All of them were read only
//
// Revision 1.2  2004/01/27 09:59:57  ounsy
// Added asynchronous related methods
//
// Revision 1.1  2003/10/23 13:50:55  ounsy
//
//
// Added Python binding for TANGO
//
//
// copyleft :     Synchrotron SOLEIL
//			L'Orme des Merisiers, Saint-Aubin BP 48
//			91192 GIF SUR YVETTE Cedex
//
//-=============================================================================

#include <device_proxy.h>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/exception_translator.hpp>
#include <exception>

std::string attralarminfo_to_string(AttributeAlarmInfo const& attr_alarm_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"AttributeAlarmInfo[\nmin_alarm : "<<attr_alarm_info.min_alarm<<",\n";
	s<<"max_alarm : "<<attr_alarm_info.max_alarm<<",\n";
	s<<"min_warning : "<<attr_alarm_info.min_warning<<",\n";
	s<<"max_warning : "<<attr_alarm_info.max_warning<<",\n";
	s<<"delta_t : "<<attr_alarm_info.delta_t<<",\n";
	s<<"delta_val : "<<attr_alarm_info.delta_val<<",\n";
        strtemp = boost::python::extract<std::string>((attr_alarm_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<"]";
	return s.str();
}

std::string chgevtinfo_to_string(ChangeEventInfo const& chg_evt_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"ChangeEventInfo[\nrel_change : "<<chg_evt_info.rel_change<<",\n";
	s<<"abs_change : "<<chg_evt_info.abs_change<<",\n";
        strtemp = boost::python::extract<std::string>((chg_evt_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<"]";
	return s.str();
}

std::string periodicevtinfo_to_string(PeriodicEventInfo const& periodic_evt_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"PeriodicEventInfo[\nperiod : "<<periodic_evt_info.period<<",\n";
        strtemp = boost::python::extract<std::string>((periodic_evt_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<"]";
	return s.str();
}

std::string archevtinfo_to_string(ArchiveEventInfo const& arch_evt_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"ArchiveEventInfo[\narchive_rel_change : "<<arch_evt_info.archive_rel_change<<",\n";
	s<<"archive_abs_change : "<<arch_evt_info.archive_abs_change<<",\n";
	s<<"archive_period : "<<arch_evt_info.archive_period<<",\n";
        strtemp = boost::python::extract<std::string>((arch_evt_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<"]";
	return s.str();
}

std::string attrevtinfo_to_string(AttributeEventInfo const& attr_evt_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"AttributeEventInfo[\n";
	s<<"ch_event : "<< chgevtinfo_to_string(attr_evt_info.ch_event) <<",\n";
	s<<"per_event : "<< periodicevtinfo_to_string(attr_evt_info.per_event) <<",\n";
	s<<"arch_event : "<< archevtinfo_to_string(attr_evt_info.arch_event) <<"]";
	return s.str();
}

std::string attrinfo_to_string(AttributeInfo const& attr_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"AttributeInfo[\nname : "<<attr_info.name<<",\n";
	s<<"writable : "<<attr_info.writable<<",\n";
	s<<"data_format : "<<attr_info.data_format<<",\n";
	s<<"data_type : "<<attr_info.data_type<<",\n";
	s<<"max_dim_x : "<<attr_info.max_dim_x<<",\n";
	s<<"max_dim_y : "<<attr_info.max_dim_y<<",\n";
	s<<"description : "<<attr_info.description<<",\n";
	s<<"label : "<<attr_info.label<<",\n";
	s<<"unit : "<<attr_info.unit<<",\n";
	s<<"standard_unit : "<<attr_info.standard_unit<<",\n";
	s<<"display_unit : "<<attr_info.display_unit<<",\n";
	s<<"format : "<<attr_info.format<<",\n";
	s<<"min_value : "<<attr_info.min_value<<",\n";
	s<<"max_value : "<<attr_info.max_value<<",\n";
	s<<"mini_alarm : "<<attr_info.min_alarm<<",\n";
	s<<"max_alarm : "<<attr_info.max_alarm<<",\n";
	s<<"writable_attr_name : "<<attr_info.writable_attr_name<<",\n";
        strtemp = boost::python::extract<std::string>((attr_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<",\n";
	s<<"disp_level : "<<attr_info.disp_level<<"]";
	return s.str();
}

std::string attrinfoex_to_string(AttributeInfoEx const& attr_info)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"AttributeInfoEx[\nname : "<<attr_info.name<<",\n";
	s<<"writable : "<<attr_info.writable<<",\n";
	s<<"data_format : "<<attr_info.data_format<<",\n";
	s<<"data_type : "<<attr_info.data_type<<",\n";
	s<<"max_dim_x : "<<attr_info.max_dim_x<<",\n";
	s<<"max_dim_y : "<<attr_info.max_dim_y<<",\n";
	s<<"description : "<<attr_info.description<<",\n";
	s<<"label : "<<attr_info.label<<",\n";
	s<<"unit : "<<attr_info.unit<<",\n";
	s<<"standard_unit : "<<attr_info.standard_unit<<",\n";
	s<<"display_unit : "<<attr_info.display_unit<<",\n";
	s<<"format : "<<attr_info.format<<",\n";
	s<<"min_value : "<<attr_info.min_value<<",\n";
	s<<"max_value : "<<attr_info.max_value<<",\n";
	s<<"mini_alarm : "<<attr_info.min_alarm<<",\n";
	s<<"max_alarm : "<<attr_info.max_alarm<<",\n";
	s<<"writable_attr_name : "<<attr_info.writable_attr_name<<",\n";
        strtemp = boost::python::extract<std::string>((attr_info.extensions).attr("__str__")());
	s<<"extensions : "<<strtemp<<",\n";
	s<<"disp_level : "<<attr_info.disp_level<<",\n";
	s<<"alarms : "<< attralarminfo_to_string(attr_info.alarms) <<",\n";
	s<<"events : "<< attrevtinfo_to_string(attr_info.events) <<",\n";
		strtemp = boost::python::extract<std::string>((attr_info.sys_extensions).attr("__str__")());
	s<<"sys_extensions : "<<strtemp<<"]";
	return s.str();
}

std::string attrvalue_to_string(AttributeValue const& attr_value)
{
	std::ostringstream s;
	std::string strtemp;
	boost::python::object obj;
	
	strtemp = boost::python::extract<std::string>((attr_value.value).attr("__str__")());
	s<<"AttributeValue[\nvalue : "<<strtemp<<",\n";

	strtemp = boost::python::extract<std::string>((attr_value.scalar_w_value).attr("__str__")());
	s<<"scalar_w_value : "<<strtemp<<",\n";

	obj = (boost::python::object)attr_value.time;
	strtemp = boost::python::extract<std::string>(obj.attr("__str__")());
	s<<"time : "<<strtemp<<",\n";
	
	s<<"quality : "<<attr_value.quality<<",\n";
	s<<"name : "<<attr_value.name<<",\n";
	s<<"dim_x : "<<attr_value.dim_x<<",\n";
	s<<"dim_y : "<<attr_value.dim_y<<",\n";
	s<<"w_dim_x : "<<attr_value.w_dim_x<<",\n";
	s<<"w_dim_y : "<<attr_value.w_dim_y<<"]";
	return s.str();
}

std::string attrhist_to_string(DeviceAttributeHistory const& attr_hist)
{
	std::ostringstream s;
        std::string strtemp;
        boost::python::object obj;
	s<<"AttributeHistory[\nattr_failed : "<<attr_hist.attr_failed<<",\n";
        obj = (boost::python::object)attr_hist.value;
        strtemp = boost::python::extract<std::string>(obj.attr("__str__")());
	s<<"value : "<<strtemp<<",\n";
        obj = (boost::python::object)attr_hist.errors;
        strtemp = boost::python::extract<std::string>(obj.attr("__str__")());
	s<<"errors : "<<strtemp<<"]";
	return s.str();
}


void declare_database_types();
//BOOST_PYTHON_MODULE(PyTango)
void declare_base_types_continued()
{
    using namespace boost::python;

	class_<AttributeAlarmInfo>("AttributeAlarmInfo",
		"A structure containing available alarm information for an attribute\n"
		"with the folowing members,\n"
		" - min_alarm : string\n"
		" - max_alarm : string\n"
		" - min_warning : string\n"
		" - max_warning : string\n"
		" - delta_t : string\n"
		" - delta_val : string\n"
		" - extensions : list of strings\n"
		)
       .def_readwrite("min_alarm", &AttributeAlarmInfo::min_alarm)
       .def_readwrite("max_alarm", &AttributeAlarmInfo::max_alarm)
       .def_readwrite("min_warning", &AttributeAlarmInfo::min_warning)
       .def_readwrite("max_warning", &AttributeAlarmInfo::max_warning)
       .def_readwrite("delta_t", &AttributeAlarmInfo::delta_t)
       .def_readwrite("delta_val", &AttributeAlarmInfo::delta_val)
       .def_readwrite("extensions", &AttributeAlarmInfo::extensions)
       .def("__str__",&attralarminfo_to_string)
	;
	 
     class_<ChangeEventInfo>("ChangeEventInfo",
		"A structure containing available change event information for an attribute\n"
		"with the folowing members,\n"
		" - rel_change : string\n"
		" - abs_change : string\n"
		" - extensions : list of strings\n"
		)
       .def_readwrite("rel_change", &ChangeEventInfo::rel_change)
       .def_readwrite("abs_change", &ChangeEventInfo::abs_change)
       .def_readwrite("extensions", &ChangeEventInfo::extensions)
       .def("__str__",&chgevtinfo_to_string)
	;

     class_<PeriodicEventInfo>("PeriodicEventInfo",
		"A structure containing available periodic event information for an attribute\n"
		"with the folowing members,\n"
		" - period : string\n"
		" - extensions : list of strings\n"
		)
       .def_readwrite("period", &PeriodicEventInfo::period)
       .def_readwrite("extensions", &PeriodicEventInfo::extensions)
       .def("__str__",&periodicevtinfo_to_string)
	;
	
     class_<ArchiveEventInfo>("ArchiveEventInfo",
		"A structure containing available archiving event information for an attribute\n"
		"with the folowing members,\n"
		" - archive_rel_change : string\n"
		" - archive_abs_change : string\n"
		" - extensions : list of strings\n"
		)
       .def_readwrite("archive_rel_change", &ArchiveEventInfo::archive_rel_change)
       .def_readwrite("archive_abs_change", &ArchiveEventInfo::archive_abs_change)
       .def_readwrite("extensions", &ArchiveEventInfo::extensions)
       .def("__str__",&archevtinfo_to_string)
	;

     class_<AttributeEventInfo>("AttributeEventInfo",
		"A structure containing available event information for an attribute\n"
		"with the folowing members,\n"
		" - ch_event : object containing change event information (see ChangeEventInfo)\n"
		" - per_event : object containing periodic event information (see PeriodicEventInfo)\n"
		" - arch_event : object containing archiving event information (see ArchiveEventInfo)\n"
		)
       .def_readwrite("ch_event", &AttributeEventInfo::ch_event)
       .def_readwrite("per_event", &AttributeEventInfo::per_event)
       .def_readwrite("arch_event", &AttributeEventInfo::arch_event)
       .def("__str__",&attrevtinfo_to_string)
	;
	       
     class_<AttributeInfo>("AttributeInfo",
		 "A structure containing available information for an attribute with\n"
		 "the following members,\n"
		 " - name : attribute name\n"
		 " - writable : one of AttrWriteType constant values,\n"
		 "              AttrWriteType.READ, AttrWriteType.READ_WITH_WRITE,\n"
		 "              AttrWriteType.WRITE or AttrWriteType.READ_WRITE\n"
		 " - data_format : one of AttrDataFormat constant values\n"
		 "              AttrWriteType.SCALAR, AttrWriteType.SPECTRUM,\n"
		 "              or AttrWriteType.IMAGE\n"
		 " - data_type : integer value indicating attribute type (float, string,..) \n"
		 " - max_dim_x : first dimension of attribute (spectrum or image attributes)\n"
		 " - max_dim_y : second dimension of attribute(image attribute)\n"
		 " - description : string describing the attribute\n"
		 " - label : attribute label (Voltage, time, ...)\n"
		 " - unit : attribute unit (V, ms, ...)\n"
		 " - standard_unit : string\n"
		 " - display_unit : string\n"
		 " - format : string\n"
		 " - min_value : string\n"
		 " - max_value : string\n"
		 " - min_alarm : string\n"
		 " - max_alarm : string\n"
		 " - writable_attr_name : string\n"
		 " - extensions : list of strings\n"
		 " - disp_level : one of DispLevel constants,\n"
		 "                DispLevel.OPERATOR or DispLevel.EXPERT"
		 " - alarms : object containing alarm information (see AttributeAlarmInfo)."
		 " - events : object containing event information (see AttributeEventInfo)."
		 " - sys_extensions : list of strings\n"
         )
        .def_readonly("name", &AttributeInfo::name)
        .def_readonly("writable", &AttributeInfo::writable)
        .def_readonly("data_format", &AttributeInfo::data_format)
        .def_readonly("data_type", &AttributeInfo::data_type)
        .def_readonly("max_dim_x", &AttributeInfo::max_dim_x)
        .def_readonly("max_dim_y", &AttributeInfo::max_dim_y)
        .def_readwrite("description", &AttributeInfo::description)
        .def_readwrite("label", &AttributeInfo::label)
        .def_readwrite("unit", &AttributeInfo::unit)
        .def_readwrite("standard_unit", &AttributeInfo::standard_unit)
        .def_readwrite("display_unit", &AttributeInfo::display_unit)
        .def_readwrite("format", &AttributeInfo::format)
        .def_readwrite("min_value", &AttributeInfo::min_value)
        .def_readwrite("max_value", &AttributeInfo::max_value)
        .def_readwrite("min_alarm", &AttributeInfo::min_alarm)
        .def_readwrite("max_alarm", &AttributeInfo::max_alarm)
        .def_readwrite("writable_attr_name", &AttributeInfo::writable_attr_name)
        .def_readwrite("extensions", &AttributeInfo::extensions)
        .def_readonly("disp_level", &AttributeInfo::disp_level)
		.def("__str__",&attrinfo_to_string)
	;
	
     class_<AttributeInfoEx>("AttributeInfoEx",
		 "A structure containing available information for an attribute with\n"
		 "the following members,\n"
		 " - name : attribute name\n"
		 " - writable : one of AttrWriteType constant values,\n"
		 "              AttrWriteType.READ, AttrWriteType.READ_WITH_WRITE,\n"
		 "              AttrWriteType.WRITE or AttrWriteType.READ_WRITE\n"
		 " - data_format : one of AttrDataFormat constant values\n"
		 "              AttrWriteType.SCALAR, AttrWriteType.SPECTRUM,\n"
		 "              or AttrWriteType.IMAGE\n"
		 " - data_type : integer value indicating attribute type (float, string,..) \n"
		 " - max_dim_x : first dimension of attribute (spectrum or image attributes)\n"
		 " - max_dim_y : second dimension of attribute(image attribute)\n"
		 " - description : string describing the attribute\n"
		 " - label : attribute label (Voltage, time, ...)\n"
		 " - unit : attribute unit (V, ms, ...)\n"
		 " - standard_unit : string\n"
		 " - display_unit : string\n"
		 " - format : string\n"
		 " - min_value : string\n"
		 " - max_value : string\n"
		 " - min_alarm : string\n"
		 " - max_alarm : string\n"
		 " - writable_attr_name : string\n"
		 " - extensions : list of strings\n"
		 " - disp_level : one of DispLevel constants,\n"
		 "                DispLevel.OPERATOR or DispLevel.EXPERT"
		 " - alarms : object containing alarm information (see AttributeAlarmInfo)."
		 " - events : object containing event information (see AttributeEventInfo)."
		 " - sys_extensions : list of strings\n"
         )
        .def_readonly("name", &AttributeInfoEx::name)
        .def_readonly("writable", &AttributeInfoEx::writable)
        .def_readonly("data_format", &AttributeInfoEx::data_format)
        .def_readonly("data_type", &AttributeInfoEx::data_type)
        .def_readonly("max_dim_x", &AttributeInfoEx::max_dim_x)
        .def_readonly("max_dim_y", &AttributeInfoEx::max_dim_y)
        .def_readwrite("description", &AttributeInfoEx::description)
        .def_readwrite("label", &AttributeInfoEx::label)
        .def_readwrite("unit", &AttributeInfoEx::unit)
        .def_readwrite("standard_unit", &AttributeInfoEx::standard_unit)
        .def_readwrite("display_unit", &AttributeInfoEx::display_unit)
        .def_readwrite("format", &AttributeInfoEx::format)
        .def_readwrite("min_value", &AttributeInfoEx::min_value)
        .def_readwrite("max_value", &AttributeInfoEx::max_value)
        .def_readwrite("min_alarm", &AttributeInfoEx::min_alarm)
        .def_readwrite("max_alarm", &AttributeInfoEx::max_alarm)
        .def_readwrite("writable_attr_name", &AttributeInfoEx::writable_attr_name)
        .def_readwrite("extensions", &AttributeInfoEx::extensions)
        .def_readonly("disp_level", &AttributeInfoEx::disp_level)
        .def_readwrite("alarms", &AttributeInfoEx::alarms)
        .def_readwrite("events", &AttributeInfoEx::events)
        .def_readwrite("sys_extensions", &AttributeInfoEx::sys_extensions)
		.def("__str__",&attrinfoex_to_string)
	;	

     class_<AttributeValue>("AttributeValue",
		 "A structure encapsulating the attribute value with the following\n"
		 "members,\n"
		 " - value : python object with effective value\n"
		 " - scalar_w_value : the set value for a writable scalar attribute. "
		 " None if not applicable (New in PyTango 3.0.4)\n"
		 " - quality : one of AttrQuality constant values,\n"
		 "             AttrQuality.VALID, AttrQuality.INVALID,\n"
		 "             or AttrQuality.ALARM\n"
		 " - time : time of value read (see TimeVal type)\n"
		 " - name : attribute name \n"
		 " - dim_x : effective first dimension of attribute (spectrum or image attributes)\n"
		 " - dim_y : effective second dimension of attribute(image attribute)\n"
		 " - w_dim_x : effective writable first dimension of attribute (writable"
		 "spectrum or image attributes) (New in PyTango 3.0.4)\n"
		 " - w_dim_y : effective writable second dimension of attribute "
		 "(writable image attribute) (New in PyTango 3.0.4)"
         )
        .def_readwrite("value", &AttributeValue::value)
        .def_readwrite("scalar_w_value", &AttributeValue::scalar_w_value)
        .def_readonly("time", &AttributeValue::time)
        .def_readonly("quality", &AttributeValue::quality)
        .def_readwrite("name", &AttributeValue::name)
        .def_readwrite("dim_x", &AttributeValue::dim_x)
        .def_readwrite("dim_y", &AttributeValue::dim_y)
        .def_readwrite("w_dim_x", &AttributeValue::w_dim_x)
        .def_readwrite("w_dim_y", &AttributeValue::w_dim_y)
		.def("__str__",&attrvalue_to_string)
	;

     class_<DeviceAttributeHistory>("DeviceAttributeHistory",
		 "Structure members,\n"
		 " - attr_failed : true if attribute read operation failed\n"
		 " - value : attribute value as an AttributeValue type,\n"
		 "           valid if attr_failed is false,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if attr_failed is false\n"
		 " - name : attribute name \n"
		 " - dim_x : effective first dimension of attribute (spectrum or image attributes)\n"
		 " - dim_y : effective second dimension of attribute(image attribute)"
         )
        .def_readonly("attr_failed", &DeviceAttributeHistory::attr_failed)
        .def_readonly("value", &DeviceAttributeHistory::value)
        .def_readonly("errors", &DeviceAttributeHistory::errors)
		.def("__str__",&attrhist_to_string)
	;

     declare_database_types();
}
