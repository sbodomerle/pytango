static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy4.cpp,v 1.5 2008/02/20 15:48:36 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy4.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This introduces DeviceProxy class and defines
//			the class constructor
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.5 $
//
// $Log: py_device_proxy4.cpp,v $
// Revision 1.5  2008/02/20 15:48:36  tiagocoutinho
// added documentation
//
// Revision 1.4  2007/03/07 10:50:55  tiagocoutinho
// - Support for AttributeInfoEx
// - Support for attribute configuration events
//
// Revision 1.3  2005/03/24 13:20:20  taurel
// - Added Vincenzo changes for the Tango V5 port
// - Fix bug on Linux in the client_request function (default  argument)
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


void first_of_device_proxy(boost::python::class_<DeviceProxy> &x);
boost::python::object client_request(ClientRequestHandler &crh, 
					boost::python::object pyData=boost::python::object())
{
	return crh.call(pyData);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(client_request_over,client_request,1,2);

void declare_device_proxy()
{
    using namespace boost::python;

	 class_<ClientRequestHandler>("ClientRequestHandler",
		   init<DeviceProxy&,std::string>())
		   .def("__call__",client_request,client_request_over())
//		   .def("__call__",(boost::python::object (*) (ClientRequestHandler &))&client_request)
//		   .def("__call__",(boost::python::object (*) (ClientRequestHandler &,boost::python::object))&client_request)

     ;

     class_<CmdDoneEvent>("CmdDoneEvent",
		 "Structure members,\n"
		 " - device : Device object on which command was invoked\n"
		 " - argout : command result\n"
		 " - cmd_name : invoked command on the device,\n"
		 " - err : true if errors occured while invoking  the command,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if err is false"
         )
        .def_readonly("device", &CmdDoneEvent::device)
        .def_readonly("argout", &CmdDoneEvent::argout)
        .def_readonly("cmd_name", &CmdDoneEvent::cmd_name)
        .def_readonly("err", &CmdDoneEvent::err)
        .def_readonly("errors", &CmdDoneEvent::errors)
	;

     class_<AttrReadEvent>("AttrReadEvent",
		 "Structure members,\n"
		 " - device : Device object of which attributes are read\n"
		 " - argout : list of attribute read results\n"
		 " - attr_names : list of attribute names,\n"
		 " - err : true if errors occured while reading the attributes,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if err is false"
         )
        .def_readonly("device", &AttrReadEvent::device)
        .def_readonly("argout", &AttrReadEvent::argout)
        .def_readonly("attr_names", &AttrReadEvent::attr_names)
        .def_readonly("err", &AttrReadEvent::err)
        .def_readonly("errors", &AttrReadEvent::errors)
	;

     class_<AttrWrittenEvent>("AttrWrittenEvent",
		 "Structure members,\n"
		 " - device : Device object of which attributes are written\n"
		 " - argout : list of attribute read results\n"
		 " - attr_names : list of attribute names,\n"
		 " - err : true if errors occured while reading the attributes,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if err is false"
         )
        .def_readonly("device", &AttrWrittenEvent::device)
        .def_readonly("argout", &AttrWrittenEvent::argout)
        .def_readonly("attr_names", &AttrWrittenEvent::attr_names)
        .def_readonly("err", &AttrWrittenEvent::err)
        .def_readonly("errors", &AttrWrittenEvent::errors)
	;
	
     /* VF 21/03/2005*/
     class_<EventData>("EventData",
		 "Structure members,\n"
		 " - device : Device object for which the event was generated\n"
		 " - event : event type (PyTango.EventType)"
		 " - attr_value : attribute value (PyTango.AttributeValue) or None if err is True"
		 " - attr_name : attribute name (string)"
		 " - err : True is an error event or False otherwise"
  		 " - errors : list of errors that occurred (see DevError type)\n"
   		 "            empty if err is false"
         )
        .def_readonly("device", &EventData::device)
        .def_readonly("event", &EventData::event)
        .def_readonly("attr_value", &EventData::attr_value)
        .def_readonly("attr_name", &EventData::attr_name)
        .def_readonly("err", &EventData::err)
        .def_readonly("errors", &EventData::errors)
	;
     /* */

     class_<AttrConfEventData>("AttrConfEventData",
		 "Structure members,\n"
		 " - device : Device object which contains the attribute whose configuration changed\n"
		 " - event : The name of the event type (normally 'attr_conf')\n"
    	 " - attr_conf : attribute configuration data (see AttributeInfoEx type),\n"    		 
		 " - attr_name : the attribute name,\n"
		 " - err : true if an event error occured,\n"
		 " - errors : list of errors that occurred (see DevError type)\n"
		 "            empty if err is false"
         )
        .def_readonly("device", &AttrConfEventData::device)
        .def_readonly("event", &AttrConfEventData::event)
        .def_readonly("attr_conf", &AttrConfEventData::attr_conf)
        .def_readonly("attr_name", &AttrConfEventData::attr_name)
        .def_readonly("err", &AttrConfEventData::err)
        .def_readonly("errors", &AttrConfEventData::errors)
	;
	
     class_<DeviceProxy> devProx("DeviceProxy",
           "DeviceProxy is the high level Tango object which provides the client\n"
           "with an easy-to-use interface to TANGO devices.\n"
           "DeviceProxy provides interfaces to all TANGO Device interfaces.\n"
           "The DeviceProxy manages timouts, stateless connections and reconnection\n"
           "if the device server is restarted.\n"
           "To create a DeviceProxy, a Tango Device name must be set in the object\n"
           "constructor.\n"
           "Example :\n"
           "          dev = DeviceProxy(""tango/tangotest/1"")",
		   init<std::string>()
		   )
     ;

	first_of_device_proxy(devProx);

}


































