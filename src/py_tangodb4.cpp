static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_tangodb4.cpp,v 1.6 2007/05/11 07:32:11 taurel Exp $";
//+=============================================================================
//
// file :         py_tangodb4.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a second subset of Database methods
//
//
// project :      Python Binding
//
// $Author: taurel $
//
// $Revision: 1.6 $
//
// $Log: py_tangodb4.cpp,v $
// Revision 1.6  2007/05/11 07:32:11  taurel
// - Added wrapping of some new methods (Database::put_device_alias and
// delete_device_alias and DeviceClass::device_destroyer)
//
// Revision 1.5  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.4  2007/03/06 09:54:12  tiagocoutinho
// - fixed bug 167390 : Problems with Database methods put(get)_device_property
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

void attribute_proxy();

void second_of_tangodb(boost::python::class_<Database> &tangoDB)
{
    using namespace boost::python;
    
	tangoDB

        .def("get_property", &Database::get_property,
 		   "Query the database for a list of object (i.e non-device).\n"
 		   "properties.\n"
		   "Parameters :\n"
		   "        - string : object name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_property", &Database::put_property,
 		   "Insert or update a list of properties for the specified object.\n"
		   "Parameters :\n"
		   "        - string : object name\n"
		   "        - props : a dictionary which keys are the property names\n"
		   "                  the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_property", &Database::delete_property,
 		   "Delete a list of properties for the specified object.\n"
		   "Parameters :\n"
		   "        - string : object name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

        .def("get_device_property", &Database::get_device_property,
 		   "Query the database for a list of device properties.\n"
		   "Parameters :\n"
		   "        - string : full tango device name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_device_property", &Database::put_device_property,
 		   "Insert or update a list of properties for the specified device.\n"
		   "Parameters :\n"
		   "        - string : full tango device name\n"
		   "        - props : a dictionary which keys are the property names\n"
		   "                  the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_device_property", &Database::delete_device_property,
 		   "Delete a list of properties for the specified device.\n"
		   "Parameters :\n"
		   "        - string : full tango device name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

        .def("get_device_attribute_property", &Database::get_device_attribute_property,
 		   "Query the database for a list of device attribute properties.\n"
		   "Parameters :\n"
		   "        - string :  full tango device name\n"
		   "        - prop_list : list of attribute names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being another\n"
		   "             dictionary which keys are the attribute propery names,\n"
		   "             and the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_device_attribute_property", &Database::put_device_attribute_property,
 		   "Insert or update a list of properties for the specified device attribute.\n"
		   "Parameters :\n"
		   "        - string : full tango device name\n"
		   "        - props : a dictionary which keys are the attribute names\n"
		   "                  the value associated with each key being another\n"
		   "                  dictionary which keys are the attribute property names\n"
		   "                  and the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_device_attribute_property", &Database::delete_device_attribute_property,
 		   "Delete a list of properties for the specified device attribute.\n"
		   "Parameters :\n"
		   "        - string : device attribute name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

        .def("get_class_property", &Database::get_class_property,
 		   "Query the database for a list of device class properties.\n"
		   "Parameters :\n"
		   "        - string : device class name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_class_property", &Database::put_class_property,
 		   "Insert or update a list of properties for the specified device class.\n"
		   "Parameters :\n"
		   "        - string : device class name\n"
		   "        - props : a dictionary which keys are the property names\n"
		   "                  the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_class_property", &Database::delete_class_property,
 		   "Delete a list of properties for the specified device class.\n"
		   "Parameters :\n"
		   "        - string : device class name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

        .def("get_class_attribute_property", &Database::get_class_attribute_property,
 		   "Query the database for a list of device class attribute properties.\n"
		   "Parameters :\n"
		   "        - string : class attribute name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : a dictionary which keys are the property names\n"
		   "             the value associated with each key being a list\n"
		   "             of strings representing the property value"
			)

        .def("put_class_attribute_property", &Database::put_class_attribute_property,
 		   "Insert or update a list of properties for the specified class attribute.\n"
		   "Parameters :\n"
		   "        - string : class attribute name\n"
		   "        - props : a dictionary which keys are the property names\n"
		   "                  the value associated with each key being a list\n"
		   "                  of strings representing the property value\n"
 		   "Return     : None"
		   )

        .def("delete_class_attribute_property", &Database::delete_class_attribute_property,
 		   "Delete a list of properties for the specified class attribute.\n"
		   "Parameters :\n"
		   "        - string : class attribute name\n"
		   "        - prop_list : list of property names\n"
 		   "Return     : None"
		   )

        .def("put_device_alias", &Database::put_device_alias,
 		   "Create a device alias.\n"
		   "Parameters :\n"
		   "        - string : device name\n"
		   "        - string : alias name\n"
 		   "Return     : None"
		   )

        .def("delete_device_alias", &Database::delete_device_alias,
 		   "Delete a device alias.\n"
		   "Parameters :\n"
		   "        - string : alias name\n"
 		   "Return     : None"
		   )
	;
	attribute_proxy();
}


































