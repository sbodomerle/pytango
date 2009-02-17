static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy5.cpp,v 1.5 2008/05/06 11:52:27 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy5.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a first subset of DeviceProxy methods
//
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.5 $
//
// $Log: py_device_proxy5.cpp,v $
// Revision 1.5  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.4  2006/06/28 09:02:07  taurel
// - Add get/set_transparency_reconnection on DeviceProxy class
//
// Revision 1.3  2006/03/27 08:50:48  taurel
// - Add the Deviceproxy.alisa() method
// - Fix a 32/64 bits problem in the wrapping of the "at" method  (for DS)
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (DeviceProxy_command_overloads,DeviceProxy::command_inout,1,2)

void second_of_device_proxy(boost::python::class_<DeviceProxy> &x);

void first_of_device_proxy(boost::python::class_<DeviceProxy> &devProx)
{

    using namespace boost::python;


    devProx
		.def("__getattr__",&DeviceProxy::getattr)

		.def("state",&DeviceProxy::state, 
		   "A method which returns the state of the device.\n"
		   "Parameters : None\n"
 		   "Return     : DevState constant\n"
           "Example :\n"
           "        dev_st = dev.state()\n"
		   "        if dev_st == DevState.ON : ..."
		   )

        .def("status", &DeviceProxy::status,
		   "A method which returns the status of the device as a string.\n"
		   "Parameters : None\n"
		   "Return     : None"
		   )

        .def("ping", &DeviceProxy::ping,
		   "A method which sends a ping to the device\n"
		   "Parameters : None\n"
		   "Return     : time elapsed in milliseconds"
		   )

        .def("set_timeout_millis", &DeviceProxy::set_timeout_millis,
		   "Set client side timeout for device in milliseconds. Any method which\n"
		   "takes longer than this time to execute will throw an exception\n"
		   "Parameters :\n"
		   "    - timeout : integer value of timeout in milliseconds\n"
 		   "Return     : None\n"
           "Example :\n"
           "        dev.set_timeout_milliseconds(1000)"
		   )

        .def("get_timeout_millis", &DeviceProxy::get_timeout_millis,
		   "Get the client side timeout in milliseconds.\n"
		   "Parameters : None"
		   )
		   
	.def("get_transparency_reconnection", &DeviceProxy::get_transparency_reconnection,
			"Returns the device transparency reconnection flag")
				
	.def("set_transparency_reconnection", &DeviceProxy::set_transparency_reconnection,
			"Set the device transparency reconnection flag")
			

        .def("get_idl_version", &DeviceProxy::get_idl_version,
		   "Get the version of the Tango Device interface implemented\n"
		   "by the device"
		   )

        .def("set_source", &DeviceProxy::set_source,
		   "Set the data source(device, polling buffer, polling buffer then device)\n"
		   "for command_inout and read_attribute methods\n"
		   "Parameters :\n"
		   "    - source : DevSource constant\n"
 		   "Return     : None\n"
           "Example :\n"
           "        dev.set_source(DevSource.CACHE_DEV)"
		   )

        .def("get_source", &DeviceProxy::get_source,
		   "Get the data source(device, polling buffer, polling buffer then device)\n"
		   "used by command_inout or read_attribute methods\n"
		   "Parameters : None\n"
 		   "Return     : DevSource constant\n"
           "Example :\n"
           "        source = dev.get_source()\n"
		   "        if source == DevSource.CACHE_DEV : ..."
		   )

        .def("black_box", &DeviceProxy::black_box,
		   "Get the last commands executed on the device server\n"
		   "Parameters : \n"
		   "    - n : n number of commands to get\n"
 		   "Return     : list of strings containing the date, time, command and from\n"
		   "             which client computer the command was executed\n"
           "Example :\n"
           "        print black_box(4)"
		   )

        .def("name", &DeviceProxy::name,
		   "Return the device name from the device itself"
		   )
		   
        .def("alias", &DeviceProxy::alias,
		   "Return the device alias if one is defined\n"
		   "Otherwise, throws exception"
		   )

        .def("adm_name", &DeviceProxy::adm_name,
		   "Return the name of the corresponding administrator device. This is "
		   "useful if you need to send an administration command to the device "
		   "server, e.g restart it\n"
		   "\nNew in PyTango 3.0.4"
		   )

        .def("dev_name", &DeviceProxy::dev_name,
		   "Return the device name as it is stored locally"
		   )

        .def("info", &DeviceProxy::info,
		   "A method which returns information on the device\n"
		   "Parameters : None\n"
 		   "Return     : DeviceInfo object\n"
           "Example :\n"
           "        dev_info = dev.info()\n"
		   "        print dev_info.dev_class\n"
		   "        print dev_info.server_id\n"
		   "        print dev_info.server_host\n"
		   "        print dev_info.server_version\n"
		   "        print dev_info.doc_url\n"
		   "        print dev_info.dev_type\n"
		   "All DeviceInfo fields are strings except for the server_version which is a\n"
		   "integer"
		   )

		.def("import_info", &DeviceProxy::import_info,
		   "Query the device for import info from the database.\n"
		   "Parameters : None\n"
 		   "Return     : DbDevImportInfo object\n"
           "Example :\n"
           "        dev_import = dev.import_info()\n"
		   "        print dev_import.name\n"
		   "        print dev_import.exported\n"
		   "        print dev_ior.ior\n"
		   "        print dev_version.version\n"
		   "All DbDevImportInfo fields are strings except for exported which is an\n"
		   "integer"
		   )

        .def("description", &DeviceProxy::description,
		   "Get device description.\n"
		   "Parameters : None\n"
 		   "Return     : string describing the device\n"
           "Example :"
		   )

        .def("command_query", &DeviceProxy::command_query,
		   "Query the device for information about a single command.\n"
		   "Parameters : \n"
		   "        - command : command name\n"
 		   "Return     : CommandInfo object\n"
           "Example :\n"
           "        com_info = dev.command_query(""DevString"")\n"
		   "        print com_info.cmd_name\n"
		   "        print com_info.cmd_tag\n"
		   "        print com_info.in_type\n"
		   "        print com_info.out_type\n"
		   "        print com_info.in_type_desc\n"
		   "        print com_info.out_type_desc\n"
		   "        print com_info.disp_level\n"
		   "see CommandInfo documentation string form more detail"
		   )

        .def("command_list_query", &DeviceProxy::command_list_query,
 		   "Query the device for information on all commands.\n"
		   "Parameters : None\n"
 		   "Return     : list of CommandInfo objects"
		   )

		.def("command_inout", &DeviceProxy::command_inout,
               DeviceProxy_command_overloads(args("name","pyData"),
 		   "Execute a command on a device which takes zero or one input argument.\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - argin : if needed, python object containing the input argument\n"
 		   "Return     : result of command execution as a python object\n"
           "Example :\n"
           "        str_res = dev.command_inout(""DevString"",""Hello!"")\n"
		   "        print str_res\n"
		   ))

        .def("command_history", &DeviceProxy::command_history,
 		   "Retrive command history from the command polling buffer.\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - depth : integer representing the wanted history depth\n"
 		   "Return     : a list of DeviceDataHistory types"
           "Example :\n"
           "        com_hist = dev.command_history(""DevString"",3)\n"
		   "        for dev_hist in com_hist : print dev_hist\n"
		   "see DeviceDataHistory documentation string form more detail"
		   )
	;


	second_of_device_proxy(devProx);

}


































