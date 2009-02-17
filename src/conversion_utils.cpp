#include "conversion_utils.h"
#include <templates.cpp>

boost::python::tuple Utils::translate_exception_value(const Tango::DevErrorList &dev_error_list)
{
    	boost::python::list exc_value;
    	CORBA::ULong i;
    	for (i=0; i < dev_error_list.length() ; i++)
    	{
		boost::python::dict err;
		err["reason"] = dev_error_list[i].reason.in();

        	std::string severity;
        	switch (dev_error_list[i].severity)
		{
			case Tango::WARN :
				severity = "WARNING";
				break;
			case Tango::ERR :
				severity = "ERROR";
				break;
			case Tango::PANIC :
				severity = "PANIC";
				break;
		}
		err["severity"] = severity;
		err["desc"] = dev_error_list[i].desc.in();
		err["origin"] = dev_error_list[i].origin.in();
		exc_value.append(err);
    	}
    	return boost::python::tuple(exc_value);
}

boost::python::tuple Utils::translate_black_box(std::vector<string> *comHist) 
{
	boost::python::list pyComHist = get_list_from_vector<std::string>(*comHist);
	delete comHist;
	return boost::python::tuple(pyComHist);
}

void Utils::translate_to_device_data(Tango::DeviceData &deviceData, boost::python::object pyData, long arg_type)
{
	switch (arg_type) {
	case Tango::DEV_BOOLEAN :
	{
		if ((pyData.attr("__str__")() == "false") || (pyData.attr("__str__")() == "False") || (pyData.attr("__str__")() == "FALSE") || (pyData.attr("__str__")() == "0"))
			deviceData << false;
		else 
			deviceData << true;
		break;
	}
	case Tango::DEV_SHORT :
	{
		Tango::DevShort short_val;
		get_scalar_value<Tango::DevShort>(short_val,"Tango::DevShort",pyData);
		deviceData << short_val;
		break;
	}
	case Tango::DEV_LONG :
	{
		Tango::DevLong long_val;
		get_scalar_value<Tango::DevLong>(long_val,"Tango::DevLong",pyData);
		deviceData << long_val;
		break;
	}
	case Tango::DEV_FLOAT :
	{
		Tango::DevFloat float_val;
		get_scalar_value<Tango::DevFloat>(float_val,"Tango::DevFloat",pyData);
		deviceData << float_val;
		break;
	}
	case Tango::DEV_DOUBLE :
	{
		Tango::DevDouble double_val;
		get_scalar_value<Tango::DevDouble>(double_val,"Tango::DevDouble",pyData);
		deviceData << double_val;
		break;
	}
	case Tango::DEV_USHORT :
	{
		Tango::DevUShort ushort_val;
		get_scalar_value<Tango::DevUShort>(ushort_val,"Tango::DevUShort",pyData);
		deviceData << ushort_val;
		break;
	}
	case Tango::DEV_ULONG :
	{
		Tango::DevULong ulong_val;
		get_scalar_value<Tango::DevULong>(ulong_val,"Tango::DevULong",pyData);
		deviceData << ulong_val;
		break;
	}
	case Tango::DEV_STRING :
	case Tango::CONST_DEV_STRING :
	{
		std::string string_val;
		get_scalar_value<std::string>(string_val,"String",pyData);
		deviceData << string_val;
		break;
	}
	case Tango::DEV_LONG64 :
	{
		Tango::DevLong64 long64_val;
		get_scalar_value<Tango::DevLong64>(long64_val,"Tango::DevLong64",pyData);
		deviceData << long64_val;
		break;
	}
	case Tango::DEV_ULONG64 :
	{
		Tango::DevULong64 ulong64_val;
		get_scalar_value<Tango::DevULong64>(ulong64_val,"Tango::DevULong64",pyData);
		deviceData << ulong64_val;
		break;
	}	
	case Tango::DEV_STATE :
	{
		Tango::DevState state_val;
		boost::python::extract<Tango::DevState> val(pyData);
		if (val.check())
		{
			state_val = (Tango::DevState) val();
			deviceData << state_val;
		}
		else
		{
			Tango::Except::throw_exception((const char *)"PyDs_WrongDatatype",
						       (const char *)"Data coming from Python is not a State",
						       (const char *)"Python_binding::translate_to_device_data()");
		}
		break;
	}
	case Tango::DEVVAR_CHARARRAY :
	{
		std::vector<unsigned char> vchar_array;
		get_array_value<unsigned char>(vchar_array,"unsigned char list",pyData);
		deviceData << vchar_array;
		break;
	}
	case Tango::DEVVAR_SHORTARRAY :
	{	
		std::vector<Tango::DevShort> vshort_array;
		get_array_value<Tango::DevShort>(vshort_array,"Tango::DevShort list",pyData);
		deviceData << vshort_array;
		break;    
	}
	case Tango::DEVVAR_LONGARRAY :
	{
		std::vector<Tango::DevLong> vlong_array;
		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong list",pyData);
		deviceData << vlong_array;
		break;    
	}
	case Tango::DEVVAR_FLOATARRAY :
	{
		std::vector<Tango::DevFloat> vfloat_array;
		get_array_value<Tango::DevFloat>(vfloat_array,"Tango::DevFloat list",pyData);
		deviceData << vfloat_array;
		break;    
	}
	case Tango::DEVVAR_DOUBLEARRAY :
	{
		std::vector<Tango::DevDouble> vdouble_array;
		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble list",pyData);
		deviceData << vdouble_array;
		break;    
	}
	case Tango::DEVVAR_USHORTARRAY :
	{
		std::vector<Tango::DevUShort> vushort_array;
		get_array_value<Tango::DevUShort>(vushort_array,"Tango::DevUShort list",pyData);
		deviceData << vushort_array;
		break;    
	}
	case Tango::DEVVAR_ULONGARRAY :
	{
		std::vector<Tango::DevULong> vulong_array;
		get_array_value<Tango::DevULong>(vulong_array,"Tango::DevULong list",pyData);
		deviceData << vulong_array;
		break;    
	}
	case Tango::DEVVAR_STRINGARRAY :
	{
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String list",pyData);
		deviceData << vstr_array;
		break;
	}
	case Tango::DEVVAR_LONGSTRINGARRAY :
	{
		std::vector<Tango::DevLong> vlong_array;
		std::vector<std::string> vstr_array;
		get_array_value<Tango::DevLong>(vlong_array,"Tango::DevLong list",pyData[0]);
		get_array_value<std::string>(vstr_array,"String list",pyData[1]);
		deviceData.insert(vlong_array,vstr_array);
		break;
	}
	case Tango::DEVVAR_DOUBLESTRINGARRAY :
	{
		std::vector<Tango::DevDouble> vdouble_array;
		std::vector<std::string> vstr_array;
		get_array_value<Tango::DevDouble>(vdouble_array,"Tango::DevDouble list",pyData[0]);
		get_array_value<std::string>(vstr_array,"String list",pyData[1]);
		deviceData.insert(vdouble_array,vstr_array);
		break;
	}
	case Tango::DEVVAR_LONG64ARRAY :
	{
		std::vector<Tango::DevLong64> vlong64_array;
		get_array_value<Tango::DevLong64>(vlong64_array,"Tango::DevLong64 list",pyData);
		deviceData << vlong64_array;
		break;    
	}
	case Tango::DEVVAR_ULONG64ARRAY :
	{
		std::vector<Tango::DevULong64> vulong64_array;
		get_array_value<Tango::DevULong64>(vulong64_array,"Tango::DevULong64 list",pyData);
		deviceData << vulong64_array;
		break;    
	}	
	default :
		break;
	}
}
	
boost::python::object Utils::translate_from_device_data(Tango::DeviceData &deviceData, long arg_type)
{
	boost::python::object def ;
	switch (arg_type) {
	case Tango::DEV_BOOLEAN :
	{
		Tango::DevBoolean val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_SHORT :
	{
		Tango::DevShort val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_LONG :
	{
		Tango::DevLong val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_FLOAT :
	{	Tango::DevFloat val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_DOUBLE :
	{
		Tango::DevDouble val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_USHORT :
	{
		Tango::DevUShort val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_ULONG :
	{
		Tango::DevULong val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::CONST_DEV_STRING :
	case Tango::DEV_STRING :
	{
		std::string val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_LONG64 :
	{
		Tango::DevLong64 val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}
	case Tango::DEV_ULONG64 :
	{
		Tango::DevULong64 val;
		deviceData >> val;
		return boost::python::object(val);
		break;
	}	
	/* VF */
	case Tango::DEV_STATE:
	{
		Tango::DevState dev_state;
		deviceData >> dev_state;
		return boost::python::object(dev_state);
		break;
	}
	case Tango::DEVVAR_CHARARRAY :
	{
		const Tango::DevVarCharArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarCharArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_SHORTARRAY :
	{
		const Tango::DevVarShortArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarShortArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_LONGARRAY :
	{
		const Tango::DevVarLongArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarLongArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_FLOATARRAY :
	{
		const Tango::DevVarFloatArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarFloatArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_DOUBLEARRAY :
	{
		const Tango::DevVarDoubleArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarDoubleArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_USHORTARRAY :
	{
		const Tango::DevVarUShortArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarUShortArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_ULONGARRAY :
	{
		const Tango::DevVarULongArray *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarULongArray>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_STRINGARRAY :
	{
		std::vector<std::string> vstring;
		deviceData >> vstring;
		return get_list_from_vector<std::string>(vstring);
		break;
	}
	case Tango::DEVVAR_LONGSTRINGARRAY :
	{
		const Tango::DevVarLongStringArray *tmp_ptr;
		deviceData >> tmp_ptr;
		boost::python::list l_val = get_list_from_const_sequence<Tango::DevVarLongArray>(&(tmp_ptr->lvalue));
		boost::python::list s_val = get_list_from_const_sequence<Tango::DevVarStringArray>(&(tmp_ptr->svalue));
		return boost::python::make_tuple(l_val,s_val);
		break;
	}
	case Tango::DEVVAR_DOUBLESTRINGARRAY :
	{
		const Tango::DevVarDoubleStringArray *tmp_ptr;
		deviceData >> tmp_ptr;
		boost::python::list d_val = get_list_from_const_sequence<Tango::DevVarDoubleArray>(&(tmp_ptr->dvalue));
		boost::python::list s_val = get_list_from_const_sequence<Tango::DevVarStringArray>(&(tmp_ptr->svalue));
		return boost::python::make_tuple(d_val,s_val);
		break;
	}
	case Tango::DEVVAR_LONG64ARRAY :
	{
		const Tango::DevVarLong64Array *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarLong64Array>(tmp_ptr);
		break;
	}
	case Tango::DEVVAR_ULONG64ARRAY :
	{
		const Tango::DevVarULong64Array *tmp_ptr;
		deviceData >> tmp_ptr;
		return get_list_from_const_sequence<Tango::DevVarULong64Array>(tmp_ptr);
		break;
	}	
	default :
		break;
	}
	return def;  
}

DeviceDataHistory Utils::translate_from_data_history(Tango::DeviceDataHistory &deviceDataHistory,long arg_type)
{
	DeviceDataHistory py_data_history ;
	py_data_history.time = deviceDataHistory.date();
	boost::python::object value;
	boost::python::list errors;
	if (deviceDataHistory.failed()) {
		py_data_history.cmd_failed = 1;
		const Tango::DevErrorList &dev_error_list = deviceDataHistory.errors();
		CORBA::ULong i;
		for (i=0; i < dev_error_list.length() ; i++)
		{
			errors.append( dev_error_list[i] );
		}
	} else {
		py_data_history.cmd_failed = 0;
		value = translate_from_device_data(deviceDataHistory,arg_type);
	}
	py_data_history.value = value;
	py_data_history.errors = errors;
	return py_data_history;
}

DeviceAttributeHistory Utils::translate_from_attribute_history(
		Tango::DeviceAttributeHistory &deviceAttrHistory, 
		Tango::AttributeInfoEx *attr_info)
{
	DeviceAttributeHistory py_attr_history ;
	AttributeValue value;
	boost::python::list errors;
	if (deviceAttrHistory.failed()) 
	{
		py_attr_history.attr_failed = 1;
		/* commented out to compile in Tango V5: check */
		const Tango::DevErrorList &dev_error_list = deviceAttrHistory.get_err_stack();
		CORBA::ULong i;
		for (i=0; i < dev_error_list.length() ; i++)
		{
			errors.append( dev_error_list[i] );
		}
	} 
	else 
	{
		py_attr_history.attr_failed = 0;
		translate_from_device_attribute(deviceAttrHistory, value, attr_info);
	}
	py_attr_history.value = value;
	py_attr_history.errors = errors;
	return py_attr_history;
}

void Utils::translate_attribute_info(Tango::AttributeInfo *attributeInfo, AttributeInfo *out_attr)
{
	out_attr->name = attributeInfo->name;
	out_attr->writable = attributeInfo->writable;
	out_attr->data_format = attributeInfo->data_format;
	out_attr->data_type = attributeInfo->data_type;
	out_attr->max_dim_x = attributeInfo->max_dim_x;
	out_attr->max_dim_y = attributeInfo->max_dim_y;
	out_attr->description = attributeInfo->description;
	out_attr->label = attributeInfo->label;
	out_attr->unit = attributeInfo->unit;
	out_attr->standard_unit = attributeInfo->standard_unit;
	out_attr->display_unit = attributeInfo->display_unit;
	out_attr->format = attributeInfo->format;
	out_attr->min_value = attributeInfo->min_value;
	out_attr->max_value = attributeInfo->max_value;
	out_attr->min_alarm = attributeInfo->min_alarm;
	out_attr->max_alarm = attributeInfo->max_alarm;
	out_attr->writable_attr_name = attributeInfo->writable_attr_name;
	out_attr->extensions = get_list_from_vector<std::string>(attributeInfo->extensions);
	out_attr->disp_level = attributeInfo->disp_level;
}

AttributeInfo Utils::translate_attribute_info(Tango::AttributeInfo *attributeInfo)
{
	AttributeInfo pyAttributeInfo;
	translate_attribute_info(attributeInfo,&pyAttributeInfo);
	return pyAttributeInfo;
}

AttributeInfoEx Utils::translate_attribute_info_ex(Tango::AttributeInfoEx *attributeInfo) 
{
	AttributeInfoEx pyAttributeInfo;
	
	translate_attribute_info(attributeInfo, &pyAttributeInfo);
	translate_attribute_alarm_info(attributeInfo->alarms,pyAttributeInfo.alarms);
	translate_attribute_event_info(attributeInfo->events,pyAttributeInfo.events);
	
	pyAttributeInfo.sys_extensions = get_list_from_vector<std::string>(attributeInfo->sys_extensions);
	
	return pyAttributeInfo;
}

void Utils::translate_to_attribute_info(Tango::AttributeInfo *tgAttributeInfo, AttributeInfo &pyAttributeInfo) 
{
	get_scalar_value<std::string>(tgAttributeInfo->name,"String",
			(boost::python::object)(pyAttributeInfo.name));
	tgAttributeInfo->writable = pyAttributeInfo.writable;
	tgAttributeInfo->data_format = pyAttributeInfo.data_format;
	get_scalar_value<int>(tgAttributeInfo->data_type,"int",
			(boost::python::object)(pyAttributeInfo.data_type));
	get_scalar_value<int>(tgAttributeInfo->max_dim_x,"int",
			(boost::python::object)(pyAttributeInfo.max_dim_x));
	get_scalar_value<int>(tgAttributeInfo->max_dim_y,"int",
			(boost::python::object)(pyAttributeInfo.max_dim_y));
	get_scalar_value<std::string>(tgAttributeInfo->description,"String",
			(boost::python::object)(pyAttributeInfo.description));
	get_scalar_value<std::string>(tgAttributeInfo->label,"String",
			(boost::python::object)(pyAttributeInfo.label));
	get_scalar_value<std::string>(tgAttributeInfo->unit,"String",
			(boost::python::object)(pyAttributeInfo.unit));
	get_scalar_value<std::string>(tgAttributeInfo->standard_unit,"String",
			(boost::python::object)(pyAttributeInfo.standard_unit));
	get_scalar_value<std::string>(tgAttributeInfo->display_unit,"String",
			(boost::python::object)(pyAttributeInfo.display_unit));
	get_scalar_value<std::string>(tgAttributeInfo->format,"String",
			(boost::python::object)(pyAttributeInfo.format));
	get_scalar_value<std::string>(tgAttributeInfo->min_value,"String",
			(boost::python::object)(pyAttributeInfo.min_value));
	get_scalar_value<std::string>(tgAttributeInfo->max_value,"String",
			(boost::python::object)(pyAttributeInfo.max_value));
	get_scalar_value<std::string>(tgAttributeInfo->min_alarm,"String",
			(boost::python::object)(pyAttributeInfo.min_alarm));
	get_scalar_value<std::string>(tgAttributeInfo->max_alarm,"String",
			(boost::python::object)(pyAttributeInfo.max_alarm));
	get_scalar_value<std::string>(tgAttributeInfo->writable_attr_name,"String",
			(boost::python::object)(pyAttributeInfo.writable_attr_name));
	boost::python::list extensions = pyAttributeInfo.extensions;
	get_array_value<std::string>(tgAttributeInfo->extensions,"String list",
			(boost::python::object)extensions);
	tgAttributeInfo->disp_level = pyAttributeInfo.disp_level;
}

Tango::AttributeInfo Utils::translate_to_attribute_info(AttributeInfo &pyAttributeInfo) 
{
	Tango::AttributeInfo attributeInfo;
	translate_to_attribute_info(&attributeInfo, pyAttributeInfo);
	return attributeInfo;
}

Tango::AttributeInfoEx Utils::translate_to_attribute_info_ex(AttributeInfoEx &pyAttributeInfo) 
{
	Tango::AttributeInfoEx attributeInfo;
	translate_to_attribute_info(&attributeInfo, pyAttributeInfo);
	translate_to_attribute_alarm_info(attributeInfo.alarms, pyAttributeInfo.alarms);
	translate_to_attribute_event_info(attributeInfo.events, pyAttributeInfo.events);
	get_array_value<std::string>(attributeInfo.extensions,"String list",
			(boost::python::object)pyAttributeInfo.sys_extensions);
	return attributeInfo;
}

boost::python::list Utils::translate_attribute_list(Tango::AttributeInfoList *attrHist) 
{
	boost::python::list pyAttrHist;
	std::vector<Tango::AttributeInfo>::iterator i;
	for (i = attrHist->begin() ; i < attrHist->end() ; i++ )
		pyAttrHist.append( translate_attribute_info( &(*i) ) );
	delete attrHist;
	return pyAttrHist;
}

boost::python::list Utils::translate_attribute_list_ex(Tango::AttributeInfoListEx *attrHist) 
{
	boost::python::list pyAttrHist;
	std::vector<Tango::AttributeInfoEx>::iterator i;
	for (i = attrHist->begin() ; i < attrHist->end() ; i++ )
		pyAttrHist.append( translate_attribute_info_ex( &(*i) ) );
	delete attrHist;
	return pyAttrHist;
}

void Utils::translate_from_device_attribute(Tango::DeviceAttribute &deviceAttr,
		AttributeValue& result, Tango::AttributeInfoEx *attr_info, bool to_str)
{
	assert(attr_info != NULL);
	
	int arg_type = attr_info->data_type;
	Tango::AttrDataFormat arg_format = attr_info->data_format;
	bool writable = attr_info->writable != Tango::READ;
	
	result.quality = deviceAttr.quality;
	result.time = deviceAttr.time;
	result.name = deviceAttr.name;
	result.dim_x = deviceAttr.dim_x;
	result.dim_y = deviceAttr.dim_y;
	result.w_dim_x = deviceAttr.get_written_dim_x();
	result.w_dim_y = deviceAttr.get_written_dim_y();
	
	switch (arg_type) 
	{
		case Tango::DEV_STATE:
		{
		 	if (to_str == true)
			{
				TangoSys_OMemStream o;
				o << "Not possible to call read_attribute_as_str() when the "
				     "attribute is a DevState. Use read_attribute()" << ends;
				
				Tango::Except::throw_exception(
						(const char *)"PyAPI_WrongDataType", o.str(), 
						(const char *)"Utils::translate_from_device_attribute");
			}
			
			switch (arg_format) 
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevState> val;
						deviceAttr >> val;
						result.value = (boost::python::object) val[0];
						result.scalar_w_value = (boost::python::object) val[1];
					}
					else
					{
						Tango::DevState val;
						deviceAttr >> val;
						result.value = (boost::python::object) val;
					}
					break;
				}
				default :
				{
					Tango::DevVarStateArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
			
		}
		case Tango::DEV_SHORT :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR :
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevShort> val;
						deviceAttr >> val;
						Tango::DevShort read_val = val[0];
						Tango::DevShort write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevShort, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevShort, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevShort read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevShort, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;
				}
				default :
				{
					Tango::DevVarShortArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevShort,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_LONG :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR :
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevLong> val;
						deviceAttr >> val;
						Tango::DevLong read_val = val[0];
						Tango::DevLong write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevLong, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevLong, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevLong read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevLong, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;
				}
				default :
				{
					Tango::DevVarLongArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevLong,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_DOUBLE :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR :
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevDouble> val;
						deviceAttr >> val;
						Tango::DevDouble read_val = val[0];
						Tango::DevDouble write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevDouble, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevDouble, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevDouble read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevDouble, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;			
				}
				default :
				{
					Tango::DevVarDoubleArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevDouble,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_FLOAT :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevFloat> val;
						deviceAttr >> val;
						Tango::DevFloat read_val = val[0];
						Tango::DevFloat write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevFloat, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevFloat, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevFloat read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevFloat, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;	
				}
				default:
				{
					Tango::DevVarFloatArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevFloat,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_BOOLEAN :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevBoolean> val;
						deviceAttr >> val;
						Tango::DevBoolean read_val = val[0];
						Tango::DevBoolean write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevBoolean, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevBoolean, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevBoolean read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevBoolean, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;	
				}
				default:
				{	
					Tango::DevVarBooleanArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevBoolean,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_USHORT :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevUShort> val;
						deviceAttr >> val;
						Tango::DevUShort read_val = val[0];
						Tango::DevUShort write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevUShort, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevUShort, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevUShort read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevUShort, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;						
				}
				default:
				{
					Tango::DevVarUShortArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevUShort,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_ULONG :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevULong> val;
						deviceAttr >> val;
						Tango::DevULong read_val = val[0];
						Tango::DevULong write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevULong, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevULong, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevULong read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevULong, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;						
				}
				default:
				{
					Tango::DevVarULongArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevULong,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_STRING :
		{
		 	if (to_str == true)
			{
				TangoSys_OMemStream o;
				o << "Not possible to call read_attribute_as_str() when the "
				     "attribute is already a string. Use read_attribute()" << ends;
				
				Tango::Except::throw_exception(
						(const char *)"PyAPI_WrongDataType",o.str(),
						(const char *)"Utils::translate_from_device_attribute");
			}
			
			switch (arg_format) 
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<std::string> val;
						deviceAttr >> val;
						result.value = (boost::python::object) val[0];
						result.scalar_w_value = (boost::python::object) val[1];
					}
					else
					{
						std::string val;
						deviceAttr >> val;
						result.value = (boost::python::object) val;
					}
					break;
				}
				default :
				{
					std::vector<std::string> vstring;
					try
					{
						deviceAttr >> vstring;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					result.value = (boost::python::object)get_list_from_vector<std::string>(vstring);
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_UCHAR :
		{
			switch (arg_format) 
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevUChar> val;
						deviceAttr >> val;
						Tango::DevUChar read_val = val[0];
						Tango::DevUChar write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevUChar, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevUChar, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevUChar read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevUChar, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;						
				}
				default :
				{			
					Tango::DevVarCharArray *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevUChar,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
		
					break;        
				}
			}
			break;
		}
		
		case Tango::DEV_LONG64 :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR :
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevLong64> val;
						deviceAttr >> val;
						Tango::DevLong64 read_val = val[0];
						Tango::DevLong64 write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevLong64, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevLong64, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevLong64 read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevLong64, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;	
				}
				default :
				{
					Tango::DevVarLong64Array *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevLong64,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		case Tango::DEV_ULONG64 :
		{
			switch (arg_format)  
			{
				case Tango::SCALAR:
				{
					if(writable && result.w_dim_x > 0)
					{
						vector<Tango::DevULong64> val;
						deviceAttr >> val;
						Tango::DevULong64 read_val = val[0];
						Tango::DevULong64 write_val = val[1];
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevULong64, &read_val)
							BUILD_STR_FROM_W_SCALAR(Tango::DevULong64, &write_val)
						}							
						else
						{
							result.value = (boost::python::object)read_val;
							result.scalar_w_value = (boost::python::object)write_val;
						}
					}
					else
					{
						Tango::DevULong64 read_val;
						deviceAttr >> read_val;
						if (to_str == true)
						{
							BUILD_STR_FROM_SCALAR(Tango::DevULong64, &read_val)
						}
						else
						{
							result.value = (boost::python::object)read_val;
						}
					}
					break;
				}
				default:
				{
					Tango::DevVarULong64Array *tmp_ptr = NULL;
					try
					{
						deviceAttr >> tmp_ptr;
					}
					catch (Tango::DevFailed &e)
					{
						if (strcmp(e.errors[0].reason.in(),"API_EmptyDeviceAttribute") != 0)
							throw;
					}
					if (to_str == true)
						BUILD_STR_FROM_SEQUENCE(Tango::DevULong64,tmp_ptr)
					else
						BUILD_LIST_FROM_SEQUENCE(tmp_ptr)
					break;           
				}
			}
			break;
		}
		
		default :
		{
			result.value = boost::python::object();
			break;
		}
	}

}

void Utils::translate_to_device_attribute(Tango::DeviceAttribute &deviceAttr, 
		AttributeValue &attr_val, long arg_type, 
		Tango::AttrDataFormat arg_format)
{
	std::string name = attr_val.name;
	//    get_scalar_value<std::string>(name,"String",attr_val[0]);
	deviceAttr.set_name(name);
	deviceAttr.quality = attr_val.quality;
	deviceAttr.time = attr_val.time;
	deviceAttr.name = attr_val.name;
	deviceAttr.dim_x = attr_val.dim_x;
	deviceAttr.dim_y = attr_val.dim_y;
	switch (arg_type) {
	case Tango::DEV_SHORT :
		switch (arg_format)  {
		case Tango::SCALAR :
		{
			Tango::DevShort val;
			get_scalar_value<Tango::DevShort>(val,"Tango::DevShort",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevShort> vshort;
			get_array_value<Tango::DevShort>(vshort,"Tango::DevShort",attr_val.value);
			deviceAttr.insert(vshort, attr_val.dim_x, attr_val.dim_y);
			break;           
		}
		default :
		{
			std::vector<Tango::DevShort> vshort;
			get_array_value<Tango::DevShort>(vshort,"Tango::DevShort",attr_val.value);
			deviceAttr << vshort;
			break;           
		}
		}
		break;
		
	case Tango::DEV_LONG :
		switch (arg_format)  {
		case Tango::SCALAR :
		{
			Tango::DevLong val;
			get_scalar_value<Tango::DevLong>(val,"Tango::DevLong",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevLong> vlong;
			get_array_value<Tango::DevLong>(vlong,"Tango::DevLong",attr_val.value);
			deviceAttr.insert(vlong, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default :
		{
			std::vector<Tango::DevLong> vlong;
			get_array_value<Tango::DevLong>(vlong,"Tango::DevLong",attr_val.value);
			deviceAttr << vlong;
			break;           
		}
		}
		break;
		
	case Tango::DEV_DOUBLE :
		switch (arg_format)  {
		case Tango::SCALAR :
		{
			Tango::DevDouble val;
			get_scalar_value<Tango::DevDouble>(val,"Tango::DevDouble",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevDouble> vdouble;
			get_array_value<Tango::DevDouble>(vdouble,"Tango::DevDouble",attr_val.value);
			deviceAttr.insert(vdouble, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default :
		{
			std::vector<Tango::DevDouble> vdouble;
			get_array_value<Tango::DevDouble>(vdouble,"Tango::DevDouble",attr_val.value);
			deviceAttr << vdouble;
			break;           
		}
		}
		break;
		
	case Tango::DEV_FLOAT :
		switch (arg_format) {
		case Tango::SCALAR:
		{
			Tango::DevFloat val;
			get_scalar_value<Tango::DevFloat>(val,"Tango::DevFloat",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevFloat> vfloat;
			get_array_value<Tango::DevFloat>(vfloat,"Tango::DevFloat",attr_val.value);
			deviceAttr.insert(vfloat, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default:
		{
			std::vector<Tango::DevFloat> vfloat;
			get_array_value<Tango::DevFloat>(vfloat,"Tango::DevFloat",attr_val.value);
			deviceAttr << vfloat;
			break;           
		}
		}
		break;
		
	case Tango::DEV_BOOLEAN :
		switch (arg_format) { 
		case Tango::SCALAR:
		{
			Tango::DevBoolean val;
			get_scalar_value<Tango::DevBoolean>(val,"Tango::DevBoolean",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevBoolean> vbool;
			get_array_value<Tango::DevBoolean>(vbool,"Tango::DevBoolean",attr_val.value);
			deviceAttr.insert(vbool, attr_val.dim_x, attr_val.dim_y);
			break;           
		}
		default:
		{
			std::vector<Tango::DevBoolean> vbool;
			get_array_value<Tango::DevBoolean>(vbool,"Tango::DevBoolean",attr_val.value);
			deviceAttr << vbool;
			break;           
		}
		}
		break;
		
	case Tango::DEV_USHORT :
		switch (arg_format){
		case Tango::SCALAR:
		{
			Tango::DevUShort val;
			get_scalar_value<Tango::DevUShort>(val,"Tango::DevUShort",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevUShort> vushort;
			get_array_value<Tango::DevUShort>(vushort,"Tango::DevUShort",attr_val.value);
			deviceAttr.insert(vushort, attr_val.dim_x, attr_val.dim_y);
			break;           
		}
		default:
		{
			std::vector<Tango::DevUShort> vushort;
			get_array_value<Tango::DevUShort>(vushort,"Tango::DevUShort",attr_val.value);
			deviceAttr << vushort;
			break;           
		}
		}
		break;
		
	case Tango::DEV_ULONG :
		switch (arg_format){
		case Tango::SCALAR:
		{
			Tango::DevLong val;
			get_scalar_value<Tango::DevLong>(val,"Tango::DevLong",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevLong> vlong;
			get_array_value<Tango::DevLong>(vlong,"Tango::DevLong",attr_val.value);
			deviceAttr.insert(vlong, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default:
		{
			std::vector<Tango::DevLong> vlong;
			get_array_value<Tango::DevLong>(vlong,"Tango::DevLong",attr_val.value);
			deviceAttr << vlong;
			break;           
		}
		}
		break;
		
	case Tango::DEV_STRING :
		switch (arg_format){
		case Tango::SCALAR:
		{
			std::string val;
			get_scalar_value<std::string>(val,"String",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<std::string> vstr;
			get_array_value<std::string>(vstr,"String",attr_val.value);
			deviceAttr.insert(vstr, attr_val.dim_x, attr_val.dim_y);
			break;           
		}
		default:
		{
			std::vector<std::string> vstr;
			get_array_value<std::string>(vstr,"String",attr_val.value);
			deviceAttr << vstr;
			break;           
		}
		}
		break;
		
	case Tango::DEV_LONG64 :
		switch (arg_format)  {
		case Tango::SCALAR :
		{
			Tango::DevLong64 val;
			get_scalar_value<Tango::DevLong64>(val,"Tango::DevLong64",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevLong64> vlong64;
			get_array_value<Tango::DevLong64>(vlong64,"Tango::DevLong64",attr_val.value);
			deviceAttr.insert(vlong64, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default :
		{
			std::vector<Tango::DevLong64> vlong64;
			get_array_value<Tango::DevLong64>(vlong64,"Tango::DevLong64",attr_val.value);
			deviceAttr << vlong64;
			break;           
		}
		}
		break;

	case Tango::DEV_ULONG64 :
		switch (arg_format){
		case Tango::SCALAR:
		{
			Tango::DevLong64 val;
			get_scalar_value<Tango::DevLong64>(val,"Tango::DevLong64",attr_val.value);
			deviceAttr << val;
			break;
		}
		case Tango::IMAGE :
		{
			std::vector<Tango::DevLong64> vlong64;
			get_array_value<Tango::DevLong64>(vlong64,"Tango::DevLong64",attr_val.value);
			deviceAttr.insert(vlong64, attr_val.dim_x, attr_val.dim_y);
			break;           
		}		
		default:
		{
			std::vector<Tango::DevLong64> vlong64;
			get_array_value<Tango::DevLong64>(vlong64,"Tango::DevLong64",attr_val.value);
			deviceAttr << vlong64;
			break;           
		}
		}
		break;
				
	default :
		break;
	}
}

void Utils::translate_attribute_alarm_info(Tango::AttributeAlarmInfo &tg_alarm_info, AttributeAlarmInfo &alarm_info)
{
	alarm_info.min_alarm = tg_alarm_info.min_alarm;
	alarm_info.max_alarm = tg_alarm_info.max_alarm;
	alarm_info.min_warning = tg_alarm_info.min_warning;
	alarm_info.max_warning = tg_alarm_info.max_warning;
	alarm_info.delta_t = tg_alarm_info.delta_t;
	alarm_info.delta_val = tg_alarm_info.delta_val;
	alarm_info.extensions = get_list_from_vector<std::string>(tg_alarm_info.extensions);
}

void Utils::translate_change_event_info(Tango::ChangeEventInfo &tg_ch_evt_info, ChangeEventInfo &ch_evt_info)
{
	ch_evt_info.rel_change = tg_ch_evt_info.rel_change;
	ch_evt_info.abs_change = tg_ch_evt_info.abs_change;
	ch_evt_info.extensions = get_list_from_vector<std::string>(tg_ch_evt_info.extensions);	
}

void Utils::translate_periodic_event_info(Tango::PeriodicEventInfo &tg_prd_evt_info, PeriodicEventInfo &prd_evt_info)
{
	prd_evt_info.period = tg_prd_evt_info.period;
	prd_evt_info.extensions = get_list_from_vector<std::string>(tg_prd_evt_info.extensions);
}

void Utils::translate_archive_event_info(Tango::ArchiveEventInfo &tg_arch_evt_info, ArchiveEventInfo &arch_evt_info)
{
	arch_evt_info.archive_rel_change = tg_arch_evt_info.archive_rel_change;
	arch_evt_info.archive_abs_change = tg_arch_evt_info.archive_abs_change;
	arch_evt_info.archive_period = tg_arch_evt_info.archive_period;
	arch_evt_info.extensions = get_list_from_vector<std::string>(tg_arch_evt_info.extensions);;
}

void Utils::translate_attribute_event_info(Tango::AttributeEventInfo &tg_attr_evt_info, AttributeEventInfo &attr_evt_info)
{
	translate_change_event_info(tg_attr_evt_info.ch_event,attr_evt_info.ch_event);
	translate_periodic_event_info(tg_attr_evt_info.per_event,attr_evt_info.per_event);
	translate_archive_event_info(tg_attr_evt_info.arch_event,attr_evt_info.arch_event);
}

void Utils::translate_to_attribute_alarm_info(Tango::AttributeAlarmInfo &tg_alarm_info, AttributeAlarmInfo &py_alarm_info)
{
	get_scalar_value<std::string>(tg_alarm_info.min_alarm,"String",
			(boost::python::object)(py_alarm_info.min_alarm));
	get_scalar_value<std::string>(tg_alarm_info.max_alarm,"String",
			(boost::python::object)(py_alarm_info.max_alarm));
	get_scalar_value<std::string>(tg_alarm_info.min_warning,"String",
			(boost::python::object)(py_alarm_info.min_warning));
	get_scalar_value<std::string>(tg_alarm_info.max_warning,"String",
			(boost::python::object)(py_alarm_info.max_warning));
	get_scalar_value<std::string>(tg_alarm_info.delta_t,"String",
			(boost::python::object)(py_alarm_info.delta_t));
	get_scalar_value<std::string>(tg_alarm_info.delta_val,"String",
			(boost::python::object)(py_alarm_info.delta_val));
	get_scalar_value<std::string>(tg_alarm_info.delta_t,"String",
			(boost::python::object)(py_alarm_info.delta_t));
	get_array_value<std::string>(tg_alarm_info.extensions,"String list",
			(boost::python::object)py_alarm_info.extensions);
}

void Utils::translate_to_change_event_info(Tango::ChangeEventInfo &tg_change_info, ChangeEventInfo &py_change_info)
{
	get_scalar_value<std::string>(tg_change_info.rel_change,"String",
			(boost::python::object)py_change_info.rel_change);
	get_scalar_value<std::string>(tg_change_info.abs_change,"String",
			(boost::python::object)py_change_info.abs_change);
	get_array_value<std::string>(tg_change_info.extensions,"String list",
			(boost::python::object)py_change_info.extensions);
}

void Utils::translate_to_periodic_event_info(Tango::PeriodicEventInfo &tg_per_info, PeriodicEventInfo &py_per_info)
{
	get_scalar_value<std::string>(tg_per_info.period,"String",
			(boost::python::object)py_per_info.period);
	get_array_value<std::string>(tg_per_info.extensions,"String list",
			(boost::python::object)py_per_info.extensions);
}

void Utils::translate_to_archive_event_info(Tango::ArchiveEventInfo &tg_arch_info, ArchiveEventInfo &py_arch_info)
{
	get_scalar_value<std::string>(tg_arch_info.archive_rel_change,"String",
			(boost::python::object)py_arch_info.archive_rel_change);
	get_scalar_value<std::string>(tg_arch_info.archive_abs_change,"String",
			(boost::python::object)py_arch_info.archive_abs_change);
	get_scalar_value<std::string>(tg_arch_info.archive_period,"String",
			(boost::python::object)py_arch_info.archive_period);
	get_array_value<std::string>(tg_arch_info.extensions,"String list",
			(boost::python::object)py_arch_info.extensions);
}

void Utils::translate_to_attribute_event_info(Tango::AttributeEventInfo &tg_evt_info, AttributeEventInfo &py_evt_info)
{
	translate_to_change_event_info(tg_evt_info.ch_event,py_evt_info.ch_event);
	translate_to_periodic_event_info(tg_evt_info.per_event,py_evt_info.per_event);
	translate_to_archive_event_info(tg_evt_info.arch_event,py_evt_info.arch_event);
}




