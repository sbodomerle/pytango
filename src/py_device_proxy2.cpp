static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy2.cpp,v 1.4 2008/01/16 11:10:22 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy2.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a first subset   of Tango base types
//			(DeviceInfo, CommandInfo, .. etc)
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.4 $
//
// $Log: py_device_proxy2.cpp,v $
// Revision 1.4  2008/01/16 11:10:22  tiagocoutinho
// fix pydoc
//
// Revision 1.3  2005/07/19 12:05:52  dusdan
// by Vincenzo Forchi`
//
// fixed bug in error handling in push_event (segfault)
// unified format of deverrorlist (tuple of dictionaries)
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


std::string device_info_to_string(Tango::DeviceInfo const& dev_info)
{
	std::ostringstream s;
	s<<"DeviceInfo[\ndev_class : "<<dev_info.dev_class<<",\n";
	s<<"server_id : "<<dev_info.server_id<<",\n";
	s<<"server_host : "<<dev_info.server_host<<",\n";
	s<<"server_version : "<<dev_info.server_version<<",\n";
	s<<"doc_url : "<<dev_info.doc_url<<"]";
	return s.str();
}

std::string device_importinfo_to_string(Tango::DbDevImportInfo const& dev_importinfo)
{
	std::ostringstream s;
	s<<"DbDevImportInfo[\nname : "<<dev_importinfo.name<<",\n";
	s<<"exported : "<<dev_importinfo.exported<<",\n";
	s<<"ior : "<<dev_importinfo.ior<<",\n";
	s<<"version : "<<dev_importinfo.version<<"]";
	return s.str();
}

std::string device_cominfo_to_string(Tango::DevCommandInfo const& dev_cominfo)
{
	std::ostringstream s;
	s<<"DevCommandInfo[\ncmd_name : "<<dev_cominfo.cmd_name<<",\n";
	s<<"cmd_tag : "<<dev_cominfo.cmd_tag<<",\n";
	s<<"in_type : "<<dev_cominfo.in_type<<",\n";
	s<<"out_type : "<<dev_cominfo.out_type<<",\n";
	s<<"in_type_desc : "<<dev_cominfo.in_type_desc<<",\n";
	s<<"out_type_desc : "<<dev_cominfo.out_type_desc<<"]";
	return s.str();
}

std::string cominfo_to_string(Tango::CommandInfo const& cominfo)
{
	std::ostringstream s;
	s<<"CommandInfo[\ncmd_name : "<<cominfo.cmd_name<<",\n";
	s<<"cmd_tag : "<<cominfo.cmd_tag<<",\n";
	s<<"in_type : "<<cominfo.in_type<<",\n";
	s<<"out_type : "<<cominfo.out_type<<",\n";
	s<<"in_type_desc : "<<cominfo.in_type_desc<<",\n";
	s<<"out_type_desc : "<<cominfo.out_type_desc<<",\n";
	s<<"disp_level : "<<cominfo.disp_level<<"]";
	return s.str();
}

std::string deverror_to_string(Tango::DevError const& dev_error)
{
	std::ostringstream s;
	s<<"DevError[\nreason : "<<dev_error.reason<<",\n";
	s<<"severity : "<<dev_error.severity<<",\n";
	s<<"desc : "<<dev_error.desc<<",\n";
	s<<"origin : "<<dev_error.origin<<"]";
	return s.str();
}

std::string timeval_to_string(Tango::TimeVal const& time_val)
{
	std::ostringstream s;
	s<<"TimeVal[\ntv_sec : "<<time_val.tv_sec<<",\n";
	s<<"tv_usec : "<<time_val.tv_usec<<",\n";
	s<<"tv_nsec : "<<time_val.tv_nsec<<"]";
	return s.str();
}

std::string datahist_to_string(DeviceDataHistory const& data_hist)
{
	std::ostringstream s;
        std::string strtemp;
	s<<"DeviceDataHistory[\ntime : "<<timeval_to_string(data_hist.time)<<",\n";
	s<<"cmd_failed : "<<data_hist.cmd_failed<<",\n";
        strtemp = boost::python::extract<std::string>((data_hist.value).attr("__str__")());
	s<<"value : "<<strtemp<<",\n";
        strtemp = boost::python::extract<std::string>((data_hist.errors).attr("__str__")());
	s<<"errors : "<<strtemp<<"]";
	return s.str();
}

std::string deverror_reason_to_string(Tango::DevError const& dev_error)
{
	std::ostringstream s;
	s<<dev_error.reason;
	return s.str();
}

void declare_base_types_continued();
void declare_base_types()
{
	using namespace boost::python;

     class_<Tango::DeviceInfo>("DeviceInfo",
		 "A structure containing available information for a device with the\n"
		 "following members,\n"
		 " - dev_class : string\n"
		 " - server_id : string\n"
		 " - server_host : string\n"
		 " - server_version : integer\n"
		 " - doc_url : string"
         )
        .def_readonly("dev_class", &Tango::DeviceInfo::dev_class)
        .def_readonly("server_id", &Tango::DeviceInfo::server_id)
        .def_readonly("server_host", &Tango::DeviceInfo::server_host)
        .def_readonly("server_version", &Tango::DeviceInfo::server_version)
        .def_readonly("doc_url", &Tango::DeviceInfo::doc_url)
        .def_readonly("dev_type", &Tango::DeviceInfo::dev_type)
		.def("__str__",&device_info_to_string)
	;

     class_<Tango::DbDevImportInfo>("DbDevImportInfo",
		 "import info for a device (should be retrived from the database) with\n"
		 "the following members,\n"
		 " - name : device name\n"
		 " - exported : 1 if device is running, 0 else\n"
		 " - ior : CORBA reference of the device\n"
		 " - version : string"
         )
        .def_readonly("name", &Tango::DbDevImportInfo::name)
        .def_readonly("exported", &Tango::DbDevImportInfo::exported)
        .def_readonly("ior", &Tango::DbDevImportInfo::ior)
		.def("__str__",&device_importinfo_to_string)
	;

     class_<Tango::DevCommandInfo>("DevCommandInfo")
        .def_readonly("cmd_name", &Tango::DevCommandInfo::cmd_name)
        .def_readonly("cmd_tag", &Tango::DevCommandInfo::cmd_tag)
        .def_readonly("in_type", &Tango::DevCommandInfo::in_type)
        .def_readonly("out_type", &Tango::DevCommandInfo::out_type)
        .def_readonly("in_type_desc", &Tango::DevCommandInfo::in_type_desc)
        .def_readonly("out_type_desc", &Tango::DevCommandInfo::out_type_desc)
		.def("__str__",&device_cominfo_to_string)
	;

     class_<Tango::CommandInfo , bases<Tango::DevCommandInfo> >("CommandInfo",
		 "A device command info with the following members,\n"
		 " - cmd_name : command name as ascci string\n"
		 " - cmd_tag : command as binary value (for TACO)\n"
		 " - in_type : input type as binary value (integer)\n"
		 " - out_type : output type as binary value (integer)\n"
		 " - in_type_desc : description of input type (optional)\n"
		 " - out_type_desc : description of output type (optional)\n"
		 " - disp_level : command display level (DispLevel type)"
         )
        .def_readonly("disp_level", &Tango::CommandInfo::disp_level)
		.def("__str__",&cominfo_to_string)
	;

     class_<Tango::DevError>("DevError",
		 "Structure discribing any error resulting from a command execution,\n"
		 "or an attribute query, with following members,\n"
		 " - reason : string\n"
		 " - severity : one of ErrSeverity.WARN, ERRSeverity.ERR\n"
		 "              or ErrSeverity.PANIC constants\n"
		 " - desc : error description (string)\n"
		 " - out_type : output type as binary value (integer)\n"
		 " - origin : Tango server method in which the error happened"
         )
        //.def_readonly("reason", &Tango::DevError::reason::in)
        .def("reason", &deverror_reason_to_string)
        .def_readonly("severity", &Tango::DevError::severity)
        .def_readonly("desc", &Tango::DevError::desc)
        .def_readonly("origin", &Tango::DevError::origin)
		.def("__str__",&deverror_to_string)
	;

     class_<Tango::TimeVal>("TimeVal",
		 "Time value structure,\n"
		 " - tv_sec : seconds\n"
		 " - tv_usec : microseconds\n"
		 " - ty_nsec : nanoseconds"
         )
        .def_readonly("tv_sec", &Tango::TimeVal::tv_sec)
        .def_readonly("tv_usec", &Tango::TimeVal::tv_usec)
        .def_readonly("tv_nsec", &Tango::TimeVal::tv_nsec)
		.def("__str__",&timeval_to_string)
	;

     class_<DeviceDataHistory>("DeviceDataHistory",
		 "Structure members,\n"
		 " - time : time of command execution (see TimeVal type)\n"
		 " - cmd_failed : true if attribute command execution failed\n"
		 " - value : returned value as a python object,\n"
		 "           valid if cmd_failed is false,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if cmd_failed is false"
         )
        .def_readonly("time", &DeviceDataHistory::time)
        .def_readonly("cmd_failed", &DeviceDataHistory::cmd_failed)
        .def_readonly("value", &DeviceDataHistory::value)
        .def_readonly("errors", &DeviceDataHistory::errors)
		.def("__str__",&datahist_to_string)
	;

     declare_base_types_continued();
}































