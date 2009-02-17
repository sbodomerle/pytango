static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_group2.cpp,v 1.2 2007/04/02 07:41:59 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_group2.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This introduces Group related classes 
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.2 $
//
// $Log: py_device_group2.cpp,v $
// Revision 1.2  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.1  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
//
//
// copyleft :     Synchrotron SOLEIL
//			L'Orme des Merisiers, Saint-Aubin BP 48
//			91192 GIF SUR YVETTE Cedex
//
//-=============================================================================

#include <device_group.h>
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_remove_overloads,Group::remove,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_contains_overloads,Group::contains,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_getsize_overloads,Group::get_size,0,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_getdevicelist_overloads,Group::get_device_list,0,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_ping_overloads,Group::ping,0,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_command_reply_overloads,Group::command_inout_reply,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattr_overloads,Group::read_attribute,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattrs_overloads,Group::read_attributes,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattr_asynch_overloads,Group::read_attribute_asynch,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattrs_asynch_overloads,Group::read_attributes_asynch,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattr_reply_overloads,Group::read_attribute_reply,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_readattrs_reply_overloads,Group::read_attributes_reply,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_writeattr1_overloads,Group::write_attribute,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_writeattr2_overloads,Group::write_attribute,2,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_writeattr1_asynch_overloads,Group::write_attribute_asynch,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_writeattr2_asynch_overloads,Group::write_attribute_asynch,2,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Group_writeattr_reply_overloads,Group::write_attribute_reply,1,2)

void first_of_device_group(boost::python::class_<Group> &devGroup)
{
    using namespace boost::python;
    
     devGroup

        .def("get_name",
              (std::string (Group::*) (void))&Group::get_name, 
		   "Obtains the user name of the group.\n"
		   "Parameters : None\n"
		   "Return     : the group name"
		   )

        .def("get_fully_qualified_name",
              (std::string (Group::*) (void))&Group::get_fully_qualified_name, 
		   "Obtains the complete (dot separated) name of the group.\n"
		   "Parameters : None\n"
		   "Return     : the full group name"
		   )

        .def("enable",
              (void (Group::*) (const std::string&, boost::python::object))&Group::enable, 
		   "enables the group.\n"
		   "Parameters : \n"
		   "        - device name : the name of the device element"
		   "        - PyData : if needed, python object containing a true or false value\n"
		   "Return     : None"
		   )

        .def("disable",
              (void (Group::*) (const std::string&, boost::python::object))&Group::disable, 
		   "disables the group.\n"
		   "Parameters : \n"
		   "        - device name : the name of the device element"
		   "        - PyData : if needed, python object containing a true or false value\n"
		   "Return     : None"
		   )

/*
 * NOTE: This method is not being exposed because the C++ counterpart doesn't seem to make sence since
 *       there is a is_enabled() but no enable(void) or disable(void) methods available
        .def("is_enabled",
              (bool (Group::*) (void))&Group::is_enabled, 
		   "Determines if the group is enabled.\n"
		   "Parameters : None\n"
		   "Return     : True if the group is enabled or False otherwise"
		   )
*/

        .def("add",
              (void (Group::*) (boost::python::object))&Group::add, 
		   "Attaches a sub-group\n"
		   "Parameters : group object to attach\n"
		   "Return     : None"
		   )

		.def("add",
              (void (Group::*) (boost::python::list))&Group::add, 
		   "Attaches any device which name matches one of the specified patterns.\n"
		   "Parameters : patterns : a list of string patterns\n"
		   "Return     : None"
		   )

        .def("add",
              (void (Group::*) (const std::string&))&Group::add, 
		   "Attaches any device which name matches the specified pattern\n"
		   "Parameters : string pattern\n"
		   "Return     : None"
		   )

		.def("remove", &Group::remove,
               Group_remove_overloads(args("pattern","PyData"),
 		   "Removes any group or device which name matches the specified pattern.\n"
 		   "If the second arg is set to true (the default), the request is forwarded to sub-groups.\n"
		   "Parameters :\n"
		   "        - pattern : string pattern\n"
		   "        - PyData : if needed, python object containing a true or false value\n"
 		   "Return     : None\n"
		   ))

        .def("remove_all",&Group::remove_all, 
		   "Removes all elements in a group"
		   "Parameters : None\n"
		   "Return     : None"
		   )

		.def("contains", &Group::contains,
               Group_contains_overloads(args("pattern","PyData"),
 		   "Returns true if the hierarchy contains groups and.or devices\n"
 		   "which name matches the specified pattern.\n"
 		   "If the second arg is set to true (the default), the request is forwarded to sub-groups.\n"
		   "Parameters :\n"
		   "        - pattern : string pattern\n"
		   "        - PyData : if needed, python object containing a true or false value\n"
 		   "Return     : true or false\n"
		   ))

		.def("get_size", &Group::get_size,
               Group_getsize_overloads(args("PyData"),
 		   "Returns the number of the devices in the hierarchy\n"
 		   "If the arg is set to true (the default), the request is forwarded to sub-groups.\n"
		   "Parameters :\n"
		   "        - PyData : if needed, python object containing a true or false value\n"
 		   "Return     : long value\n"
		   ))

		.def("get_device_list", &Group::get_device_list,
               Group_getdevicelist_overloads(args("PyData"),
 		   "Returns the list of devices currently in the hierarchy\n"
 		   "If the arg is set to true (the default), the request is forwarded to sub-groups.\n"
		   "Parameters :\n"
		   "        - PyData : if needed, python object containing a true or false value\n"
 		   "Return     : list of DeviceProxy string names\n"
		   ))

        .def("get_device",
              (boost::python::object (Group::*) (const std::string&))&Group::get_device, 
		   "Returns a reference to the specified device or NONE if there is\n"
		   "no device by that name in the group\n"
		   "Parameters : device name\n"
		   "Return     : DeviceProxy object or NONE"
		   )

        .def("get_device",
              (boost::python::object (Group::*) (long idx))&Group::get_device, 
		   "Returns a reference to the idx-th device in the hierarchy or NONE if \n"
		   "the hierarchy contains less than idx devices. The request is forwarded\n"
		   "Parameters : device index in the hierarchy\n"
		   "Return     : DeviceProxy object or NONE"
		   )

		.def("ping", &Group::ping,
               Group_ping_overloads(args("PyData"),
 		   "Ping all devices in a group\n"
 		   "Returns true if all devices in the group are alive, false otherwise.\n"
 		   "If the argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - PyData : if needed, python object containing a true or false value\n"
 		   "Return     : true or false\n"
		   ))

        .def("command_inout",
              (boost::python::list (Group::*) (const std::string&,boost::python::object))&Group::command_inout,
		   "Executes a Tango command on a group. This method is synchronous and does not "
		   "return until replies are obtained or timeouts occured.\n"
 		   "If the second argument (fwd) is set to true, the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - cmd_name : name of the command\n"
		   "        - fwd : python object containing a true or false value. For this call there is no default value!\n"
 		   "Return     : list of GroupReply objects\n"
		   )

        .def("command_inout",
              (boost::python::list (Group::*) (const std::string&,boost::python::object,boost::python::object))&Group::command_inout,
		   "Executes a Tango command on a group. This method is synchronous and does not "
		   "return until replies are obtained or timeouts occured.\n"
 		   "If the last argument (fwd) is set to true, the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - cmd_name : name of the command\n"
		   "        - argin : python object containing the input argument\n"
		   "        - fwd : python object containing a true or false value. For this call there is no default value!\n"
 		   "Return     : list of GroupReply objects\n"
		   )

        .def("command_inout_asynch",
              (long (Group::*) (const std::string&,boost::python::object,boost::python::object))&Group::command_inout_asynch,
		   "Executes a Tango command on a group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "command_inout_reply() method to obtain the results.\n"
 		   "The third argument (fgt) is a fire and forget flag. If set to true,\n"
 		   "that means that the caller does not care about the result.\n"
 		   "If the last (fwd) is set to true, the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - cmd_name : name of the command\n"
		   "        - fgt : python object containing a true or false value. For this call there is no default value!\n"
		   "        - fwd : python object containing a true or false value. For this call there is no default value!\n"
 		   "Return     : long integer identifying the request\n"
		   )//)

        .def("command_inout_asynch",
              (long (Group::*) (const std::string&,boost::python::object,boost::python::object,boost::python::object))&Group::command_inout_asynch,
		   "Executes a Tango command on a group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "command_inout_reply() method to obtain the results.\n"
 		   "The third argument (fgt) is a fire and forget flag. If set to true,\n"
 		   "that means that the caller does not care about the result.\n"
 		   "If the last (fwd) is set to true, the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - c : name of the command\n"
		   "        - argin : python object containing the input argument\n"
		   "        - fgt : python object containing a true or false value. For this call there is no default value!\n"
		   "        - fwd : python object containing a true or false value. For this call there is no default value!\n"
 		   "Return     : long integer identifying the request\n"
		   )//)

        .def("command_inout_asynch",
              (long (Group::*) (const std::string&,boost::python::list,boost::python::object,boost::python::object))&Group::command_inout_asynch,
		   "Executes a Tango command on a group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "command_inout_reply() method to obtain the results.\n"
 		   "The second argument (fgt) is a fire and forget flag. If set to true,\n"
 		   "that means that the caller does not care about the result.\n"
 		   "The third (fwd) is set to true, the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - c : name of the command\n"
		   "        - argins : python list containing the input arguments\n"
		   "        - fgt : python object containing a true or false value. For this call there is no default value!\n"
		   "        - fwd : python object containing a true or false value. For this call there is no default value!\n"
 		   "Return     : long integer identifying the request\n"
		   )//)

		.def("command_inout_reply", &Group::command_inout_reply,
               Group_command_reply_overloads(args("req_id","timeout_ms"),
 		   "Returns the result of an asynchronous command.\n"
		   "Parameters :\n"
		   "        - req_id : request identifier\n"
		   "        - timeout_ms : if timeout_ms is set to 0 ,method waits indefinitely\n"
 		   "Return     : list of GroupReply objects\n"
		   ))

        .def("read_attribute",
              (boost::python::list (Group::*) (const std::string&,boost::python::object))&Group::read_attribute,
              Group_readattr_overloads(args("attr_name","fwd"),
		   "Reads an attribute on each device in the group. This method is synchronous and does not"
		   "return until replies are obtained or timeouts occured."
 		   "If the second argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_name : name of the attribute\n"
		   "        - fwd : python object containing a true or false value\n"
 		   "Return     : list of GroupReply objects\n"
		   ))
		   
        .def("read_attributes",
              (boost::python::list (Group::*) (boost::python::list,boost::python::object))&Group::read_attributes, 
              Group_readattrs_overloads(args("attr_list","fwd"),
		   "Reads several attributes on each device in the group. This method is synchronous and does not"
		   "return until replies are obtained or timeouts occured."
 		   "If the second argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_list : a list of attribute names\n"
		   "        - fwd : python object containing a true or false value\n"
 		   "Return     : list of GroupReply objects\n"
		   ))
		   
        .def("read_attribute_asynch",
              (long (Group::*) (const std::string&,boost::python::object))&Group::read_attribute_asynch,
              Group_readattr_asynch_overloads(args("attr_name","fwd"), 
		   "Reads an attribute on each device in the group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "read_attribute_reply() method to obtain the results.\n"
 		   "If the second argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_name : name of the attribute\n"
		   "        - fwd : python object containing a true or false value\n"
		   "Return     : long integer identifying the request\n"
		   ))

        .def("read_attributes_asynch",
              (long (Group::*) (boost::python::list,boost::python::object))&Group::read_attributes_asynch,
              Group_readattrs_asynch_overloads(args("attr_list","fwd"), 
		   "Reads several attributes on each device in the group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "read_attribute_reply() method to obtain the results.\n"
 		   "If the second argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_list : a list of attribute names\n"
		   "        - fwd : python object containing a true or false value\n"
		   "Return     : long integer identifying the request\n"
		   ))

		.def("read_attribute_reply", &Group::read_attribute_reply,
               Group_readattr_reply_overloads(args("req_id","timeout_ms"),
 		   "Returns the result of an asynchronous read attribute request.\n"
		   "Parameters :\n"
		   "        - req_id : request identifier\n"
		   "        - timeout_ms : if timeout_ms is set to 0 ,method waits indefinitely\n"
 		   "Return     : list of GroupReply objects\n"
		   ))

		.def("read_attributes_reply", &Group::read_attributes_reply,
               Group_readattrs_reply_overloads(args("req_id","timeout_ms"),
 		   "Returns the result of an asynchronous read attributes request.\n"
		   "Parameters :\n"
		   "        - req_id : request identifier\n"
		   "        - timeout_ms : if timeout_ms is set to 0 ,method waits indefinitely\n"
 		   "Return     : list of GroupReply objects\n"
		   ))

        .def("write_attribute",
              (boost::python::list (Group::*) (AttributeValue,boost::python::object))&Group::write_attribute,
              Group_writeattr1_overloads(args("attr_value","fwd"), 
		   "Writes an attribute on each device in the group. This method is synchronous and does not"
		   "return until replies are obtained or timeouts occured."
 		   "If the last argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_value : attribute information (AttributeValue)\n"
		   "        - fwd : python object containing a true or false value\n"
 		   "Return     : list of GroupReply objects\n"
		   ))

        .def("write_attribute",
              (boost::python::list (Group::*) (const std::string&,boost::python::object,boost::python::object))&Group::write_attribute,
              Group_writeattr2_overloads(args("name","value","fwd"), 
		   "Writes an attribute on each device in the group. This method is synchronous and does not"
		   "return until replies are obtained or timeouts occured."
 		   "If the last argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - name : attribute name\n"
		   "        - value : attribute value or list of attribute values *\n"
		   "        - fwd : python object containing a true or false value\n"
 		   "Return     : list of GroupReply objects\n"
 		   "*Note: In case a list of values is given, this method supports only non string, SCALAR and SPECTRUM attributes.\n"
		   ))

        .def("write_attribute_asynch",
              (long (Group::*) (AttributeValue,boost::python::object))&Group::write_attribute_asynch,
               Group_writeattr1_asynch_overloads(args("attr_value","fwd"), 
		   "Writes an attribute on each device in the group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "write_attribute_reply() method to obtain the results.\n"
 		   "If the second argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_value : attribute information (AttributeValue)\n"
		   "        - fwd : python object containing a true or false value\n"
		   "Return     : long integer identifying the request\n"
		   ))

        .def("write_attribute_asynch",
              (long (Group::*) (const std::string&,boost::python::object,boost::python::object))&Group::write_attribute_asynch,
              Group_writeattr2_asynch_overloads(args("attr_name","value","fwd"), 
		   "Writes an attribute on each device in the group asynchronously. The method sends the request\n"
		   "to all devices and returns immediately. Pass the returned request id to\n"
		   "write_attribute_reply() method to obtain the results.\n"
 		   "If the last argument (fwd) is set to true (the default), the request\n"
 		   "is also forwarded to subgroups.\n"
		   "Parameters :\n"
		   "        - attr_name : attribute name\n"
		   "        - value : attribute value or list of attribute values *\n"
		   "        - fwd : python object containing a true or false value\n"
		   "Return     : long integer identifying the request\n"
		   ))

		.def("write_attribute_reply", &Group::write_attribute_reply,
               Group_writeattr_reply_overloads(args("req_id","timeout_ms"),
 		   "Returns the result of an asynchronous write attribute request.\n"
		   "Parameters :\n"
		   "        - req_id : request identifier\n"
		   "        - timeout_ms : if timeout_ms is set to 0 ,method waits indefinitely\n"
 		   "Return     : list of command replies\n"
		   ))

;

}
