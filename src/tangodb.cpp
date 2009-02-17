static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/tangodb.cpp,v 1.12 2008/02/13 16:30:36 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         tangodb.cpp
//
// description :  C++ source for the light Database class
//                intented to be exported to python world.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.12 $
//
// $Log: tangodb.cpp,v $
// Revision 1.12  2008/02/13 16:30:36  tiagocoutinho
// added get_db_host, get_db_port, get_db_port_num
//
// Revision 1.11  2008/01/23 12:18:53  tiagocoutinho
// - Added missing database API
//
// Revision 1.10  2008/01/22 09:32:25  tiagocoutinho
// added to Database API: get_server_info, put_server_info, delete_server_info
//
// Revision 1.9  2007/12/06 16:25:30  taurel
// - Fix bug in the put_class_attribute_property() method
//
// Revision 1.8  2007/11/12 14:35:24  tiagocoutinho
// - Extended Database API
//
// Revision 1.7  2007/05/11 07:32:11  taurel
// - Added wrapping of some new methods (Database::put_device_alias and
// delete_device_alias and DeviceClass::device_destroyer)
//
// Revision 1.6  2006/08/11 13:54:24  taurel
// - Added the wrapping of the Database::get_device_property_list() method
//
// Revision 1.5  2005/10/31 09:03:01  dusdan
// fixed bug with python2.4 and state attribute subscribe_event;
// added new Database object constructor
//
// Revision 1.4  2005/04/08 11:49:23  dusdan
// Vincenzo Forchi`
// - now all tango types _should_ be supported
// - modified the get_scalar_value in templates.cpp: now it tries to get a suitable c++ type from the string representation of the python object (if the boost::python::extract fails)
// - I grouped all the utility functions in a file called conversion_utils.cpp to avoid duplicated code
// - attribute_proxy_utils.cpp and device_proxy_utils.cpp aren't needed anymore
// - added a test_att_proxy.py to test the new object
// - added tango_client.py: a small command line client
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

#include <templates.cpp>

Database::Database()
{
	database = new Tango::Database();
}

Database::Database(std::string host, int port)
{
	database = new Tango::Database(host, port);
}

Database::~Database()
{ 
    delete database; 
}

std::string 
Database::get_db_host()
{
	return database->get_db_host();
}

std::string 
Database::get_db_port()
{
	return database->get_db_port();
}

int 
Database::get_db_port_num()
{
	return database->get_db_port_num();
}

std::string 
Database::get_info()
{ 
  try {
    return database->get_info(); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::add_device(Tango::DbDevInfo dev_info)
{ 
  try { 
    database->add_device(dev_info); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}     


void 
Database::delete_device(std::string name)
{ 
  try { 
    database->delete_device(name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}     


boost::python::object 
Database::import_device(std::string name)
{ 
  try { 
    return (boost::python::object) (database->import_device(name)); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}     


void 
Database::export_device(Tango::DbDevExportInfo export_info)
{
  try { 
    database->export_device(export_info); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::unexport_device(std::string name)
{
  try { 
    database->unexport_device(name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::add_server(std::string name, boost::python::list dev_infos)
{
  try { 
    std::vector<Tango::DbDevInfo> vinfo_array;
    //get_array_value<Tango::DbDevInfo>(vinfo_array,"DbDevInfo list", (boost::python::object)dev_infos);
    get_vector_from_list(vinfo_array, dev_infos);
    database->add_server(name,vinfo_array);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::delete_server(std::string name)
{ 
  try { 
    database->delete_server(name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}     


void 
Database::export_server(std::string name,boost::python::list dev_export_infos)
{
  try { 
    std::vector<Tango::DbDevExportInfo> vexport_info_array;
    //get_array_value<Tango::DbDevExportInfo>(vexport_info_array,"DbDevExportInfo list",(boost::python::object)dev_export_infos);
    get_vector_from_list(vexport_info_array, dev_export_infos);
	database->export_server(vexport_info_array);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::unexport_server(std::string name)
{
  try { 
    database->unexport_server(name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::list 
Database::get_services(string servicename,string instname)
{
	try { 
	    Tango::DbDatum db_datum = database->get_services(servicename, instname); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 	
}

void 
Database::register_service(string servicename,string instname,string devname)
{
	try { 
	    database->register_service(servicename, instname, devname); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

void 
Database::unregister_service(string servicename,string instname)
{
	try { 
	    database->unregister_service(servicename, instname); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 	
}

boost::python::list
Database::get_host_list()
{
	try { 
	    Tango::DbDatum db_datum = database->get_host_list(); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 	
}

boost::python::list
Database::get_host_list(std::string wildcard)
{
	try { 
	    Tango::DbDatum db_datum = database->get_host_list(wildcard); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 	
}

boost::python::list
Database::get_server_class_list(std::string server)
{
	try { 
	    Tango::DbDatum db_datum = database->get_server_class_list(server); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::list
Database::get_server_name_list()
{
	try { 
	    Tango::DbDatum db_datum = database->get_server_name_list(); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::list
Database::get_instance_name_list(std::string servername)
{
	try { 
	    Tango::DbDatum db_datum = database->get_instance_name_list(servername); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::list
Database::get_server_list()
{
	try { 
	    Tango::DbDatum db_datum = database->get_server_list(); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::list
Database::get_server_list(std::string wildcard)
{
	try { 
	    Tango::DbDatum db_datum = database->get_server_list(wildcard); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::list
Database::get_host_server_list(std::string hostname)
{
	try { 
	    Tango::DbDatum db_datum = database->get_host_server_list(hostname); 
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 		
}

boost::python::object
Database::get_server_info(std::string server)
{
	try { 
		return (boost::python::object) (database->get_server_info(server)); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

void
Database::put_server_info(boost::python::object info)
{
	try {
		boost::python::extract<Tango::DbServerInfo&> _info(info);
		if (_info.check())
		{
			Tango::DbServerInfo &tg_info = _info();
			database->put_server_info(tg_info);
		}
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

void
Database::delete_server_info(std::string server)
{
	try {
		database->delete_server_info(server); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	}	
}


boost::python::list 
Database::get_device_name(std::string serv_name,std::string class_name)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_name(serv_name,class_name);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::list 
Database::get_device_exported(std::string filter)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_exported(filter);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}



boost::python::list 
Database::get_device_domain(std::string filter)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_domain(filter);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::list 
Database::get_device_family(std::string filter)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_family(filter);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::list 
Database::get_device_member(std::string filter)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_member(filter);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::list 
Database::get_device_class_list(std::string server)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_class_list(server);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

std::string
Database::get_class_for_device(std::string dev_name)
{
  try { 
    return database->get_class_for_device(dev_name);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::list 
Database::get_device_exported_for_class(std::string classname)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_exported_for_class(classname);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::list 
Database::get_object_list(std::string wildcard)
{
	try { 
		Tango::DbDatum db_datum = database->get_object_list(wildcard);
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}
      
boost::python::list 
Database::get_object_property_list(std::string objectname, std::string wildcard)
{
	try { 
		Tango::DbDatum db_datum = database->get_object_property_list(objectname, wildcard);
		return get_list_from_vector<std::string>(db_datum.value_string);  
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 	
}

boost::python::list 
Database::get_device_property_list(std::string dev_name,std::string filter)
{
  try { 
    Tango::DbDatum db_datum = database->get_device_property_list(dev_name,filter);
	return get_list_from_vector<std::string>(db_datum.value_string);  
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::dict 
Database::get_property(std::string obj,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->get_property(obj,db_data);
    return translate_dbdata_to_property_dict(db_data,list_prop);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::put_property(std::string obj,boost::python::dict py_list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_dict_to_valued_dbdata(py_list_prop,db_data);
    database->put_property(obj,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_property(std::string obj,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->delete_property(obj,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

boost::python::dict 
Database::get_device_property(std::string dev_name,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->get_device_property(dev_name,db_data);
    return translate_dbdata_to_property_dict(db_data,list_prop);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::put_device_property(std::string dev_name,boost::python::dict py_list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_dict_to_valued_dbdata(py_list_prop,db_data);
    database->put_device_property(dev_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_device_property(std::string dev_name,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->delete_device_property(dev_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::dict 
Database::get_device_attribute_property(std::string dev_name,boost::python::list list_attr)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_attr,db_data);
    database->get_device_attribute_property(dev_name,db_data);
    return translate_dbdata_to_attribute_property_dict(db_data,list_attr);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::put_device_attribute_property(std::string dev_name,boost::python::dict py_list_attr_prop)
{
  try { 
    Tango::DbData db_data;
	translate_attr_dict_to_valued_dbdata(py_list_attr_prop,db_data);
    database->put_device_attribute_property(dev_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_device_attribute_property(std::string dev_name,boost::python::dict attr_dict)
{
  try { 
    Tango::DbData db_data;
	translate_attr_dict_to_str_valued_dbdata(attr_dict,db_data);
    database->delete_device_attribute_property(dev_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::dict 
Database::get_class_property(std::string class_name,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->get_class_property(class_name,db_data);
    return translate_dbdata_to_property_dict(db_data,list_prop);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::put_class_property(std::string class_name,boost::python::dict py_list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_dict_to_valued_dbdata(py_list_prop,db_data);
    database->put_class_property(class_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_class_property(std::string class_name,boost::python::list list_prop)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_prop,db_data);
    database->delete_class_property(class_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


boost::python::dict 
Database::get_class_attribute_property(std::string class_name,boost::python::list list_attr)
{
  try { 
    Tango::DbData db_data;
	translate_list_to_nonvalued_dbdata(list_attr,db_data);
    database->get_class_attribute_property(class_name,db_data);
    return translate_dbdata_to_attribute_property_dict(db_data,list_attr);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}


void 
Database::put_class_attribute_property(std::string class_name,boost::python::dict py_list_attr_prop)
{
  try { 
    Tango::DbData db_data;
	translate_attr_dict_to_valued_dbdata(py_list_attr_prop,db_data);
    database->put_class_attribute_property(class_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_class_attribute_property(std::string class_name,boost::python::dict attr_dict)
{
  try { 
    Tango::DbData db_data;
	translate_attr_dict_to_str_valued_dbdata(attr_dict,db_data);
    database->delete_class_attribute_property(class_name,db_data);
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

std::string
Database::get_alias(std::string dev_name)
{
  try {
    string dev_alias;
    database->get_alias(dev_name,dev_alias);
	return dev_alias;
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

std::string
Database::get_device_alias(std::string dev_alias)
{
  try {
    string dev_name;
    database->get_device_alias(dev_alias,dev_name);
	return dev_name;
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::put_device_alias(std::string dev_name,std::string alias_name)
{
  try { 
    database->put_device_alias(dev_name,alias_name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}

void 
Database::delete_device_alias(std::string alias_name)
{
  try { 
    database->delete_device_alias(alias_name); 
  } catch(const Tango::ConnectionFailed &e) {
    throw e;
  } catch(const Tango::CommunicationFailed &e) {
    throw e;
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
}














