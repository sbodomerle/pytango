#ifndef CONVERSION_UTILS_H
#define CONVERSION_UTILS_H

#include <boost/python.hpp>
#include <vector>
#include <string>
#include "device_proxy.h"

class Utils
{
public:
	static boost::python::tuple translate_exception_value(const Tango::DevErrorList &dev_error_list);
	static boost::python::tuple translate_black_box(std::vector<string>*);
	static void translate_to_device_data(Tango::DeviceData &, boost::python::object, long);
	static boost::python::object translate_from_device_data(Tango::DeviceData &, long);
	
	static DeviceAttributeHistory translate_from_attribute_history(
			Tango::DeviceAttributeHistory &, Tango::AttributeInfoEx *);
	
	static void translate_attribute_info(Tango::AttributeInfo *, AttributeInfo *);
	static AttributeInfo translate_attribute_info(Tango::AttributeInfo *);
	static AttributeInfoEx translate_attribute_info_ex(Tango::AttributeInfoEx *);
	static void translate_to_attribute_info(Tango::AttributeInfo *, AttributeInfo &);
	static Tango::AttributeInfo translate_to_attribute_info(AttributeInfo &);
	static Tango::AttributeInfoEx translate_to_attribute_info_ex(AttributeInfoEx &);
	static boost::python::list translate_attribute_list(Tango::AttributeInfoList *);
	static boost::python::list translate_attribute_list_ex(Tango::AttributeInfoListEx *);
	static DeviceDataHistory translate_from_data_history(Tango::DeviceDataHistory &,long);

	static void translate_from_device_attribute(Tango::DeviceAttribute &, 
			AttributeValue &, Tango::AttributeInfoEx *, bool to_str=false);
	
	static void translate_to_device_attribute(Tango::DeviceAttribute &, 
			AttributeValue &, long, Tango::AttrDataFormat);
	
	static void translate_attribute_alarm_info(Tango::AttributeAlarmInfo &, AttributeAlarmInfo &);
	static void translate_change_event_info(Tango::ChangeEventInfo &, ChangeEventInfo &);
	static void translate_periodic_event_info(Tango::PeriodicEventInfo &, PeriodicEventInfo &);
	static void translate_archive_event_info(Tango::ArchiveEventInfo &, ArchiveEventInfo &);
	static void translate_attribute_event_info(Tango::AttributeEventInfo &, AttributeEventInfo &);
	
	static void translate_to_attribute_alarm_info(Tango::AttributeAlarmInfo &, AttributeAlarmInfo &);
	static void translate_to_change_event_info(Tango::ChangeEventInfo &, ChangeEventInfo &);
	static void translate_to_periodic_event_info(Tango::PeriodicEventInfo &, PeriodicEventInfo &);
	static void translate_to_archive_event_info(Tango::ArchiveEventInfo &, ArchiveEventInfo &);
	static void translate_to_attribute_event_info(Tango::AttributeEventInfo &, AttributeEventInfo &);
};

#define BUILD_STR_FROM_SEQUENCE(ELT_TYPE,PTR) \
{ \
	if (PTR != NULL) \
	{ \
  		unsigned long nb_bytes = PTR->length() * sizeof(ELT_TYPE); \
		ELT_TYPE *co = PTR->get_buffer(); \
		const char *ch_ptr = (const char *)co; \
		boost::python::str py_str(ch_ptr,(size_t)nb_bytes); \
  		delete PTR; \
 		result.value = py_str;	\
    	} \
	else \
	{ \
		boost::python::str py_str; \
    		result.value = py_str; \
	} \
}


#define BUILD_LIST_FROM_SEQUENCE(PTR) \
{ \
    	boost::python::list py_list; \
	if (PTR != NULL) \
	{ \
    		unsigned long nb_data = PTR->length(); \
    		for (unsigned long i = 0 ; i < nb_data ; i++ ) \
       			py_list.append((*PTR)[i]); \
		delete PTR; \
    	} \
    	result.value = py_list; \
}

#define BUILD_STR_FROM_SCALAR(ELT_TYPE,PTR) \
{ \
	const char *ch_ptr = (const char *)PTR; \
	boost::python::str py_str(ch_ptr,sizeof(ELT_TYPE)); \
	result.value = py_str; \
}

#define BUILD_STR_FROM_W_SCALAR(ELT_TYPE,PTR) \
{ \
	const char *ch_ptr = (const char *)PTR; \
	boost::python::str py_str(ch_ptr,sizeof(ELT_TYPE)); \
	result.scalar_w_value = py_str; \
}
				
#endif /* CONVERSION_UTILS_H */
