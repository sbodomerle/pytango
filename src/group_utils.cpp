static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/group_utils.cpp,v 1.3 2007/04/02 07:41:59 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         group_utils.cpp
//
// description :  C++ source for the utility methods used in the 
//                other source files.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.3 $
//
// $Log: group_utils.cpp,v $
// Revision 1.3  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.2  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.1  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
//
// Revision 1.2  2004/01/27 09:59:57  ounsy
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

#include <device_group.h>

#include <templates.cpp>


GroupUtils::GroupUtils(Tango::Group *group) : group_m(group)
{
}

Tango::AttrDataFormat GroupUtils::get_format_from_type(long type)
{
	if((type >= Tango::DEVVAR_CHARARRAY && type <= Tango::DEVVAR_DOUBLESTRINGARRAY) ||
	   type == Tango::DEVVAR_BOOLEANARRAY || type == Tango::DEVVAR_LONG64ARRAY ||
	   type == Tango::DEVVAR_ULONG64ARRAY)
		return Tango::SPECTRUM;
	else
		return Tango::SCALAR;  	
} 

std::pair<long,long>
GroupUtils::get_command_args(std::string name)
{
    if ( command_args.count(name) == 0 ) {
    Tango::DeviceProxy *deviceProxy = group_m->get_device(1);
	Tango::CommandInfo commandInfo = deviceProxy->command_query(name);
        command_args[name] = pair<long,long>( commandInfo.in_type , commandInfo.out_type );	
    }
    return command_args[name]; 
}

std::pair<long,Tango::AttrDataFormat>
GroupUtils::get_attribute_type(std::string name)
{
	if ( attribute_types.count(name) == 0 ) {
		Tango::DeviceProxy *deviceProxy = group_m->get_device(1);
		Tango::AttributeInfoEx attributeInfo = deviceProxy->attribute_query(name);
		attribute_types[name] = pair<long,Tango::AttrDataFormat>( attributeInfo.data_type , attributeInfo.data_format );	
	}
	return attribute_types[name];
}

boost::python::list
GroupUtils::translate_command_reply_list(Tango::GroupCmdReplyList &groupList) 
{
    boost::python::list pyCmdReplyList;
    std::vector<Tango::GroupCmdReply>::iterator i;
    for (i = groupList.begin() ; i < groupList.end() ; i++ )
    {
       pyCmdReplyList.append( translate_command_reply( &(*i) ) );
    }
    return pyCmdReplyList;
    
}

GroupReply
GroupUtils::translate_command_reply(Tango::GroupCmdReply *groupCmdReply) 
{
    GroupReply groupReply(groupCmdReply->dev_name(),
                          groupCmdReply->obj_name());
    pair<long,long> arg_types = get_command_args(groupCmdReply->obj_name());
    
    boost::python::object result;
    try 
    {
    	result = translate_from_command_reply(*groupCmdReply,arg_types.second);
    	groupReply.set_data(result);
    }
    catch (const Tango::DevFailed& df) 
    {
    	groupReply.set_exception(df.errors);
    }
    
    return groupReply;
}

boost::python::object
GroupUtils::translate_from_command_reply(Tango::GroupCmdReply &groupCmdReply,long arg_type)
{
    boost::python::object def ;
    bool extraction_result = false;
    switch (arg_type) {
    case Tango::DEV_BOOLEAN :
        {
       	   Tango::DevBoolean val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_SHORT :
        {
       	   Tango::DevShort val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_LONG :
        {
       	   Tango::DevLong val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_FLOAT :
        {
       	   Tango::DevFloat val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_DOUBLE :
        {
       	   Tango::DevDouble val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_USHORT :
        {
       	   Tango::DevUShort val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_ULONG :
        {
       	   Tango::DevULong val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::long_(val);
        }
	break;
    case Tango::DEV_STRING :
        {
       	   std::string val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_LONG64 :
        {
       	   Tango::DevLong64 val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::object(val);
        }
	break;	
    case Tango::DEV_ULONG64 :
        {
       	   Tango::DevULong64 val;
       	   extraction_result = groupCmdReply >> val;
       	   return boost::python::long_(val);
        }
	break;
    case Tango::DEVVAR_CHARARRAY :
        {
            std::vector<unsigned char> vchar;
            extraction_result = groupCmdReply >> vchar;
	    return get_list_from_vector<unsigned char>(vchar);
        }
	break;
    case Tango::DEVVAR_SHORTARRAY :
        {
            std::vector<Tango::DevShort> vshort;
            extraction_result = groupCmdReply >> vshort;
	    return get_list_from_vector<Tango::DevShort>(vshort);
        }
	break;
    case Tango::DEVVAR_LONGARRAY :
        {
            std::vector<Tango::DevLong> vlong;
            extraction_result = groupCmdReply >> vlong;
	    return get_list_from_vector<Tango::DevLong>(vlong);
        }
	break;
    case Tango::DEVVAR_FLOATARRAY :
        {
            std::vector<Tango::DevFloat> vfloat;
            extraction_result = groupCmdReply >> vfloat;
	    return get_list_from_vector<Tango::DevFloat>(vfloat);
        }
	break;
    case Tango::DEVVAR_DOUBLEARRAY :
        {
            std::vector<Tango::DevDouble> vdouble;
            extraction_result = groupCmdReply >> vdouble;
	    return get_list_from_vector<Tango::DevDouble>(vdouble);
        }
	break;
    case Tango::DEVVAR_USHORTARRAY :
        {
            std::vector<Tango::DevUShort> vushort;
            extraction_result = groupCmdReply >> vushort;
	    return get_list_from_vector<Tango::DevUShort>(vushort);
        }
	break;
    case Tango::DEVVAR_ULONGARRAY :
        {
            std::vector<Tango::DevULong> vulong;
            extraction_result = groupCmdReply >> vulong;
	    return get_list_from_vector<Tango::DevULong>(vulong);
        }
	break;
    case Tango::DEVVAR_STRINGARRAY :
        {
            std::vector<std::string> vstring;
            extraction_result = groupCmdReply >> vstring;
	    return get_list_from_vector<std::string>(vstring);
        }
	break;
    case Tango::DEVVAR_LONGSTRINGARRAY :
        {
            std::vector<Tango::DevLong> vlong;
            std::vector<std::string> vstring;
            extraction_result = groupCmdReply.extract(vlong,vstring);
	    boost::python::list l_val = get_list_from_vector<Tango::DevLong>(vlong);
	    boost::python::list s_val = get_list_from_vector<std::string>(vstring);
		return boost::python::make_tuple(l_val,s_val);
        }
	break;
    case Tango::DEVVAR_DOUBLESTRINGARRAY :
        {
           std::vector<Tango::DevDouble> vdouble;
            std::vector<std::string> vstring;
            extraction_result = groupCmdReply.extract(vdouble,vstring);
	    boost::python::list d_val = get_list_from_vector<Tango::DevDouble>(vdouble);
	    boost::python::list s_val = get_list_from_vector<std::string>(vstring);
		return boost::python::make_tuple(d_val,s_val);
        }
	break;
    case Tango::DEVVAR_LONG64ARRAY :
        {
            std::vector<Tango::DevLong64> vlong64;
            extraction_result = groupCmdReply >> vlong64;
	    return get_list_from_vector<Tango::DevLong64>(vlong64);
        }
	break;
    case Tango::DEVVAR_ULONG64ARRAY :
        {
            std::vector<Tango::DevULong64> vulong64;
            extraction_result = groupCmdReply >> vulong64;
	    return get_list_from_vector<Tango::DevULong64>(vulong64);
        }
	break;	
    default :
	break;
    }
	if (extraction_result == false)
	{
		const Tango::DevErrorList& errors = groupCmdReply.get_err_stack();
		Tango::DevFailed df(errors);
		throw df;
	}
		
    return def;  
}

template<class T>
void GroupUtils::get_scalar_values_from_attrib_values(std::vector<T> &val_array,boost::python::object pyData)
{
	int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
	for (int i = 0 ; i < arr_len; i++ )
	{
		AttributeValue s_val = boost::python::extract<AttributeValue>(pyData[i]);
		T i_val = boost::python::extract<T>(s_val.value);
		val_array.push_back(i_val);
	}
}

template<class T>
void GroupUtils::get_spectrum_values_from_attrib_values(std::vector<std::vector<T> > &val_array,const char *type,boost::python::object pyData)
{
	int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
	for (int i = 0 ; i < arr_len; i++ )
	{
		AttributeValue s_val = boost::python::extract<AttributeValue>(pyData[i]);
		std::vector<T> value;
		get_array_value(value,type,s_val.value);
		val_array.push_back(value);
	}
}

template<class T>
void GroupUtils::get_spectrum_values(std::vector<std::vector<T> > &val_array,const char *type,boost::python::object pyData)
{
	int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
	for (int i = 0 ; i < arr_len; i++ )
	{
		std::vector<T> value;
		get_array_value(value,type,pyData[i]);
		val_array.push_back(value);
	}
}

Tango::GroupReplyList 
GroupUtils::apply_write_attrib_to_arglist(const std::string &attr_name,boost::python::object pyData,boost::python::object pyFWD,bool has_attr_val)
{
	bool opt_arg = pyFWD.ptr() != Py_None; 
	bool fwd = opt_arg ? pyFWD : true;
	
	pair<long,Tango::AttrDataFormat> arg_typ_fmt = get_attribute_type(attr_name); 
	long arg_type = arg_typ_fmt.first;
	Tango::AttrDataFormat arg_fmt = arg_typ_fmt.second;

	if(arg_fmt == Tango::SCALAR)
	{	
		switch (arg_type) {
		case Tango::DEV_SHORT :
		{
			std::vector<Tango::DevShort> short_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevShort>(short_array,pyData);
			else
				get_array_value<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,short_array,fwd) :
				group_m->write_attribute(attr_name,short_array);
		}
		break;
	
		case Tango::DEV_LONG :
		{
			std::vector<Tango::DevLong> long_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevLong>(long_array,pyData);
			else
				get_array_value<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,long_array,fwd) :
				group_m->write_attribute(attr_name,long_array);
		}
		break;
	
		case Tango::DEV_DOUBLE :
		{
			std::vector<Tango::DevDouble> double_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevDouble>(double_array,pyData);
			else
				get_array_value<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,double_array,fwd) :
				group_m->write_attribute(attr_name,double_array);
		}
		break;
		
		case Tango::DEV_FLOAT :
		{
			std::vector<Tango::DevFloat> float_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevFloat>(float_array,pyData);
			else
				get_array_value<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,float_array,fwd) :
				group_m->write_attribute(attr_name,float_array);
		}
		break;
		
		case Tango::DEV_BOOLEAN :
		{
			std::vector<Tango::DevBoolean> bool_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevBoolean>(bool_array,pyData);
			else
				get_array_value<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,bool_array,fwd) :
				group_m->write_attribute(attr_name,bool_array);
		}
		break;
	
		case Tango::DEV_USHORT :
		{
			std::vector<Tango::DevUShort> ushort_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevUShort>(ushort_array,pyData);
			else
				get_array_value<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,ushort_array,fwd) :
				group_m->write_attribute(attr_name,ushort_array);
		}
		break;
	
		case Tango::DEV_STRING :
		{
			std::vector<Tango::DevString> str_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevString>(str_array,pyData);
			else
				get_array_value<Tango::DevString>(str_array,"Tango::DevString",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,str_array,fwd) :
				group_m->write_attribute(attr_name,str_array);
		}
		break;
		
		case Tango::DEV_UCHAR :
		{
			std::vector<Tango::DevUChar> uchar_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevUChar>(uchar_array,pyData);
			else
				get_array_value<Tango::DevUChar>(uchar_array,"Tango::DevUChar",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,uchar_array,fwd) :
				group_m->write_attribute(attr_name,uchar_array);
		}
		break;
		
		case Tango::DEV_LONG64 :
		{
			std::vector<Tango::DevLong64> long64_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevLong64>(long64_array,pyData);
			else
				get_array_value<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,long64_array,fwd) :
				group_m->write_attribute(attr_name,long64_array);
		}
		break;
				
		default :
			return Tango::GroupReplyList();
		break;
		}
	}
	else if (arg_fmt == Tango::SPECTRUM)
	{
		switch (arg_type) {
		case Tango::DEV_SHORT :
		{
			std::vector<std::vector<Tango::DevShort> > short_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			else
				get_spectrum_values<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,short_array,fwd) :
				group_m->write_attribute(attr_name,short_array);
		}
		break;
	
		case Tango::DEV_LONG :
		{
			std::vector<std::vector<Tango::DevLong> > long_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			else
				get_spectrum_values<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,long_array,fwd) :
				group_m->write_attribute(attr_name,long_array);
		}
		break;
	
		case Tango::DEV_DOUBLE :
		{
			std::vector<std::vector<Tango::DevDouble> > double_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			else
				get_spectrum_values<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,double_array,fwd) :
				group_m->write_attribute(attr_name,double_array);
		}
		break;
		
		case Tango::DEV_FLOAT :
		{
			std::vector<std::vector<Tango::DevFloat> > float_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			else
				get_spectrum_values<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,float_array,fwd) :
				group_m->write_attribute(attr_name,float_array);
		}
		break;
		
		case Tango::DEV_BOOLEAN :
		{
			std::vector<std::vector<Tango::DevBoolean> > bool_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			else
				get_spectrum_values<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,bool_array,fwd) :
				group_m->write_attribute(attr_name,bool_array);
		}
		break;
	
		case Tango::DEV_USHORT :
		{
			std::vector<std::vector<Tango::DevUShort> > ushort_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			else
				get_spectrum_values<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,ushort_array,fwd) :
				group_m->write_attribute(attr_name,ushort_array);
		}
		break;
	
		case Tango::DEV_STRING :
		{
			TangoSys_OMemStream o;
			o << "Unsupported Python group call for attribute " << attr_name;
			o << "\nwrite_attribute on groups with individualized values not supported string spectrum attributes." << ends;
	
			Tango::Except::throw_exception((const char *)"PyDs_UnsupportedPythonDataFormatForAttribute",
				       o.str(),
				       (const char *)"write_attribute()");			
		}
		break;
		
		case Tango::DEV_UCHAR :
		{
			std::vector<std::vector<Tango::DevUChar> > uchar_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevUChar>(uchar_array,"Tango:DevUChar",pyData);
			else
				get_spectrum_values<Tango::DevUChar>(uchar_array,"Tango:DevUChar",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,uchar_array,fwd) :
				group_m->write_attribute(attr_name,uchar_array);
		}
		break;
		
		case Tango::DEV_LONG64 :
		{
			std::vector<std::vector<Tango::DevLong64> > long64_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			else
				get_spectrum_values<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			return opt_arg ? 
				group_m->write_attribute(attr_name,long64_array,fwd) :
				group_m->write_attribute(attr_name,long64_array);
		}
		break;
				
		default :
			return Tango::GroupReplyList();
		break;
		}
	}
	return Tango::GroupReplyList();
}

long
GroupUtils::apply_write_attrib_asynch_to_arglist(const std::string &attr_name,boost::python::object pyData,boost::python::object pyFWD,bool has_attr_val)
{
	bool opt_arg = pyFWD.ptr() != Py_None; 
	bool fwd = opt_arg ? pyFWD : true;
		
	pair<long,Tango::AttrDataFormat> arg_typ_fmt = get_attribute_type(attr_name); 
	long arg_type = arg_typ_fmt.first;
	Tango::AttrDataFormat arg_fmt = arg_typ_fmt.second;

	if(arg_fmt == Tango::SCALAR)
	{	
		switch (arg_type) {
		case Tango::DEV_SHORT :
		{
			std::vector<Tango::DevShort> short_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevShort>(short_array,pyData);
			else
				get_array_value<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,short_array,fwd) :
				group_m->write_attribute_asynch(attr_name,short_array);
		}
		break;
	
		case Tango::DEV_LONG :
		{
			std::vector<Tango::DevLong> long_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevLong>(long_array,pyData);
			else
				get_array_value<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,long_array,fwd) :
				group_m->write_attribute_asynch(attr_name,long_array);
		}
		break;
	
		case Tango::DEV_DOUBLE :
		{
			std::vector<Tango::DevDouble> double_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevDouble>(double_array,pyData);
			else
				get_array_value<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,double_array,fwd) :
				group_m->write_attribute_asynch(attr_name,double_array);
		}
		break;
		
		case Tango::DEV_FLOAT :
		{
			std::vector<Tango::DevFloat> float_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevFloat>(float_array,pyData);
			else
				get_array_value<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,float_array,fwd) :
				group_m->write_attribute_asynch(attr_name,float_array);
		}
		break;
		
		case Tango::DEV_BOOLEAN :
		{
			std::vector<Tango::DevBoolean> bool_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevBoolean>(bool_array,pyData);
			else
				get_array_value<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,bool_array,fwd) :
				group_m->write_attribute_asynch(attr_name,bool_array);
		}
		break;
	
		case Tango::DEV_USHORT :
		{
			std::vector<Tango::DevUShort> ushort_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevUShort>(ushort_array,pyData);
			else
				get_array_value<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,ushort_array,fwd) :
				group_m->write_attribute_asynch(attr_name,ushort_array);
		}
		break;
	
		case Tango::DEV_STRING :
		{
			std::vector<Tango::DevString> str_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevString>(str_array,pyData);
			else
				get_array_value<Tango::DevString>(str_array,"Tango::DevString",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,str_array,fwd) :
				group_m->write_attribute_asynch(attr_name,str_array);
		}
		break;
		
		case Tango::DEV_UCHAR :
		{
			std::vector<Tango::DevUChar> uchar_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevUChar>(uchar_array,pyData);
			else
				get_array_value<Tango::DevUChar>(uchar_array,"Tango::DevUChar",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,uchar_array,fwd) :
				group_m->write_attribute_asynch(attr_name,uchar_array);
		}
		break;
		
		case Tango::DEV_LONG64 :
		{
			std::vector<Tango::DevLong64> long64_array;
			if(has_attr_val == true)
				get_scalar_values_from_attrib_values<Tango::DevLong64>(long64_array,pyData);
			else
				get_array_value<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,long64_array,fwd) :
				group_m->write_attribute_asynch(attr_name,long64_array);
		}
		break;
				
		default :
			return -1;
		break;
		}
	}
	else if (arg_fmt == Tango::SPECTRUM)
	{
		switch (arg_type) {
		case Tango::DEV_SHORT :
		{
			std::vector<std::vector<Tango::DevShort> > short_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			else
				get_spectrum_values<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,short_array,fwd) :
				group_m->write_attribute_asynch(attr_name,short_array);
		}
		break;
	
		case Tango::DEV_LONG :
		{
			std::vector<std::vector<Tango::DevLong> > long_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			else
				get_spectrum_values<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,long_array,fwd) :
				group_m->write_attribute_asynch(attr_name,long_array);
		}
		break;
	
		case Tango::DEV_DOUBLE :
		{
			std::vector<std::vector<Tango::DevDouble> > double_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			else
				get_spectrum_values<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,double_array,fwd) :
				group_m->write_attribute_asynch(attr_name,double_array);
		}
		break;
		
		case Tango::DEV_FLOAT :
		{
			std::vector<std::vector<Tango::DevFloat> > float_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			else
				get_spectrum_values<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,float_array,fwd) :
				group_m->write_attribute_asynch(attr_name,float_array);
		}
		break;
		
		case Tango::DEV_BOOLEAN :
		{
			std::vector<std::vector<Tango::DevBoolean> > bool_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			else
				get_spectrum_values<Tango::DevBoolean>(bool_array,"Tango::DevBoolean",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,bool_array,fwd) :
				group_m->write_attribute_asynch(attr_name,bool_array);
		}
		break;
	
		case Tango::DEV_USHORT :
		{
			std::vector<std::vector<Tango::DevUShort> > ushort_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			else
				get_spectrum_values<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,ushort_array,fwd) :
				group_m->write_attribute_asynch(attr_name,ushort_array);
		}
		break;
	
		case Tango::DEV_STRING :
		{
			TangoSys_OMemStream o;
			o << "Unsupported Python group call for attribute " << attr_name;
			o << "\nwrite_attribute on groups with individualized values not supported string spectrum attributes." << ends;
	
			Tango::Except::throw_exception((const char *)"PyDs_UnsupportedPythonDataFormatForAttribute",
				       o.str(),
				       (const char *)"write_attribute()");			
		}
		break;
		
		case Tango::DEV_UCHAR :
		{
			std::vector<std::vector<Tango::DevUChar> > uchar_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevUChar>(uchar_array,"Tango:DevUChar",pyData);
			else
				get_spectrum_values<Tango::DevUChar>(uchar_array,"Tango:DevUChar",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,uchar_array,fwd) :
				group_m->write_attribute_asynch(attr_name,uchar_array);
		}
		break;
		
		case Tango::DEV_LONG64 :
		{
			std::vector<std::vector<Tango::DevLong64> > long64_array;
			if(has_attr_val == true)
				get_spectrum_values_from_attrib_values<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			else
				get_spectrum_values<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
			return opt_arg ?
				group_m->write_attribute_asynch(attr_name,long64_array,fwd) :
				group_m->write_attribute_asynch(attr_name,long64_array);
		}
		break;
				
		default :
			return -1;
		break;
		}
	}
	return -1;
}

Tango::GroupCmdReplyList 
GroupUtils::apply_command_to_arglist(const std::string &c,boost::python::object pyData,boost::python::object pyFWD,long arg_type)
{
	bool opt_arg = pyFWD.ptr() != Py_None; 
	bool fwd = opt_arg ? pyFWD : true;
		
    switch (arg_type) {
    case Tango::DEV_BOOLEAN :
        {
        	std::vector<bool> bool_array;
        	get_array_value<bool>(bool_array,"Tango::Boolean",pyData);
        	return opt_arg ? 
        		group_m->command_inout(c,bool_array,fwd) :
        		group_m->command_inout(c,bool_array);
		}
	break;
    case Tango::DEV_SHORT :
        {
         	std::vector<Tango::DevShort> short_array;
        	get_array_value<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
        	return opt_arg ? 
        		group_m->command_inout(c,short_array,fwd) :
        		group_m->command_inout(c,short_array);
		}
	break;
    case Tango::DEV_LONG :
        {
         	std::vector<Tango::DevLong> long_array;
        	get_array_value<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,long_array,fwd) :
        		group_m->command_inout(c,long_array);
	}
	break;
    case Tango::DEV_FLOAT :
        {
         	std::vector<Tango::DevFloat> float_array;
        	get_array_value<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,float_array,fwd) :
        		group_m->command_inout(c,float_array);
	}
	break;
    case Tango::DEV_DOUBLE :
        {
         	std::vector<Tango::DevDouble> double_array;
        	get_array_value<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,double_array,fwd) :
        		group_m->command_inout(c,double_array);
	}
	break;
    case Tango::DEV_USHORT :
        {
         	std::vector<Tango::DevUShort> ushort_array;
        	get_array_value<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,ushort_array,fwd) :
        		group_m->command_inout(c,ushort_array);
		}
	break;
    case Tango::DEV_ULONG :
        {
         	std::vector<Tango::DevULong> ulong_array;
        	get_array_value<Tango::DevULong>(ulong_array,"Tango::DevULong",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,ulong_array,fwd) :
        		group_m->command_inout(c,ulong_array);
		}
	break;
    case Tango::DEV_STRING :
        {
         	std::vector<std::string> string_array;
        	get_array_value<std::string>(string_array,"string",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,string_array,fwd) :
        		group_m->command_inout(c,string_array);
	}
	break;
    case Tango::DEV_LONG64 :
        {
         	std::vector<Tango::DevLong64> long64_array;
        	get_array_value<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,long64_array,fwd) :
        		group_m->command_inout(c,long64_array);
	}
	break;
    case Tango::DEV_ULONG64 :
        {
         	std::vector<Tango::DevULong64> ulong64_array;
        	get_array_value<Tango::DevULong64>(ulong64_array,"Tango::DevULong64",pyData);
        	return opt_arg ?
        		group_m->command_inout(c,ulong64_array,fwd) :
        		group_m->command_inout(c,ulong64_array);
		}
	break;	
    case Tango::DEVVAR_CHARARRAY :
        {
           std::vector< std::vector<unsigned char> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<unsigned char> vchar_array;
           		get_array_value<unsigned char>(vchar_array,"unsigned char array",pyData[i]);
                val_array.push_back( vchar_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;
    case Tango::DEVVAR_SHORTARRAY :
        {
           std::vector< std::vector<Tango::DevShort> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevShort> vshort_array;
           		get_array_value<Tango::DevShort>(vshort_array,"Tango::DevShort array",pyData[i]);
                val_array.push_back( vshort_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_LONGARRAY :
        {
           std::vector< std::vector<Tango::DevLong> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevLong> vlong_array;
           		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong array",pyData[i]);
                val_array.push_back( vlong_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_FLOATARRAY :
        {
           std::vector< std::vector<Tango::DevFloat> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevFloat> vfloat_array;
           		get_array_value<Tango::DevFloat>(vfloat_array,"Tango::DevFloat array",pyData[i]);
                val_array.push_back( vfloat_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_DOUBLEARRAY :
        {
           std::vector< std::vector<Tango::DevDouble> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevDouble> vdouble_array;
           		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble array",pyData[i]);
                val_array.push_back( vdouble_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_USHORTARRAY :
        {
           std::vector< std::vector<Tango::DevUShort> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevUShort> vushort_array;
           		get_array_value<Tango::DevUShort>(vushort_array,"Tango::DevUShort array",pyData[i]);
                val_array.push_back( vushort_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_ULONGARRAY :
        {
           std::vector< std::vector<Tango::DevULong> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevULong> vulong_array;
           		get_array_value<Tango::DevULong>(vulong_array,"Tango::DevULong array",pyData[i]);
                val_array.push_back( vulong_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    
    case Tango::DEVVAR_STRINGARRAY :
        {
           std::vector< std::vector<std::string> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<std::string> vstring_array;
           		get_array_value<std::string>(vstring_array,"string array",pyData[i]);
                val_array.push_back( vstring_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;
    case Tango::DEVVAR_LONGSTRINGARRAY :
        {
           std::vector< Tango::DevVarLongStringArray * > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
            	Tango::DevVarLongStringArray *vlong_string_array  = new Tango::DevVarLongStringArray();
           		std::vector<Tango::DevLong> vlong_array;
           		std::vector<std::string> vstr_array;
           		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong list",pyData[i][0]);
           		get_array_value<std::string>(vstr_array,"String list",pyData[i][1]);
           		vlong_string_array->lvalue.length( vlong_array.size() );
           		for (unsigned long il=0 ; il < vlong_array.size() ; il++ )
           		{
           			vlong_string_array->lvalue[il] = vlong_array[il];
           		}
           		vlong_string_array->svalue.length( vstr_array.size() );
           		for (unsigned long is=0 ; is < vstr_array.size() ; is++ )
           		{
           			vlong_string_array->svalue[is] = CORBA::string_dup(vstr_array[is].c_str());
           		}
                val_array.push_back( vlong_string_array );		  		  
    	   }    	   
          return opt_arg ?
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;
    case Tango::DEVVAR_DOUBLESTRINGARRAY :
        {
           std::vector< Tango::DevVarDoubleStringArray * > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
            	Tango::DevVarDoubleStringArray *vdouble_string_array  = new Tango::DevVarDoubleStringArray();
           		std::vector<Tango::DevDouble> vdouble_array;
           		std::vector<std::string> vstr_array;
           		boost::python::tuple pyI = (boost::python::tuple )pyData[i];
           		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble list",pyI[0]);
           		get_array_value<std::string>(vstr_array,"String list",pyI[1]);
           		vdouble_string_array->dvalue.length( vdouble_array.size() );
           		for (unsigned long id=0 ; id < vdouble_array.size() ; id++ )
           		{
           			vdouble_string_array->dvalue[id] = vdouble_array[id];
           		}
           		vdouble_string_array->svalue.length( vstr_array.size() );
           		for (unsigned long is=0 ; is < vstr_array.size() ; is++ )
           		{
           			vdouble_string_array->svalue[is] = CORBA::string_dup(vstr_array[is].c_str());
           		}
                val_array.push_back( vdouble_string_array );		  		  
    	   }    	   
           return opt_arg ? 
           	group_m->command_inout(c,val_array,fwd) :
           	group_m->command_inout(c,val_array);
		}
	break;
    case Tango::DEVVAR_LONG64ARRAY :
        {
           std::vector< std::vector<Tango::DevLong64> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevLong64> vlong64_array;
           		get_array_value<Tango::DevLong64>(vlong64_array,"Tango::DevLong64 array",pyData[i]);
                val_array.push_back( vlong64_array );		  		  
    	   }    	   
          return opt_arg ? 
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;    	
    case Tango::DEVVAR_ULONG64ARRAY :
        {
           std::vector< std::vector<Tango::DevULong64> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevULong64> vulong64_array;
           		get_array_value<Tango::DevULong64>(vulong64_array,"Tango::DevULong array",pyData[i]);
                val_array.push_back( vulong64_array );		  		  
    	   }    	   
          return opt_arg ? 
          	group_m->command_inout(c,val_array,fwd) :
          	group_m->command_inout(c,val_array);
	}
	break;   
     default :
		return Tango::GroupCmdReplyList();
	 break;
    }

}


long 
GroupUtils::apply_command_asynch_to_arglist(const std::string &c,boost::python::object pyData,boost::python::object pyFGT,boost::python::object pyFWD,long arg_type)
{
	bool opt_arg_fgt = pyFGT.ptr() != Py_None; 
	bool fgt = opt_arg_fgt ? pyFGT : false;
	
	bool opt_arg_fwd = pyFWD.ptr() != Py_None; 
	bool fwd = opt_arg_fwd ? pyFWD : true;
	
    switch (arg_type) {
    case Tango::DEV_BOOLEAN :
        {
        	std::vector<bool> bool_array;
        	get_array_value<bool>(bool_array,"Tango::Boolean",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,bool_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,bool_array,fgt)) :
        		group_m->command_inout_asynch(c,bool_array);
		}
	break;
    case Tango::DEV_SHORT :
        {
         	std::vector<Tango::DevShort> short_array;
        	get_array_value<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,short_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,short_array,fgt)) :
        		group_m->command_inout_asynch(c,short_array);
		}
	break;
    case Tango::DEV_LONG :
        {
         	std::vector<Tango::DevLong> long_array;
        	get_array_value<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,long_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,long_array,fgt)) :
        		group_m->command_inout_asynch(c,long_array);
	}
	break;
    case Tango::DEV_FLOAT :
        {
         	std::vector<Tango::DevFloat> float_array;
        	get_array_value<Tango::DevFloat>(float_array,"Tango::DevFloat",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,float_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,float_array,fgt)) :
        		group_m->command_inout_asynch(c,float_array);
	}
	break;
    case Tango::DEV_DOUBLE :
        {
         	std::vector<Tango::DevDouble> double_array;
        	get_array_value<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,double_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,double_array,fgt)) :
        		group_m->command_inout_asynch(c,double_array);
	}
	break;
    case Tango::DEV_USHORT :
        {
         	std::vector<Tango::DevUShort> ushort_array;
        	get_array_value<Tango::DevUShort>(ushort_array,"Tango::DevUShort",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,ushort_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,ushort_array,fgt)) :
        		group_m->command_inout_asynch(c,ushort_array);
		}
	break;
    case Tango::DEV_ULONG :
        {
         	std::vector<Tango::DevULong> ulong_array;
        	get_array_value<Tango::DevULong>(ulong_array,"Tango::DevULong",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,ulong_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,ulong_array,fgt)) :
        		group_m->command_inout_asynch(c,ulong_array);
		}
	break;
    case Tango::DEV_STRING :
        {
         	std::vector<std::string> string_array;
        	get_array_value<std::string>(string_array,"string",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,string_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,string_array,fgt)) :
        		group_m->command_inout_asynch(c,string_array);
	}
	break;
    case Tango::DEV_LONG64 :
        {
         	std::vector<Tango::DevLong64> long64_array;
        	get_array_value<Tango::DevLong64>(long64_array,"Tango::DevLong64",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,long64_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,long64_array,fgt)) :
        		group_m->command_inout_asynch(c,long64_array);
	}
	break;
    case Tango::DEV_ULONG64 :
        {
         	std::vector<Tango::DevULong64> ulong64_array;
        	get_array_value<Tango::DevULong64>(ulong64_array,"Tango::DevULong64",pyData);
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,ulong64_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,ulong64_array,fgt)) :
        		group_m->command_inout_asynch(c,ulong64_array);
		}
	break;	
    case Tango::DEVVAR_CHARARRAY :
        {
           std::vector< std::vector<unsigned char> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<unsigned char> vchar_array;
           		get_array_value<unsigned char>(vchar_array,"unsigned char array",pyData[i]);
                val_array.push_back( vchar_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;
    case Tango::DEVVAR_SHORTARRAY :
        {
           std::vector< std::vector<Tango::DevShort> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevShort> vshort_array;
           		get_array_value<Tango::DevShort>(vshort_array,"Tango::DevShort array",pyData[i]);
                val_array.push_back( vshort_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_LONGARRAY :
        {
           std::vector< std::vector<Tango::DevLong> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevLong> vlong_array;
           		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong array",pyData[i]);
                val_array.push_back( vlong_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_FLOATARRAY :
        {
           std::vector< std::vector<Tango::DevFloat> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevFloat> vfloat_array;
           		get_array_value<Tango::DevFloat>(vfloat_array,"Tango::DevFloat array",pyData[i]);
                val_array.push_back( vfloat_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_DOUBLEARRAY :
        {
           std::vector< std::vector<Tango::DevDouble> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevDouble> vdouble_array;
           		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble array",pyData[i]);
                val_array.push_back( vdouble_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_USHORTARRAY :
        {
           std::vector< std::vector<Tango::DevUShort> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevUShort> vushort_array;
           		get_array_value<Tango::DevUShort>(vushort_array,"Tango::DevUShort array",pyData[i]);
                val_array.push_back( vushort_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_ULONGARRAY :
        {
           std::vector< std::vector<Tango::DevULong> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevULong> vulong_array;
           		get_array_value<Tango::DevULong>(vulong_array,"Tango::DevULong array",pyData[i]);
                val_array.push_back( vulong_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
    case Tango::DEVVAR_STRINGARRAY :
        {
           std::vector< std::vector<std::string> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<std::string> vstring_array;
           		get_array_value<std::string>(vstring_array,"string array",pyData[i]);
                val_array.push_back( vstring_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;
    case Tango::DEVVAR_LONGSTRINGARRAY :
        {
           std::vector< Tango::DevVarLongStringArray * > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
            	Tango::DevVarLongStringArray *vlong_string_array  = new Tango::DevVarLongStringArray();
           		std::vector<Tango::DevLong> vlong_array;
           		std::vector<std::string> vstr_array;
           		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong list",pyData[i][0]);
           		get_array_value<std::string>(vstr_array,"String list",pyData[i][1]);
           		vlong_string_array->lvalue.length( vlong_array.size() );
           		for (unsigned long il=0 ; il < vlong_array.size() ; il++ )
           		{
           			vlong_string_array->lvalue[il] = vlong_array[il];
           		}
           		vlong_string_array->svalue.length( vstr_array.size() );
           		for (unsigned long is=0 ; is < vstr_array.size() ; is++ )
           		{
           			vlong_string_array->svalue[is] = CORBA::string_dup(vstr_array[is].c_str());
           		}
                val_array.push_back( vlong_string_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;
    case Tango::DEVVAR_DOUBLESTRINGARRAY :
        {
           std::vector< Tango::DevVarDoubleStringArray * > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
            	Tango::DevVarDoubleStringArray *vdouble_string_array  = new Tango::DevVarDoubleStringArray();
           		std::vector<Tango::DevDouble> vdouble_array;
           		std::vector<std::string> vstr_array;
           		boost::python::tuple pyI = (boost::python::tuple )pyData[i];
           		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble list",pyI[0]);
           		get_array_value<std::string>(vstr_array,"String list",pyI[1]);
           		vdouble_string_array->dvalue.length( vdouble_array.size() );
           		for (unsigned long id=0 ; id < vdouble_array.size() ; id++ )
           		{
           			vdouble_string_array->dvalue[id] = vdouble_array[id];
           		}
           		vdouble_string_array->svalue.length( vstr_array.size() );
           		for (unsigned long is=0 ; is < vstr_array.size() ; is++ )
           		{
           			vdouble_string_array->svalue[is] = CORBA::string_dup(vstr_array[is].c_str());
           		}
                val_array.push_back( vdouble_string_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
		}
	break;
    case Tango::DEVVAR_LONG64ARRAY :
        {
           std::vector< std::vector<Tango::DevLong64> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevLong64> vlong64_array;
           		get_array_value<Tango::DevLong64>(vlong64_array,"Tango::DevLong64 array",pyData[i]);
                val_array.push_back( vlong64_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break; 
    case Tango::DEVVAR_ULONG64ARRAY :
        {
           std::vector< std::vector<Tango::DevULong64> > val_array;
           int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   for (int i = 0 ; i < arr_len; i++ )
           {
           		std::vector<Tango::DevULong64> vulong64_array;
           		get_array_value<Tango::DevULong64>(vulong64_array,"Tango::DevULong64 array",pyData[i]);
                val_array.push_back( vulong64_array );		  		  
    	   }    	   
        	return opt_arg_fgt ? 
        		(opt_arg_fwd ? 
        			group_m->command_inout_asynch(c,val_array,fgt,fwd) :
        			group_m->command_inout_asynch(c,val_array,fgt)) :
        		group_m->command_inout_asynch(c,val_array);
	}
	break;    
     default :
     	return -1;
	 break;
    }

}

boost::python::list
GroupUtils::translate_readattr_reply_list(Tango::GroupAttrReplyList &groupList) 
{
    boost::python::list pyAttrReplyList;
    std::vector<Tango::GroupAttrReply>::iterator i;
    for (i = groupList.begin() ; i < groupList.end() ; i++ )
    {
       pyAttrReplyList.append( translate_readattr_reply( &(*i) ) );
    }
    return pyAttrReplyList;
    
}

GroupReply
GroupUtils::translate_readattr_reply(Tango::GroupAttrReply *groupAttrReply) 
{
    GroupReply groupReply(groupAttrReply->dev_name(),
                          groupAttrReply->obj_name());
    pair<long,Tango::AttrDataFormat> attr_types = get_attribute_type(groupAttrReply->obj_name());
    boost::python::object result;
    try 
    {
    	result = translate_from_readattr_reply(*groupAttrReply,attr_types.first,attr_types.second);
    	groupReply.set_data(result);
    }
    catch (const Tango::DevFailed& df) {
//    	groupReply.set_exception(get_exception_value(df.errors));
    	groupReply.set_exception(df.errors);
    }
    
    return groupReply;
}

boost::python::object
GroupUtils::translate_from_readattr_reply(Tango::GroupAttrReply &groupAttrReply,long arg_type,Tango::AttrDataFormat arg_format)
{
    boost::python::object def;
    bool extraction_result = false;
    switch (arg_type) {
    case Tango::DEV_SHORT :
        {
           switch (arg_format)  {
	   case Tango::SCALAR :
               {
       	   		 Tango::DevShort val;
       	   		 extraction_result = groupAttrReply >> val;
       	   		 return boost::python::object(val);
	       }
               break;
           default :
               {
       	         std::vector<Tango::DevShort> vshort;
       	         extraction_result = groupAttrReply >> vshort;
	             return boost::python::object(get_list_from_vector<Tango::DevShort>(vshort));
	       }
               break;           
           }
        }
	break;
    case Tango::DEV_LONG :
        {
           switch (arg_format)  {
	   case Tango::SCALAR :
               {
       	         Tango::DevLong val;
       	         extraction_result = groupAttrReply >> val;
       	         return boost::python::object(val);
	       }
               break;
           default :
               {
       	         std::vector<Tango::DevLong> vlong;
       	         extraction_result = groupAttrReply >> vlong;
	             return boost::python::object(get_list_from_vector<Tango::DevLong>(vlong));
	       }
               break;           
           }
        }
	break;
    case Tango::DEV_DOUBLE :
        {
           switch (arg_format)  {
	   case Tango::SCALAR :
               {
       	         Tango::DevDouble val;
       	         extraction_result = groupAttrReply >> val;
       	         return boost::python::object(val);
	       }
               break;
           default :
               {
       	         std::vector<Tango::DevDouble> vdouble;
       	         extraction_result = groupAttrReply >> vdouble;
	             return boost::python::object(get_list_from_vector<Tango::DevDouble>(vdouble));
	       }
               break;           
           }
        }
	break;
    case Tango::DEV_STRING :
        {
       	   std::string val;
       	   extraction_result = groupAttrReply >> val;
       	   return boost::python::object(val);
        }
	break;
    case Tango::DEV_LONG64 :
        {
           switch (arg_format)  {
	   case Tango::SCALAR :
               {
       	         Tango::DevLong64 val;
       	         extraction_result = groupAttrReply >> val;
       	         return boost::python::object(val);
	       }
               break;
           default :
               {
       	         std::vector<Tango::DevLong64> vlong64;
       	         extraction_result = groupAttrReply >> vlong64;
	             return boost::python::object(get_list_from_vector<Tango::DevLong64>(vlong64));
	       }
               break;           
           }
        }
	break;	
    default :
       	extraction_result = false;
	break;
    }
	if (extraction_result == false)
	{
		const Tango::DevErrorList& errors = groupAttrReply.get_err_stack();
		Tango::DevFailed df(errors);
		throw df;
	}
		
    return def;  
}

boost::python::list
GroupUtils::translate_writeattr_reply_list(Tango::GroupReplyList &groupList) 
{
    boost::python::list pyReplyList;
    std::vector<Tango::GroupReply>::iterator i;
    for (i = groupList.begin() ; i < groupList.end() ; i++ )
    {
       pyReplyList.append( translate_writeattr_reply( &(*i) ) );
    }
    return pyReplyList;
    
}

GroupReply
GroupUtils::translate_writeattr_reply(Tango::GroupReply *grpReply) 
{
    GroupReply groupReply(grpReply->dev_name(),
                          grpReply->obj_name());
                          
    if (grpReply->has_failed() == false)
    {
    	groupReply.set_data(boost::python::object());
    } else {
    	groupReply.set_exception(grpReply->get_err_stack());
    }
    
    return groupReply;
}

/*
Tango::GroupReplyList
GroupUtils::apply_writeattr_to_arglist(const std::string &attr,boost::python::object pyData,bool fwd)
{
    pair<long,Tango::AttrDataFormat> attr_types = get_attribute_type(attr);
    long arg_type = attr_types.first;
    Tango::AttrDataFormat arg_format = attr_types.second;
    switch (arg_type) {
    case Tango::DEV_SHORT :
        {
           switch (arg_format)  {
	   	   case Tango::SCALAR :
               {
         			std::vector<Tango::DevShort> short_array;
        			get_array_value<Tango::DevShort>(short_array,"Tango::DevShort",pyData);
        			return group_m->write_attribute(attr,short_array,fwd);
	       	   }
               break;
           default :
               {
           			std::vector< std::vector<Tango::DevShort> > val_array;
           			int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   			for (int i = 0 ; i < arr_len; i++ )
           			{
           				std::vector<Tango::DevShort> vshort_array;
           				get_array_value<Tango::DevShort>(vshort_array,"Tango::DevShort array",pyData[i]);
                		val_array.push_back( vshort_array );		  		  
    	   			}    	   
          			return group_m->write_attribute(attr,val_array,fwd);
	       		}
               break;           
           }
        }
	break;
    case Tango::DEV_LONG :
        {
           switch (arg_format)  {
	   	   case Tango::SCALAR :
               {
         			std::vector<Tango::DevLong> long_array;
        			get_array_value<Tango::DevLong>(long_array,"Tango::DevLong",pyData);
        			return group_m->write_attribute(attr,long_array,fwd);
	       	   }
               break;
           default :
               {
           			std::vector< std::vector<Tango::DevLong> > val_array;
           			int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   			for (int i = 0 ; i < arr_len; i++ )
           			{
           				std::vector<Tango::DevLong> vlong_array;
           				get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong array",pyData[i]);
                		val_array.push_back( vlong_array );		  		  
    	   			}    	   
          			return group_m->write_attribute(attr,val_array,fwd);
	       		}
               break;           
           }
        }
	break;
    case Tango::DEV_DOUBLE :
        {
           switch (arg_format)  {
	   	   case Tango::SCALAR :
               {
         			std::vector<Tango::DevDouble> double_array;
        			get_array_value<Tango::DevDouble>(double_array,"Tango::DevDouble",pyData);
        			return group_m->write_attribute(attr,double_array,fwd);
	       	   }
               break;
           default :
               {
           			std::vector< std::vector<Tango::DevDouble> > val_array;
           			int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
     	   			for (int i = 0 ; i < arr_len; i++ )
           			{
           				std::vector<Tango::DevDouble> vdouble_array;
           				get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble array",pyData[i]);
                		val_array.push_back( vdouble_array );		  		  
    	   			}    	   
          			return group_m->write_attribute(attr,val_array,fwd);
	       		}
               break;           
           }
        }
	break;
    case Tango::DEV_STRING :
        {
           std::vector<std::string> string_array;
           get_array_value<std::string>(string_array,"string",pyData);
           return group_m->write_attribute(attr,string_array,fwd);
        }
	break;
    default :
       	return Tango::GroupReplyList();
	break;
    }
}
*/

