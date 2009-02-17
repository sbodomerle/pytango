static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/device_group.cpp,v 1.2 2007/04/02 07:41:59 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         device_group.cpp
//
// description :  C++ source for the light Group class
//                intented to be exported to python world.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.2 $
//
// $Log: device_group.cpp,v $
// Revision 1.2  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.1  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
//
//
//
// copyleft :     Synchrotron SOLEIL
//			L'Orme des Merisiers, Saint-Aubin BP 48
//			91192 GIF SUR YVETTE Cedex
//
//-=============================================================================

#include <device_group.h>
#include <templates.cpp>
#include "conversion_utils.h"


GroupReply::GroupReply(std::string dev_name, 
					   std::string obj_name) 
: dev_name_m(dev_name),obj_name_m(obj_name)
{
}

GroupReply::GroupReply() 
: dev_name_m(""),obj_name_m(""),has_failed_m(true)
{
	exception_m = boost::python::tuple();
	data_m = boost::python::object();
}

GroupReply::~GroupReply()
{ 
}

void 
GroupReply::set_data(boost::python::object data)
{ 
    data_m = data;
    has_failed_m = false;
}     

void 
GroupReply::set_exception(const Tango::DevErrorList &errors)
{ 
    errors_m = errors; 
    has_failed_m = true;
}     

boost::python::tuple 
GroupReply::get_err_stack (void) const
{
	return Utils::translate_exception_value(errors_m);
}

boost::python::object 
GroupReply::get_data()
{
	if (has_failed_m==true)
	{
		Tango::DevFailed df(errors_m);
		throw df;
	} 
    return data_m; 
}     

Group::Group(std::string name) : 
	group_name(name),was_allocated(true)
{
    group_m = new Tango::Group(group_name);
	groupUtils = new GroupUtils(group_m);
}

Group::Group(Tango::Group *group,std::string name) : 
	group_name(name),was_allocated(false),group_m(group)
{
	groupUtils = new GroupUtils(group_m);
}

Group::~Group()
{
	if (was_allocated)
    	delete group_m;
    delete groupUtils;
}

void Group::enable(const std::string& device_name, boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      group_m->enable(device_name);
   } else {
   	  bool fwd = pyData;
      group_m->enable(device_name,fwd);  	 
   }
}

void Group::disable(const std::string& device_name, boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      group_m->disable(device_name);
   } else {
   	  bool fwd = pyData;
      group_m->disable(device_name,fwd);  	 
   }
}

void 
Group::add(boost::python::object group)
{
	    Group * groupref = boost::python::extract<Group *>(group); 
	    group_m->add(groupref->group_m);
	    groupref->was_allocated = false;
}

void 
Group::add(const std::string& pattern)
{
	    group_m->add(pattern);
}

void 
Group::add(boost::python::list patterns)
{
	std::vector<std::string> str_array;
	get_array_value<std::string>(str_array,"string",patterns);
	group_m->add(str_array);
}

void 
Group::remove(const std::string& pattern, boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      group_m->remove(pattern);
   } else {
   	  bool fwd = pyData;
      group_m->remove(pattern,fwd);  	 
   }
}

void 
Group::remove_all()
{
   group_m->remove_all();  	 
}

bool 
Group::contains(const std::string& pattern, boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      return group_m->contains(pattern);
   } else {
   	  bool fwd = pyData;
      return group_m->contains(pattern,fwd);  	 
   }
}

long 
Group::get_size(boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      return group_m->get_size();
   } else {
   	  bool fwd = pyData;
      return group_m->get_size(fwd);  	 
   }
}

boost::python::list 
Group::get_device_list(boost::python::object pyData)
{
   std::vector<std::string> v_str;
   if (pyData.ptr() == Py_None) {
      v_str = group_m->get_device_list();
   } else {
   	  bool fwd = pyData;
      v_str = group_m->get_device_list(fwd);  	 
   }
   boost::python::list pyDevList = get_list_from_vector<std::string>(v_str);
   return pyDevList;
   
}

boost::python::object 
Group::get_device(const std::string& device_name)
{
	Tango::DeviceProxy *dev = group_m->get_device(device_name);
	if (dev==NULL)
		return boost::python::object();
	else {
		DeviceProxy *devPy = new DeviceProxy(dev);
		return boost::python::object(boost::python::ptr(devPy));
	}   
}

boost::python::object 
Group::get_device(long idx)
{
	Tango::DeviceProxy *dev = group_m->get_device(idx);
	if (dev==NULL)
		return boost::python::object();
	else {
		DeviceProxy *devPy = new DeviceProxy(dev);
		return boost::python::object(boost::python::ptr(devPy));
	}   
}

boost::python::object 
Group::get_group(const std::string& group_name)
{
	Tango::Group *group = group_m->get_group(group_name);
	if (group==NULL)
		return boost::python::object();
	else {
		Group *groupPy = new Group(group,group_name);
		return boost::python::object(boost::python::ptr(groupPy));
	}   
}

boost::python::object 
Group::ping(boost::python::object pyData)
{
   if (pyData.ptr() == Py_None) {
      return boost::python::object(group_m->ping());
   } else {
   	  bool fwd = pyData;
      return boost::python::object(group_m->ping(fwd));  	 
   }
   
}

boost::python::list 
Group::command_inout(const std::string &command,boost::python::object pyFWD)
{ 
	try { 
		if (pyFWD.ptr() == Py_None) {
			Tango::GroupCmdReplyList groupList = group_m->command_inout(command);
			return groupUtils->translate_command_reply_list(groupList);
		} else {
			bool fwd = pyFWD;
			Tango::GroupCmdReplyList groupList = group_m->command_inout(command,fwd);
			return groupUtils->translate_command_reply_list(groupList);
		}
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
Group::command_inout(const std::string &command,boost::python::object pyData,boost::python::object pyFWD)
{ 
	try 
	{ 
		pair<long,long> arg_types = groupUtils->get_command_args(command);
		
		Tango::AttrDataFormat in_fmt = groupUtils->get_format_from_type(arg_types.first);
		
		if(in_fmt == Tango::SCALAR)
		{
			// Specific values for each device
			if(PyList_Check(pyData.ptr()))
			{
				Tango::GroupCmdReplyList groupList 
      			= groupUtils->apply_command_to_arglist(command,pyData,pyFWD,arg_types.first);
      			return groupUtils->translate_command_reply_list(groupList);		
			}
			// Same value for all devices
			else
			{
				Tango::DeviceData data;
				Utils::translate_to_device_data(data,pyData,arg_types.first);
				
				if(pyFWD.ptr() == Py_None) {
					Tango::GroupCmdReplyList groupList = group_m->command_inout(command,data);
					return groupUtils->translate_command_reply_list(groupList);
				}
				else {
					bool fwd = pyFWD;
					Tango::GroupCmdReplyList groupList = group_m->command_inout(command,data,fwd);
					return groupUtils->translate_command_reply_list(groupList);
				}
				
			}	
		}
     	else // SPECTRUM
     	{
			if(!PyList_Check(pyData.ptr()))
			{
				// throw exception	
			}
			
			// Specific values for each device
			if(PyList_Check(PyList_GetItem(pyData.ptr(),0)))
			{
				Tango::GroupCmdReplyList groupList 
      			= groupUtils->apply_command_to_arglist(command,pyData,pyFWD,arg_types.first);
      			return groupUtils->translate_command_reply_list(groupList);		
			}
			// Same value for all devices
			else
			{
				Tango::DeviceData data;
				Utils::translate_to_device_data(data,pyData,arg_types.first);
				if(pyFWD.ptr() == Py_None) {				
					Tango::GroupCmdReplyList groupList = group_m->command_inout(command,data);
					return groupUtils->translate_command_reply_list(groupList);
				}
				else {
					bool fwd = pyFWD;
					Tango::GroupCmdReplyList groupList = group_m->command_inout(command,data,fwd);
					return groupUtils->translate_command_reply_list(groupList);
				}
			}
     	}
     
  } catch(const Tango::DevFailed &e) {
    throw e;
  } 
} 

long 
Group::command_inout_asynch(const std::string &command,boost::python::object pyFGT,boost::python::object pyFWD)
{ 
	try 
	{
		if(pyFGT.ptr() == Py_None)
		{
			return group_m->command_inout_asynch(command);
		}
		else 
		{
			bool fgt = pyFGT;
			if(pyFWD.ptr() == Py_None)
			{
				return group_m->command_inout_asynch(command,fgt);
			}
			else 
			{
				bool fwd = pyFWD;
				return group_m->command_inout_asynch(command,fgt,fwd);
			}
		}	
	}
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	}
} 

long 
Group::command_inout_asynch(const std::string &command,boost::python::object pyData,boost::python::object pyFGT,boost::python::object pyFWD)
{ 
	try 
	{ 
		pair<long,long> arg_types = groupUtils->get_command_args(command);
		Tango::DeviceData data;
		Utils::translate_to_device_data(data,pyData,arg_types.first);
		
		if(pyFGT.ptr() == Py_None)
		{
			return group_m->command_inout_asynch(command,data);
		}
		else 
		{
			bool fgt = pyFGT;
			if(pyFWD.ptr() == Py_None)
			{
				return group_m->command_inout_asynch(command,data,fgt);
			}
			else 
			{
				bool fwd = pyFWD;
				return group_m->command_inout_asynch(command,data,fgt,fwd);
			}
		}	
	} 
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
} 

long 
Group::command_inout_asynch(const std::string &command,boost::python::list pyDataList,boost::python::object pyFGT,boost::python::object pyFWD)
{ 
	try 
	{ 
		pair<long,long> arg_types = groupUtils->get_command_args(command);
		return groupUtils->apply_command_asynch_to_arglist(command,(boost::python::object)pyDataList,pyFGT,pyFWD,arg_types.first);
	}
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
} 

boost::python::list 
Group::command_inout_reply(long req_id,long timeout_ms)
{ 
	try 
	{ 
		Tango::GroupCmdReplyList groupList = group_m->command_inout_reply(req_id,timeout_ms);
		return groupUtils->translate_command_reply_list(groupList);
	} 
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
} 

boost::python::list 
Group::read_attribute(const std::string &attr,boost::python::object pyFWD)
{ 
	try 
	{
		if(pyFWD.ptr() == Py_None)
		{
			Tango::GroupAttrReplyList groupList = group_m->read_attribute(attr);
			return groupUtils->translate_readattr_reply_list(groupList);
		}
		else
		{
			bool fwd = pyFWD;
			Tango::GroupAttrReplyList groupList = group_m->read_attribute(attr,fwd);
			return groupUtils->translate_readattr_reply_list(groupList);
		}
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
} 

boost::python::list 
Group::read_attributes(boost::python::list attr_list,boost::python::object pyFWD)
{ 
	try 
	{
		vector<std::string> attr_v;
		get_vector_from_list<std::string>(attr_v,attr_list);
		if(pyFWD.ptr() == Py_None)
		{
			Tango::GroupAttrReplyList groupList = group_m->read_attributes(attr_v);
			return groupUtils->translate_readattr_reply_list(groupList);
		}
		else
		{
			bool fwd = pyFWD;
			Tango::GroupAttrReplyList groupList = group_m->read_attributes(attr_v,fwd);
			return groupUtils->translate_readattr_reply_list(groupList);
		}		
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
} 

long 
Group::read_attribute_asynch(const std::string &attr,boost::python::object pyFWD)
{ 
	try 
	{
		if(pyFWD.ptr() == Py_None)
		{
			return group_m->read_attribute_asynch(attr);
		}
		else
		{
			bool fwd = pyFWD;
			return group_m->read_attribute_asynch(attr,fwd);
		}
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
}

long 
Group::read_attributes_asynch(boost::python::list attr_list,boost::python::object pyFWD)
{ 
	try
	{
		vector<std::string> attr_v;
		get_vector_from_list<std::string>(attr_v,attr_list);
		if(pyFWD.ptr() == Py_None)
		{
			return group_m->read_attributes_asynch(attr_v);
		}
		else
		{
			bool fwd = pyFWD;
			return group_m->read_attributes_asynch(attr_v,fwd);	
		}
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
} 

boost::python::list 
Group::read_attribute_reply(long req_id,long timeout_ms)
{ 
	try
	{ 
		Tango::GroupAttrReplyList groupList = group_m->read_attribute_reply(req_id,timeout_ms);
		return groupUtils->translate_readattr_reply_list(groupList);
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
}

boost::python::list 
Group::read_attributes_reply(long req_id,long timeout_ms)
{ 
	try
	{ 
		Tango::GroupAttrReplyList groupList = group_m->read_attributes_reply(req_id,timeout_ms);
		return groupUtils->translate_readattr_reply_list(groupList);
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	} 
} 

boost::python::list 
Group::write_attribute(AttributeValue attr_val,boost::python::object pyFWD)
{
	try
	{ 
		Tango::DeviceAttribute dev_attr;
		std::string name = attr_val.name;
		long arg_type = groupUtils->get_attribute_type(name).first;
		Tango::AttrDataFormat arg_format = groupUtils->get_attribute_type(name).second;
		Utils::translate_to_device_attribute(dev_attr,attr_val,arg_type,arg_format);
		if(pyFWD.ptr() == Py_None)
		{
			Tango::GroupReplyList groupList = group_m->write_attribute(dev_attr);
			return groupUtils->translate_writeattr_reply_list(groupList);
		}
		else
		{
			bool fwd = pyFWD;
			Tango::GroupReplyList groupList = group_m->write_attribute(dev_attr,fwd);
			return groupUtils->translate_writeattr_reply_list(groupList);
		}
	} 
	catch(const Tango::DevFailed &e) 
	{
	    throw e;
	} 
}

boost::python::list 
Group::write_attribute(const string &attr,boost::python::object pyData,boost::python::object pyFWD)
{ 
	try 
	{ 
		pair<long,Tango::AttrDataFormat> attr_typ_fmt = groupUtils->get_attribute_type(attr);
		if(attr_typ_fmt.second == Tango::SCALAR)
		{
			// Specific values for each device
			if(PyList_Check(pyData.ptr()))
			{
				Tango::GroupReplyList groupList = groupUtils->apply_write_attrib_to_arglist(attr,pyData,pyFWD,false);
				return groupUtils->translate_writeattr_reply_list(groupList);
			}
		}
		else if(attr_typ_fmt.second == Tango::SPECTRUM)
		{
			// Specific values for each device
			if(PyList_Check(PyList_GetItem(pyData.ptr(),0)))
			{
				Tango::GroupReplyList groupList = groupUtils->apply_write_attrib_to_arglist(attr,pyData,pyFWD,false);
				return groupUtils->translate_writeattr_reply_list(groupList);
			}
		}
		else if(attr_typ_fmt.second == Tango::IMAGE)
		{
			TangoSys_OMemStream o;
			o << "Unsupported Python group call for attribute " << attr;
			o << "\nwrite_attribute on groups with individualized values not supported for IMAGE attributes." << ends;
	
			Tango::Except::throw_exception((const char *)"PyDs_UnsupportedPythonDataFormatForAttribute",
				       o.str(),
				       (const char *)"write_attribute()");
		}
		
		// Same value for all devices
		AttributeValue attr_val;
		attr_val.name = attr;
		attr_val.value = pyData;
		return write_attribute(attr_val,pyFWD);
		
	} 
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
}

long 
Group::write_attribute_asynch(AttributeValue attr_val,boost::python::object pyFWD)
{
	try 
	{ 
		Tango::DeviceAttribute dev_attr;
		std::string name = attr_val.name;
		long arg_type = groupUtils->get_attribute_type(name).first;
		Tango::AttrDataFormat arg_format = groupUtils->get_attribute_type(name).second;
		Utils::translate_to_device_attribute(dev_attr,attr_val,arg_type,arg_format);
		if(pyFWD.ptr() == Py_None)
		{
			return group_m->write_attribute_asynch(dev_attr);
		}
		else
		{
			bool fwd = pyFWD;
			return group_m->write_attribute_asynch(dev_attr,fwd);
		}
	}
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
}

long
Group::write_attribute_asynch(const string &attr,boost::python::object pyData,boost::python::object pyFWD)
{ 
	try { 
		pair<long,Tango::AttrDataFormat> attr_typ_fmt = groupUtils->get_attribute_type(attr);
		if(attr_typ_fmt.second == Tango::SCALAR)
		{
			// Specific values for each device
			if(PyList_Check(pyData.ptr()))
			{
				return groupUtils->apply_write_attrib_asynch_to_arglist(attr,pyData,pyFWD,false);
			}
		}
		else if(attr_typ_fmt.second == Tango::SPECTRUM)
		{
			// Specific values for each device
			if(PyList_Check(PyList_GetItem(pyData.ptr(),0)))
			{
				return groupUtils->apply_write_attrib_asynch_to_arglist(attr,pyData,pyFWD,false);
			}
		}
		else if(attr_typ_fmt.second == Tango::IMAGE)
		{
			TangoSys_OMemStream o;
			o << "Unsupported Python group call for attribute " << attr;
			o << "\nwrite_attribute on groups with individualized values not supported for IMAGE attributes." << ends;
	
			Tango::Except::throw_exception((const char *)"PyDs_UnsupportedPythonDataFormatForAttribute",
				       o.str(),
				       (const char *)"write_attribute_asynch()");
		}
		
		// Same value for all devices
		AttributeValue attr_val;
		attr_val.name = attr;
		attr_val.value = pyData;
		return write_attribute_asynch(attr_val,pyFWD);
		
	} 
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
}

boost::python::list 
Group::write_attribute_reply(long req_id,long timeout_ms)
{ 
	try 
	{ 
		Tango::GroupReplyList groupList = group_m->write_attribute_reply(req_id,timeout_ms);
		return groupUtils->translate_writeattr_reply_list(groupList);
	}
	catch(const Tango::DevFailed &e) 
	{
		throw e;
	} 
} 

boost::python::object 
Group::getself()
{	
	    boost::shared_ptr<Group>  self =  shared_from_this();
//		assert(self != 0);
		return boost::python::object ( self );
}
