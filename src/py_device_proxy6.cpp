static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy6.cpp,v 1.8 2008/05/02 13:10:45 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy6.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a second subset of DeviceProxy methods
//
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.8 $
//
// $Log: py_device_proxy6.cpp,v $
// Revision 1.8  2008/05/02 13:10:45  tiagocoutinho
// added support for stateless event subscription
//
// Revision 1.7  2008/01/16 11:12:11  tiagocoutinho
// Added pydoc
//
// Revision 1.6  2007/06/13 07:22:38  taurel
// Server changes:
// - Display traceback in case of exception thrown by the delete_device method
// - Enable dynamic attributes in Python server
// - Better detection for DS used withn a debugger
// Client changes
// - Receiving an empty Spectrum/Image attribute do not throw exception any more. It simply return an empty list
// - Print traceback in case of exception thrown by the event callback
// - Better thread protection in case of unsubscribe_event while an event in waiting for the GIL
// - Add read_attribute_as_str() metho dto the DeviceProxy and AttributeProxy class
// - Better management of Python GIL in case of multi-threaded client
// (GIL released when calling C++ tango)
//
// Revision 1.5  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.4  2007/03/12 16:54:20  tiagocoutinho
// -added missing AttributeInfoEx API. Fix some bugs
//
// Revision 1.3  2005/03/24 13:20:20  taurel
// - Added Vincenzo changes for the Tango V5 port
// - Fix bug on Linux in the client_request function (default  argument)
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

int (DeviceProxy::*subscribe_event1) (std::string, Tango::EventType, boost::python::object, boost::python::list) = &DeviceProxy::subscribe_event;
int (DeviceProxy::*subscribe_event2) (std::string, Tango::EventType, boost::python::object, boost::python::list, bool) = &DeviceProxy::subscribe_event;

void device_proxy_asynch(boost::python::class_<DeviceProxy> &x);

void second_of_device_proxy(boost::python::class_<DeviceProxy> &devProx)
{
    using namespace boost::python;
    
	devProx

        .def("attribute_query", &DeviceProxy::attribute_query,
		   "Query the device for information about a single attribute.\n"
		   "Parameters : \n"
		   "        - attribute : attribute name\n"
 		   "Return     : AttributeInfoEx object\n"
           "Example :\n"
           "        attr_info = dev.attribute_query(""short_scalar"")\n"
		   "        print attr_info.name\n"
		   "        print attr_info.writable\n"
		   "        print attr_info.data_format\n"
		   "        print attr_info.data_type\n"
		   "        print attr_info.max_dim_x\n"
		   "        print attr_info.max_dim_y\n"
		   "        print attr_info.description\n"
		   "        print attr_info.label\n"
		   "        print attr_info.unit\n"
		   "        print attr_info.standard_unit\n"
		   "        print attr_info.display_unit\n"
		   "        print attr_info.format\n"
		   "        print attr_info.min_value\n"
		   "        print attr_info.max_value\n"
		   "        print attr_info.min_alarm\n"
		   "        print attr_info.max_alarm\n"
		   "        print attr_info.writable_attr_name\n"
		   "        print attr_info.extensions\n"
		   "        print attr_info.disp_level\n"
		   "        print attr_info.alarms\n"
		   "        print attr_info.events\n"
		   "        print attr_info.sys_extensions\n"
		   "see AttributeInfo documentation string form more detail"
		   )

        .def("attribute_list_query", &DeviceProxy::attribute_list_query,
 		   "Query the device for information on all attributes.\n"
		   "Parameters : None\n"
 		   "Return     : list of AttributeInfo types"
		   )

        .def("attribute_list_query_ex", &DeviceProxy::attribute_list_query_ex,
 		   "Query the device for information on all attributes.\n"
		   "Parameters : None\n"
 		   "Return     : list of AttributeInfo types"
		   )

        .def("get_attribute_list", &DeviceProxy::get_attribute_list,
 		   "Return the names of all attributes implemented for this device.\n"
		   "Parameters : None\n"
 		   "Return     : list of strings"
		   )

        .def("get_attribute_config",
              (boost::python::object (DeviceProxy::*) (std::string))&DeviceProxy::get_attribute_config,
 		   "Same effect as calling attribute_query() or attribute_list_query().\n"
 		   "depending on the argument passed to the method : a string name or a.\n"
 		   "list of names.\n"
		   )

        .def("get_attribute_config",
              (boost::python::list (DeviceProxy::*) (boost::python::list))&DeviceProxy::get_attribute_config) 

        .def("get_attribute_config_ex",
              (boost::python::list (DeviceProxy::*) (boost::python::list))&DeviceProxy::get_attribute_config_ex) 

		.def("set_attribute_config", &DeviceProxy::set_attribute_config,
 		   "Change the attribute configuration for the specified attributes.\n"
		   "Parameters : list of AttributeInfo types\n"
 		   "Return     : None"
		   )
		   
		.def("set_attribute_config_ex", &DeviceProxy::set_attribute_config_ex,
		   "Change the attribute configuration for the specified attributes.\n"
		   "Parameters : \n"
		   "        - attr_info_list : list of AttributeInfoEx types\n"
		   "Return     : None"
		   )
		   	
		.def("subscribe_event", subscribe_event1,
		   "Subscribes to an event generated by the device.\n"
		   "Parameters : \n"
		   "        - attr_name: a string containing the attribute name to subscribe to\n"
		   "        - event: a value of EventType enum\n"
		   "        - call: the callback object that implements the push_event\n"
		   "        - filters: a list of strings containing the filters\n"
		   "Return     : the id of the event"
		   )
		   
		.def("subscribe_event", subscribe_event2,
			"Subscribes to an event generated by the device.\n"
			"When the stateless flag is set to False, an exception will be "
			"thrown when the event subscription encounters a problem.\n"
			"With the stateless flag set to True, the event subscription will "
			"always succeed, even if the corresponding device server is not "
			"running. The keep alive thread will try every 10 seconds to "
			"subscribe for the specified event. At every subscription retry, a "
			"callback is executed which contains the corresponding exception.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Parameters : \n"
			"        - attr_name: a string containing the attribute name to "
			"subscribe to\n"
			"        - event: a value of EventType enum\n"
			"        - call: the callback object that implements the "
			"push_event\n"
			"        - filters: a list of strings containing the filters\n"
			"        - stateless: stateless flag. See text above for detailed "
			"description\n"
			"Return     : the id of the event"
		)
		
		.def("unsubscribe_event", &DeviceProxy::unsubscribe_event,
		   "Unsubscribes from a given event\n"
		   "Parameters : \n"
		   "        - event_id : the id of the event\n"
 		   "Return     : None"
	   )

        .def("read_attribute", &DeviceProxy::read_attribute,
 		   "Read a single attribute.\n"
		   "Parameters : \n"
		   "        - name : attribute name\n"
 		   "Return     : AttributeValue type\n"
           "Example :\n"
           "        attr_val = dev.read_attribute(""short_scalar"")\n"
		   "        print attr_val.value\n"
		   "        print attr_val.time\n"
		   "        print attr_val.quality\n"
		   "        print attr_val.name\n"
		   "        print attr_val.dim_x\n"
		   "        print attr_val.dim_y\n"
		   "see AttributeValue documentation string form more detail"
		   )

        .def("read_attributes", &DeviceProxy::read_attributes,
 		   "Read the list of specified attributes.\n"
		   "Parameters : \n"
		   "        - name : list of attribute names\n"
 		   "Return     : list of AttributeValue types"
		   )

        .def("read_attribute_as_str", &DeviceProxy::read_attribute_as_str,
 		   "Read a single attribute with its value returned as a Python string.\n"
		   "Parameters : \n"
		   "        - name : attribute name\n"
 		   "Return     : AttributeValue type\n"
           "Example :\n"
           "        attr_val = dev.read_attribute(""short_scalar"")\n"
		   "        print attr_val.value (Allways a Python string)\n"
		   "        print attr_val.time\n"
		   "        print attr_val.quality\n"
		   "        print attr_val.name\n"
		   "        print attr_val.dim_x\n"
		   "        print attr_val.dim_y\n"
		   "see AttributeValue documentation string form more detail"
		   )
		   
        .def("read_attributes_as_str", &DeviceProxy::read_attributes_as_str,
 		   "Read the list of specified attributes with attribute values returnes as string.\n"
		   "Parameters : \n"
		   "        - name : list of attribute names\n"
 		   "Return     : list of AttributeValue types"
		   )
		   
        .def("write_attribute", &DeviceProxy::write_attribute,
 		   "Write the specified attribute.\n"
		   "Parameters : \n"
		   "        - attr_val : AttributeValue type\n"
 		   "Return     : None\n"
           "Example :\n"
           "        attr_val = AttributeValue()\n"
		   "        attr_val.name = ""short_scalar""\n"
		   "        attr_val.dim_x = 1\n"
		   "        attr_val.dim_y = 0\n"
		   "        dev.write_attribute(attr_val)"
		   )

		.def("write_attributes", &DeviceProxy::write_attributes,
 		   "Write the specified list of attributes.\n"
		   "Parameters : \n"
		   "        - attr_list : list of AttributeValue types\n"
 		   "Return     : None"
		   )

        .def("attribute_history", &DeviceProxy::attribute_history,
 		   "Retrive attribute history from the command polling buffer.\n"
		   "Parameters :\n"
		   "        - name : attribute name\n"
		   "        - depth : integer representing the wanted history depth\n"
 		   "Return     : a list of DeviceAttributeHistory types"
           "Example :\n"
           "        attr_hist = dev.attribute_history(""short_scalar"",3)\n"
		   "        for dev_hist in attr_hist : print attr_hist\n"
		   "see DeviceAttributeHistory documentation string form more detail"
		   )

        .def("is_command_polled", &DeviceProxy::is_command_polled,
 		   "True if the command is polled.\n"
		   "Parameters :\n"
		   "        - cmd_name : command name\n"
 		   "Return     : boolean value"
			)

		.def("is_attribute_polled", &DeviceProxy::is_attribute_polled,
 		   "True if the attribute is polled.\n"
		   "Parameters :\n"
		   "        - attr_name : attribute name\n"
 		   "Return     : boolean value"
			)

        .def("get_command_poll_period", &DeviceProxy::get_command_poll_period,
 		   "Return the command polling period.\n"
		   "Parameters :\n"
		   "        - cmd_name : command name\n"
 		   "Return     : polling period in milliseconds"
			)

        .def("get_attribute_poll_period", &DeviceProxy::get_attribute_poll_period,
 		   "Return the attribute polling period.\n"
		   "Parameters :\n"
		   "        - attr_name : attribute name\n"
 		   "Return     : polling period in milliseconds"
			)

        .def("polling_status", &DeviceProxy::polling_status,
 		   "Return the device polling status.\n"
		   "Parameters :None\n"
 		   "Return     : list of strings, with one string for each\n"
		   "             polled command/attribute\n"
		   "Each string is multi-line string with\n"
		   "        - attribute/command name\n"
		   "        - attribute/command polling period in milliseconds\n"
		   "        - attribute/command polling ring buffer\n"
		   "        - time needed for last attribute/command execution in milliseconds\n"
		   "        - time since data in the ring buffer has not been updated\n"
		   "        - delta time between the last records in the ring buffer\n"
		   "        - exception parameters in case of the last execution failed"
			)

        .def("poll_command", &DeviceProxy::poll_command,
 		   "Add a command to the list of polled commands.\n"
		   "Parameters :\n"
		   "        - cmd_name : command name\n"
		   "        - period : polling period in milliseconds\n"
 		   "Return     : None"
			)

        .def("poll_attribute", &DeviceProxy::poll_attribute,
 		   "Add an attribute to the list of polled attributes.\n"
		   "Parameters :\n"
		   "        - attr_name : attribute name\n"
		   "        - period : polling period in milliseconds\n"
 		   "Return     : None"
			)

        .def("stop_poll_command", &DeviceProxy::stop_poll_command,
 		   "Remove a command from the list of polled commands.\n"
		   "Parameters :\n"
		   "        - cmd_name : command name\n"
 		   "Return     : None"
			)

        .def("stop_poll_attribute", &DeviceProxy::stop_poll_attribute,
 		   "Remove an attribute from the list of polled attributes.\n"
		   "Parameters :\n"
		   "        - attr_name : attribute name\n"
 		   "Return     : None"
			)

        .def("get_property", &DeviceProxy::get_property,
 		   "Get a list of properties for a device.\n"
		   "Parameters :\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_property", &DeviceProxy::put_property,
 		   "Put a list of properties for a device.\n"
		   "Parameters :\n"
		   "        - props : a dictionary which keys are the property names\n"
		   "                  the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_property", &DeviceProxy::delete_property,
 		   "Delete a list of properties for a device.\n"
		   "Parameters :\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

	;

	device_proxy_asynch(devProx);

}
