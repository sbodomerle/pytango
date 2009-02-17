//+=============================================================================
//
// file :         device_group.h
//
// description :  header file for all principal C++ classes of 
//			the python binding for group functionalities.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.2 $
//
// $Log: device_group.h,v $
// Revision 1.2  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.1  2005/06/02 17:10:53  ounsy
// First introduction of group related methods
//
//
//
// copyleft :     Synchrotron SOLEIL
//			L'Orme des Merisiers, Saint-Aubin BP 48
//			91192 GIF SUR YVETTE Cedex
//
//-=============================================================================
#ifndef PY_DEVICE_GROUP
#define PY_DEVICE_GROUP

#include <device_proxy.h>
/*#include <map>
#include <boost/enable_shared_from_this.hpp>
#include <boost/any.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/long.hpp>
#include <boost/python/call_method.hpp>*/


//=============================================================================
// class GroupReply : reply to command or attribute read/write executed on a group
//=============================================================================
class GroupReply
{
public:
  GroupReply();
  GroupReply (std::string dev_name, 
              std::string obj_name); 
  //- dtor
  virtual ~GroupReply ();
  
  void set_data(boost::python::object data) ;
  void set_exception(const Tango::DevErrorList &errors) ;
  bool has_failed (void) const { 
    return has_failed_m;
  }
  //- data accessor (may throw Tango::DevFailed)
  boost::python::object  get_data (void);
  //- device name accessor
  	std::string dev_name (void) const {
    return dev_name_m;
  }
  //- object (i.e. command or attribute) name accessor
  	std::string obj_name (void) const {
    return obj_name_m;
  }
  //- error stack accessor
  boost::python::tuple get_err_stack (void) const;
  
private:
  //- the device name
  std::string dev_name_m;
  //- command or attribute name
  std::string obj_name_m;
  //- has_failed_m: true is an error occurred, false otherwise
  bool has_failed_m;
  //- exception: valid if has_failed_m set to true
  Tango::DevErrorList     errors_m;
  boost::python::tuple exception_m;
  //- data: valid if GroupReply::has_failed_m set to false
  boost::python::object data_m;
};


class GroupUtils
{
public:
	GroupUtils(Tango::Group *group);
//   Utils
     std::pair<long,long> get_command_args(std::string);
     std::pair<long,Tango::AttrDataFormat> get_attribute_type(std::string);

     Tango::AttrDataFormat get_format_from_type(long );
     GroupReply translate_command_reply(Tango::GroupCmdReply *);
     boost::python::object translate_from_command_reply(Tango::GroupCmdReply &,long );
     boost::python::list translate_command_reply_list(Tango::GroupCmdReplyList &); 	
     GroupReply translate_readattr_reply(Tango::GroupAttrReply *);
     boost::python::object translate_from_readattr_reply(Tango::GroupAttrReply &,long,Tango::AttrDataFormat);
     boost::python::list translate_readattr_reply_list(Tango::GroupAttrReplyList &); 	
     Tango::GroupCmdReplyList apply_command_to_arglist(const std::string &,boost::python::object,boost::python::object,long);
     long apply_command_asynch_to_arglist(const std::string &,boost::python::object,boost::python::object,boost::python::object,long);
	 boost::python::list translate_writeattr_reply_list(Tango::GroupReplyList &); 
	 GroupReply translate_writeattr_reply(Tango::GroupReply *); 

	Tango::GroupReplyList apply_write_attrib_to_arglist(const std::string &,boost::python::object,boost::python::object,bool);
	long apply_write_attrib_asynch_to_arglist(const std::string &,boost::python::object,boost::python::object,bool);	
		
	template<class T> void get_scalar_values_from_attrib_values(std::vector<T> &,boost::python::object);
	template<class T> void get_spectrum_values_from_attrib_values(std::vector<std::vector<T> > &,const char *,boost::python::object);
	template<class T> void get_spectrum_values(std::vector<std::vector<T> > &,const char *,boost::python::object);

private:
      Tango::Group *group_m;
      std::map< std::string, std::pair<long,long> > command_args;
      std::map< std::string, std::pair<long,Tango::AttrDataFormat> > attribute_types;
};

class Group : public boost::enable_shared_from_this<Group>
{
public:

      Group(std::string name);
      Group(Tango::Group *group,std::string name);

     virtual ~Group();

//- Misc.
      std::string get_name(void) { return group_name; }
      std::string get_fully_qualified_name(void)  { return group_m->get_fully_qualified_name(); }
      void enable(const std::string&, boost::python::object pyData=boost::python::object());
      void disable(const std::string&, boost::python::object pyData=boost::python::object());
      bool is_enabled(void) { return group_m->is_enabled(); }
      
// Group management related methods
      void add(boost::python::object);
      void add(const std::string&);
      void add(boost::python::list);
      void remove(const std::string&, boost::python::object pyData=boost::python::object());
      void remove_all();
      bool contains(const std::string&, boost::python::object pyData=boost::python::object());
      long get_size(boost::python::object pyData=boost::python::object());
      boost::python::list get_device_list(boost::python::object pyData = boost::python::object());
      boost::python::object get_device(const std::string&);
      boost::python::object get_device(long);
      boost::python::object get_group(const std::string&);
      boost::python::object ping(boost::python::object pyData = boost::python::object());
      boost::python::list command_inout(const std::string&, boost::python::object);
      boost::python::list command_inout(const std::string&, boost::python::object, boost::python::object);
      long command_inout_asynch(const std::string&, boost::python::object, boost::python::object);
      long command_inout_asynch(const std::string&, boost::python::object, boost::python::object,boost::python::object);
      long command_inout_asynch(const std::string&, boost::python::list, boost::python::object,boost::python::object);
      boost::python::list command_inout_reply(long req_id, long timeout_ms = 0);
      boost::python::list read_attribute(const std::string&, boost::python::object fwd = boost::python::object());
      boost::python::list read_attributes(boost::python::list, boost::python::object fwd = boost::python::object());
      long read_attribute_asynch(const std::string&, boost::python::object fwd = boost::python::object());
      long read_attributes_asynch(boost::python::list, boost::python::object fwd = boost::python::object());
      boost::python::list read_attribute_reply(long,long timeout_ms = 0);
      boost::python::list read_attributes_reply(long,long timeout_ms = 0);
	  boost::python::list write_attribute(AttributeValue, boost::python::object pyFWD = boost::python::object());
	  boost::python::list write_attribute(const string&, boost::python::object, boost::python::object pyFWD = boost::python::object());
	  long write_attribute_asynch(AttributeValue ,boost::python::object pyFWD = boost::python::object());
	  long write_attribute_asynch(const string&, boost::python::object, boost::python::object pyFWD = boost::python::object());
	  boost::python::list write_attribute_reply(long, long timeout_ms=0);
	  
// special methods
      boost::python::object getself();
private:
	  std::string group_name;
	  bool was_allocated;
      Tango::Group *group_m;
	  GroupUtils *groupUtils;
};


#endif
