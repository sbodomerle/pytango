static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_group1.cpp,v 1.2 2007/04/02 07:41:59 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_group1.cpp
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
// $Log: py_device_group1.cpp,v $
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


void first_of_device_group(boost::python::class_<Group> &x);

void declare_tangogroup_base()
{
    using namespace boost::python;

     class_<GroupReply> ("GroupReply",
           "class containing reply to command or attribute read/write executed on a group"
//		   ,init<std::string,std::string,bool>()
		   )

        .def("get_data", &GroupReply::get_data,
		   "A method which returns the reply associated with the corespnding device.\n"
		   "Parameters : None\n"
		   "Return     : result of command execution as a python object"
		   )

        .def("has_failed", &GroupReply::has_failed,
		   "Tells if command has succeeded.\n"
		   "Parameters : None\n"
		   "Return     : true if command failed"
		   )

        .def("dev_name", &GroupReply::dev_name,
		   "Gives the name of the device on which command was executed.\n"
		   "Parameters : None\n"
		   "Return     : string"
		   )

        .def("obj_name", &GroupReply::obj_name,
		   "Gives the name of the command or the attribute\n"
		   "read/write that was executed.\n"
		   "Parameters : None\n"
		   "Return     : string"
		   )

;

     class_<Group> devGroup("Group",
           "Instanciate an emty group\n"
           "The group name allows retrieving a sub-group in the hierarchy",
 		   init<std::string>()
		   )

;


	first_of_device_group(devGroup);

}
