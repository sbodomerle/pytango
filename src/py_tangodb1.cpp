static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_tangodb1.cpp,v 1.4 2008/01/23 12:18:53 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_tangodb1.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a first subset   of Tango Database API 
//			base types (DbDevInfo, .. etc)
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.4 $
//
// $Log: py_tangodb1.cpp,v $
// Revision 1.4  2008/01/23 12:18:53  tiagocoutinho
// - Added missing database API
//
// Revision 1.3  2006/07/26 07:06:43  taurel
// - Add wrapping of all the new calls added in Tango V5.5 to manually firing events
//
// Revision 1.2  2004/01/27 09:59:56  ounsy
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

#include <tangodb.h>
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


std::string dbdev_info_to_string(Tango::DbDevInfo const& dbdev_info)
{
	std::ostringstream s;
	s<<"DbDevInfo[\nname : "<<dbdev_info.name<<",\n";
	s<<"class : "<<dbdev_info._class<<",\n";
	s<<"server : "<<dbdev_info.server<<"]";
	return s.str();
}

std::string dbdev_exportinfo_to_string(Tango::DbDevExportInfo const& dbdev_exportinfo)
{
	std::ostringstream s;
	s<<"DbDevExportInfo[\nname : "<<dbdev_exportinfo.name<<",\n";
	s<<"ior : "<<dbdev_exportinfo.ior<<",\n";
	s<<"host : "<<dbdev_exportinfo.host<<",\n";
	s<<"version : "<<dbdev_exportinfo.version<<",\n";
	s<<"pid : "<<dbdev_exportinfo.pid<<"]";
	return s.str();
}

std::string dbserver_info_to_string(Tango::DbServerInfo const& dev_serverinfo)
{
	std::ostringstream s;
	s<<"DbServerInfo[\nname : "<<dev_serverinfo.name<<",\n";
	s<<"host : "<<dev_serverinfo.host<<",\n";
	s<<"mode : "<<dev_serverinfo.mode<<",\n";
	s<<"level : "<<dev_serverinfo.level<<"]";
	return s.str();
}

//void declare_tangodb();
void declare_device_proxy();
void declare_database_types()
{
    using namespace boost::python;

     class_<Tango::DbDevInfo>("DbDevInfo",
		 "A structure containing available information for a device with\n"
		 "the following members,\n"
		 " - name : string\n"
		 " - class : string\n"
		 " - server : string"
         )
        .def_readwrite("name", &Tango::DbDevInfo::name)
        .def_readwrite("_class", &Tango::DbDevInfo::_class)
        .def_readwrite("server", &Tango::DbDevInfo::server)
		.def("__str__",&dbdev_info_to_string)
	;

     class_<Tango::DbDevExportInfo>("DbDevExportInfo",
		 "export info for a device  with the following members,\n"
		 " - name : device name\n"
		 " - ior : CORBA reference of the device\n"
		 " - host : name of the computer hosting the server\n"
		 " - version : string\n"
		 " - pid : process identifier"
         )
        .def_readwrite("name", &Tango::DbDevExportInfo::name)
        .def_readwrite("ior", &Tango::DbDevExportInfo::ior)
        .def_readwrite("host", &Tango::DbDevExportInfo::host)
        .def_readwrite("version", &Tango::DbDevExportInfo::version)
        .def_readwrite("pid", &Tango::DbDevExportInfo::pid)
		.def("__str__",&dbdev_exportinfo_to_string)
	;

     class_<Tango::DbServerInfo>("DbServerInfo",
     	"A structure containing available information for a device server with\n"
    		"the following members,\n"
    		" - name : string\n"
    		" - host : string\n"
    		" - mode : string\n"
    		" - level : string"
         )
         .def_readonly("name", &Tango::DbServerInfo::name)
         .def_readonly("host", &Tango::DbServerInfo::host)
         .def_readonly("mode", &Tango::DbServerInfo::mode)
         .def_readonly("level", &Tango::DbServerInfo::level)
         .def("__str__",&dbserver_info_to_string)
 	;
     

     declare_device_proxy();
}

