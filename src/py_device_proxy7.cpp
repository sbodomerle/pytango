static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy7.cpp,v 1.2 2006/10/11 10:24:46 taurel Exp $";
//+=============================================================================
//
// file :         py_device_proxy6.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines asynchronous DeviceProxy methods
//
//
// project :      Python Binding
//
// $Author: taurel $
//
// $Revision: 1.2 $
//
// $Log: py_device_proxy7.cpp,v $
// Revision 1.2  2006/10/11 10:24:46  taurel
// - Fix bugs and memory leak in asynchronous calls
//
// Revision 1.1  2004/01/27 09:59:56  ounsy
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


void declare_tangodb();
void device_proxy_asynch(boost::python::class_<DeviceProxy> &devProx)
{
    using namespace boost::python;
    
	devProx

	.def("command_inout_asynch", (void (DeviceProxy::*) (std::string,boost::python::object))&DeviceProxy::command_inout_asynch_cb,
 		"Execute an asynchronous command on a device which takes no input argument.\n"
		"Parameters :\n"
		"        - name : command name\n"
		"        - callback : python object containing the callback object\n"
 		"Return     : None\n"
           	"Example :\n"
           	"        cb = Callback\n"
		"        dev.command_inout_asynch(""Ping"",cb)\n"
		   )

	.def("command_inout_asynch", (void (DeviceProxy::*) (std::string,boost::python::object,boost::python::object))&DeviceProxy::command_inout_asynch_cb,
 		   "Execute an asynchronous command on a device which takes an input argument.\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - argin : python object containing the input argument\n"
		   "        - callback : python object containing the callback object\n"
 		   "Return     : None\n"
           	   "Example :\n"
           	   "        cb = Callback\n"
		   "        dev.command_inout_asynch(""DevString"",""Hello"",cb)\n"
		   )

	.def("command_inout_asynch", (long (DeviceProxy::*) (std::string,int))&DeviceProxy::command_inout_asynch,
 		   "Execute an asynchronous command on a device which takes no input argument.\n"
 		   "The last argument is a fire and forget argument. The zero default value means\n"
 		   "the caller is interested in getting the server reply. If the value is not zero\n"
 		   "then the caller does not care at all about the server answer\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - forget : fire and forget flag\n"
 		   "Return     : A long integer giving this request identifier\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"",0)\n"
		   )

	.def("command_inout_asynch", (long (DeviceProxy::*) (std::string,boost::python::object,int))&DeviceProxy::command_inout_asynch,
 		   "Execute an asynchronous command on a device which takes an input argument.\n"
 		   "The last argument is a fire and forget argument. The zero default value means\n"
 		   "the caller is interested in getting the server reply. If the value is not zero\n"
 		   "then the caller does not care at all about the server answer\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - argin : python object containing the input argument\n"
		   "        - forget : fire and forget flag\n"
 		   "Return     : A long integer giving this request identifier\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"",0)\n"
		   )
		   
	.def("command_inout_asynch", (long (DeviceProxy::*) (std::string))&DeviceProxy::command_inout_asynch,
 		   "Execute an asynchronous command on a device which takes no input argument.\n"
		   "Parameters :\n"
		   "        - name : command name\n"
 		   "Return     : A long integer giving this request identifier\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"")\n"
		   )

	.def("command_inout_asynch", (long (DeviceProxy::*) (std::string,boost::python::object))&DeviceProxy::command_inout_asynch,
 		   "Execute an asynchronous command on a device which takes an input argument.\n"
		   "Parameters :\n"
		   "        - name : command name\n"
		   "        - argin : python object containing the input argument\n"
 		   "Return     : A long integer giving this request identifier\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"")\n"
		   )

	.def("command_inout_reply", (boost::python::object (DeviceProxy::*) (long))&DeviceProxy::command_inout_reply,
 		   "Check if the answer of an asynchronous command_inout is arrived.\n"
 		   "If the  reply is an exception, it is rethrown by this call. An exception is also\n"
 		   "thrown in case of the reply is not yet arrived\n"
		   "Parameters :\n"
		   "        - id : asynchronous call identifier\n"
 		   "Return     : reply as a python object (in case it is arrived and valid\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"",0)\n"
           	   "        str_res = dev.command_inout_reply(r_id)\n"
		   "        print str_res\n"
		   )

	.def("command_inout_reply", (boost::python::object (DeviceProxy::*) (long,long))&DeviceProxy::command_inout_reply,
 		   "Check if the answer of an asynchronous command_inout is arrived.\n"
 		   "If the  reply is an exception, it is rethrown by this call. If the reply is not\n"
 		   "yet arrived, the call will wait for the time specified by the second argument\n"
		   "Parameters :\n"
		   "        - id : asynchronous call identifier\n"
		   "        - timeout : time to wait for  the reply to arrive\n"
 		   "Return     : reply as a python object (in case it is arrived and valid\n"
           	   "Example :\n"
		   "        r_id = dev.command_inout_asynch(""Ping"",0,1)\n"
		   "        time.sleep(1)\n"
           	   "        str_res = dev.command_inout_reply(r_id)\n"
		   "        print str_res\n"
		   )

	;

	declare_tangodb();

}


































