static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_tangodb3.cpp,v 1.8 2008/05/06 11:52:27 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_tangodb3.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease  objects into a Python run time
//                This file defines a first subset of Database methods
//
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.8 $
//
// $Log: py_tangodb3.cpp,v $
// Revision 1.8  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.7  2008/02/13 16:30:36  tiagocoutinho
// added get_db_host, get_db_port, get_db_port_num
//
// Revision 1.6  2008/01/23 12:18:53  tiagocoutinho
// - Added missing database API
//
// Revision 1.5  2007/11/12 14:35:24  tiagocoutinho
// - Extended Database API
//
// Revision 1.4  2006/08/11 13:54:24  taurel
// - Added the wrapping of the Database::get_device_property_list() method
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


boost::python::list (Database::*get_host_list1)()				= &Database::get_host_list;
boost::python::list (Database::*get_host_list2)(std::string )	= &Database::get_host_list;
boost::python::list (Database::*get_server_list1)()				= &Database::get_server_list;
boost::python::list (Database::*get_server_list2)(std::string )	= &Database::get_server_list;

void second_of_tangodb(boost::python::class_<Database> &x);

void first_of_tangodb(boost::python::class_<Database> &tangoDB)
{

    using namespace boost::python;


    tangoDB

		.def("get_db_host",&Database::get_db_host, 
		   "Returns the host.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : None\n"
			   "Return     : a string containing the host"
		   )
		   
		.def("get_db_port",&Database::get_db_port, 
		   "Returns the port number.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : None\n"
			   "Return     : a string containing the port number"
		   )
		   
		.def("get_db_port_num",&Database::get_db_port_num, 
		   "Returns the port number (int).\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : None\n"
			   "Return     : the port number"
		   )
		   
		.def("get_info",&Database::get_info, 
		   "Query the database for some general info about the tables.\n"
		   "Parameters : None\n"
 		   "Return     : a multiline string"
		   )

        .def("add_device", &Database::add_device,
		   "Add a device to the database. The device name, server and class\n"
		   "are specified in the DbDevInfo structure\n"
		   "Parameters : DbDevInfo structure\n"
		   "Return     : None\n"
           "Example :\n"
           "        dev_info = DbDevInfo()\n"
		   "        dev_info.name = ""my/own/device""\n"
		   "        dev_info.class = ""MyDevice""\n"
		   "        dev_info.server = ""MyServer/test""\n"
		   "        db.add_device(dev_info)"
		   )

        .def("delete_device", &Database::delete_device,
		   "Delete the device of the specified name from the database\n"
		   "Parameters : Device name\n"
		   "Return     : None\n"
           "Example :\n"
		   "        db.delete_device(""my/own/device"")"
		   )

        .def("import_device", &Database::import_device,
		   "Query the databse for the export info of the specified device\n"
		   "Parameters : Device name\n"
 		   "Return     : DbDevImportInfo object\n"
           "Example :\n"
           "        dev_imp_info = db.import_device(""my/own/device"")\n"
		   "        print dev_imp_info.name\n"
		   "        print dev_imp_info.exported\n"
		   "        print dev_imp_info.ior\n"
		   "        print dev_imp_info.version"
		   )

        .def("export_device", &Database::export_device,
		   "Update the export info for this device in the database\n"
		   "Parameters : DbDevExportInfo structure\n"
		   "Return     : None\n"
           "Example :\n"
           "        dev_export = DbDevExportInfo()\n"
		   "        dev_export.name = ""my/own/device""\n"
		   "        dev_export.ior = ""the real ior""\n"
		   "        dev_export.host = ""the host""\n"
		   "        dev_export.version = ""1.0""\n"
		   "        dev_export.pid = ""....""\n"
		   "        db.export_device(dev_export)"
		   )

        .def("unexport_device", &Database::unexport_device,
		   "Mark the specified device as unexported in the database\n"
		   "Parameters : Device name\n"
		   "Return     : None\n"
           "Example :\n"
		   "        db.unexport_device(""my/own/device"")"
		   )

		.def("add_server", &Database::add_server,
		   "Add a a group of devices to the database.\n"
		   "Parameters : Server name\n"
		   "             List of DbDevInfo structures\n"
		   "Return     : None"
		   )
		
		.def("delete_server", &Database::delete_server,
		   "Delete the device server and its associated devices from database\n"
		   "Parameters : Server name\n"
		   "Return     : None"
		   )
		
		.def("export_server", &Database::add_server,
		   "Export a group of devices to the database.\n"
		   "Parameters : Server name\n"
		   "             List of DbDevExportInfo structures\n"
		   "Return     : None"
		   )
		
		.def("unexport_server", &Database::unexport_server,
		   "Mark all devices exported for this server as unexported\n"
		   "Parameters : Server name\n"
		   "Return     : None"
		   )
		   
		.def("get_services", &Database::get_services,
		   "Query database for specified services.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : Service name\n"
		   "             instance name (can be a wildcard character ('*'))\n"
		   "Return     : list of available services"
		   )

		.def("register_service", &Database::register_service,
		   "Register the specified service wihtin the database.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : Service name\n"
		   "             instance name\n"
		   "             device name\n"
		   "Return     : None"
		   )
		   
		.def("unregister_service", &Database::unregister_service,
		   "Unregister the specified service from the database.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : Service name\n"
		   "             instance name\n"
		   "Return     : None"
		   )
		   
		.def("get_host_list", get_host_list1,
		   "Returns the list of all host names registered in the database.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Return     : List of registered host names"
		   )

		.def("get_host_list", get_host_list2,
		   "Returns the list of all host names registered in the database which"
		   " match the specified wildcard (eg: 'l-c0*').\n"
		   "Parameters : wildcard\n"
		   "Return     : List of registered host names"
		   )
		
		.def("get_server_class_list", &Database::get_server_class_list,
		   "Query the database for a list of classes instancied by the "
		   "specified server. The DServer class exists in all TANGO servers and"
		   " for this reason this class is removed from the returned list.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : server string in format <ServerName>/<instance>\n"
		   "Return     : list of class names instanciated by the specified server"
		   )
		   
		.def("get_server_name_list", &Database::get_server_name_list,
			"Return the list of all server names registered in the database.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return: list of server names"
			)
		   
		.def("get_instance_name_list", &Database::get_instance_name_list,
			"Return the list of all instance names existing in the database "
			"for the specifed server.\n"
			"\nNew in PyTango 3.0.4\n\n"				
			"Parameters: server name\n"
			"Return: list of instance names"
			)
			
		.def("get_server_list", get_server_list1,
			"Return the list of all servers registered in the database.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return: list of servers"
			)
			
		.def("get_server_list", get_server_list2,
			"Return the list of all servers registered in the database which "
			"match the specified wildcard (eg: 'Serial/*').\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Parameters: Server wildcard (eg: 'Serial/*')\n"
			"Return: list of matching servers"
			)
			
		.def("get_host_server_list", &Database::get_host_server_list,
			"Query the database for a list of servers registred on the "
			"specified host (we refer 'host' in the context of tango device, "
			"i.e. following the tango naming convention 'host/family/member')\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Parameters: host name\n"
			"Return: list of matching servers"
			)

		.def("get_server_info", &Database::get_server_info,
			"Query the database for server information.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Parameters: Server name\n"
			"Return: A DbServerInfo with server information"
			)

		.def("put_server_info", &Database::put_server_info,
			"Add/update server information in the database.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Parameters: DbServerInfo structure\n"
			"Return: None"
			)
			
		.def("delete_server_info", &Database::delete_server_info,
			"Delete server information of the specifed server from the database.\n"
			"\nNew in PyTango 3.0.4\n\n"				
			"Parameters: Server name\n"
			"Return: None"
			)

        .def("get_device_name", &Database::get_device_name,
		   "Query the database for a list of devices served by a server for\n"
		   "a given device class\n"
		   "Parameters : Server name\n"
		   "             Device class name\n"
		   "Return     : List of device names"
		   )

        .def("get_alias", &Database::get_alias,
		   "Get the device alias name from its name.\n"
           "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : Device name\n"
		   "Return     : Device alias"
		   )

        .def("get_device_alias", &Database::get_device_alias,
		   "Get the device name from an alias.\n"
		   "Parameters : Device alias\n"
		   "Return     : Device name"
		   )

        .def("get_device_exported", &Database::get_device_exported,
		   "Query the database for a list of exported devices whose names\n"
		   "satisfy the supplied filter (* is wildcard for any character(s))\n"
		   "Parameters : string filter\n"
		   "Return     : List of exported devices"
		   )

        .def("get_device_domain", &Database::get_device_domain,
		   "Query the database for a list of of device domain names which\n"
		   "match the wildcard provided (* is wildcard for any character(s)).\n"
		   "Domain names are case insensitive.\n"
		   "Parameters : string filter\n"
		   "Return     : List of device domain names"
		   )

        .def("get_device_family", &Database::get_device_family,
		   "Query the database for a list of of device family names which\n"
		   "match the wildcard provided (* is wildcard for any character(s)).\n"
		   "Family names are case insensitive.\n"
		   "Parameters : string filter\n"
		   "Return     : List of device family names"
		   )

        .def("get_device_member", &Database::get_device_member,
		   "Query the database for a list of of device member names which\n"
		   "match the wildcard provided (* is wildcard for any character(s)).\n"
		   "member names are case insensitive.\n"
		   "Parameters : string filter\n"
		   "Return     : List of device member names"
		   )

		.def("get_device_class_list", &Database::get_device_class_list,
		   "Query the database for a list of devices and classes served by "
		   "the specified server.\nReturn a list with the following structure: "
		   "[device name,class name,device name,class name,...]\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : server string in format <ServerName>/<instance>\n"
		   "Return     : a list with the following structure: "
		   "[device name,class name,device name,class name,...]"
		   )

		.def("get_class_for_device",&Database::get_class_for_device, 
		   "Return the class of the specified device.\n"
		   "Parameters : string device name\n"
 		   "Return     : a string containning the device class"
		   )

		.def("get_device_exported_for_class", &Database::get_device_exported_for_class,
		   "Query database for list of exported devices for the specified class.\n"
		   "\nNew in PyTango 3.0.4\n\n"
		   "Parameters : string class name\n"
		   "Return     : List of device names"
		   )

		   
        .def("get_device_property_list", &Database::get_device_property_list,
		   "Query the database for a list of of device property names which\n"
		   "match the wildcard provided (* is wildcard for any character(s)).\n"
		   "member names are case insensitive.\n"
		   "Parameters : string device_name\n"
		   "Parameters : string filter\n"
		   "Return     : List of device member names"
		   )

	;


	second_of_tangodb(tangoDB);

}


































