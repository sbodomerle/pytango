static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_tangodb2.cpp,v 1.3 2005/10/31 09:03:01 dusdan Exp $";
//+=============================================================================
//
// file :         py_tangodb2.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This introduces Database class and defines
//			the class constructor
//
// project :      Python Binding
//
// $Author: dusdan $
//
// $Revision: 1.3 $
//
// $Log: py_tangodb2.cpp,v $
// Revision 1.3  2005/10/31 09:03:01  dusdan
// fixed bug with python2.4 and state attribute subscribe_event;
// added new Database object constructor
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


void first_of_tangodb(boost::python::class_<Database> &x);

void declare_tangodb()
{
    using namespace boost::python;

     class_<Database> tangoDB("Database",
           "Database is the high level Tango object which contains\n"
           "the link to to the static database.\n"
           "Database provides methodes for all database commands :\n"
           "get_device_property(), put_device_property(), info(), etc\n"
           "To create a Database, use the default constructor\n"
           "          db = Database()\n"
           "The constructor uses the TANGO_HOST env. variable to\n"
           "determine which instance of the Database to connect to",
		   init<>());
		   tangoDB.def(init<std::string, int>());
	first_of_tangodb(tangoDB);

}


































