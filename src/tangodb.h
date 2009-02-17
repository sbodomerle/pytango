//+=============================================================================
//
// file :         tangodb.h
//
// description :  header file for all principal C++ classes for 
//			the Database API's part of the python binding module.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.11 $
//
// $Log: tangodb.h,v $
// Revision 1.11  2008/02/13 16:30:36  tiagocoutinho
// added get_db_host, get_db_port, get_db_port_num
//
// Revision 1.10  2008/01/23 12:18:53  tiagocoutinho
// - Added missing database API
//
// Revision 1.9  2008/01/22 09:32:25  tiagocoutinho
// added to Database API: get_server_info, put_server_info, delete_server_info
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
// Revision 1.5  2006/01/18 14:26:57  taurel
// - Add a "typename" definition in order to remove compilation warning
// - Change include file order to also remove compiler warnings
// - Now call device server module wrapped stuff
//
// Revision 1.4  2005/10/31 09:03:01  dusdan
// fixed bug with python2.4 and state attribute subscribe_event;
// added new Database object constructor
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
#ifndef PY_TANGO_Database
#define PY_TANGO_Database


#include <boost/python.hpp>
#include <tango.h>
#include <dbapi.h>


class Database
{
public:

      Database();
      Database(std::string host, int port);

     ~Database();
     
// Methods from the Tango::Connection superclass
     
     std::string get_db_host();
     
     std::string get_db_port();
     
     int get_db_port_num();
     
// General methods
     
      std::string get_info();

      void add_device(Tango::DbDevInfo dev_info);

      void delete_device(std::string name);

      boost::python::object import_device(std::string name);

      void export_device(Tango::DbDevExportInfo export_info);

      void unexport_device(std::string name);

      void add_server(std::string name,boost::python::list dev_infos);

      void delete_server(std::string name);

      void export_server(std::string name,boost::python::list dev_export_infos);

      void unexport_server(std::string name);
      
      boost::python::list get_services(string servicename,string instname);

      void register_service(string servicename,string instname,string devname);
      
      void unregister_service(string servicename,string instname);
      
      boost::python::list get_host_list();
      
      boost::python::list get_host_list(std::string wildcard);
      
      boost::python::list get_server_class_list(std::string server);
      
      boost::python::list get_server_name_list();
      
      boost::python::list get_instance_name_list(string servername);
      
      boost::python::list get_server_list();
      
      boost::python::list get_server_list(string wildcard);
      
      boost::python::list get_host_server_list(string hostname);
      
      boost::python::object get_server_info(std::string server);
      
      void put_server_info(boost::python::object info);
      
      void delete_server_info(std::string server);
      
      boost::python::list get_device_name(std::string serv_name,std::string class_name);

      boost::python::list get_device_exported(std::string filter);
      
      boost::python::list get_device_domain(std::string filter);

      boost::python::list get_device_family(std::string filter);

      boost::python::list get_device_member(std::string filter);

      boost::python::list get_device_class_list(std::string server);

      std::string get_class_for_device(std::string name);

      boost::python::list get_device_exported_for_class(std::string classname);
      
      boost::python::list get_object_list(std::string wildcard);
      
      boost::python::list get_object_property_list(std::string objectname, std::string wildcard);

      boost::python::dict get_property(std::string obj,boost::python::list list_prop);

      void put_property(std::string obj,boost::python::dict py_list_prop);

      void delete_property(std::string obj,boost::python::list list_prop);

      boost::python::dict get_device_property(std::string dev_name,boost::python::list list_prop);

      boost::python::list get_device_property_list(std::string dev_name,std::string filter);

      void put_device_property(std::string dev_name,boost::python::dict py_list_prop);

      void delete_device_property(std::string dev_name,boost::python::list list_prop);

      boost::python::dict get_device_attribute_property(std::string dev_name,boost::python::list list_attr);

      void put_device_attribute_property(std::string dev_name,boost::python::dict py_list_attr_prop);

      void delete_device_attribute_property(std::string dev_name,boost::python::dict attr_dict);

      boost::python::dict get_class_property(std::string,boost::python::list list_prop);

      void put_class_property(std::string,boost::python::dict list_prop);

      void delete_class_property(std::string,boost::python::list list_prop);

      boost::python::dict get_class_attribute_property(std::string,boost::python::list list_prop);

      void put_class_attribute_property(std::string,boost::python::dict py_list_prop);

      void delete_class_attribute_property(std::string,boost::python::dict attr_dict);
      
      std::string get_alias(std::string dev_name);

      std::string get_device_alias(std::string dev_alias);
      
      void put_device_alias(std::string dev_name,std::string alias_name);
      
      void delete_device_alias(std::string alias_name);

private:
      Tango::Database *database;

//Utils
void translate_list_to_nonvalued_dbdata(boost::python::list list_prop,Tango::DbData &db_data);
void translate_dict_to_valued_dbdata(boost::python::dict py_list_prop,Tango::DbData &db_data);
void translate_attr_dict_to_valued_dbdata(boost::python::dict py_attr_list_prop,Tango::DbData &db_data);
void translate_attr_dict_to_str_valued_dbdata(boost::python::dict py_attr_list_prop,Tango::DbData &db_data);
boost::python::dict translate_dbdata_to_property_dict(Tango::DbData &db_data,boost::python::list list_prop);
boost::python::dict translate_dbdata_to_attribute_property_dict(Tango::DbData &db_data,boost::python::list list_attr);

};




template<class T>
void get_scalar_value(T &val,std::string typ,boost::python::object pyData); 

template<class T>
void get_array_value(std::vector<T> &val_array,std::string typ,boost::python::object pyData); 

template<class T>
boost::python::list get_list_from_vector(std::vector<T> &val_array) ;


#endif






























