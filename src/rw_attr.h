#ifndef _RW_ATTR_H
#define _RW_ATTR_H

#include <boost/python.hpp>
#include <tango.h>


using namespace boost::python;


void set_attribute_value(Tango::Attribute *,boost::python::list);
void set_attribute_value(Tango::Attribute *,boost::python::list,long);
void set_attribute_value(Tango::Attribute *,boost::python::list,long,long);
void set_attribute_value(Tango::Attribute *,Tango::DevLong64);
void set_attribute_value(Tango::Attribute *,boost::python::str);
void set_attribute_value(Tango::Attribute *,double);
void set_attribute_value(Tango::Attribute *,Tango::DevLong64,long);
void set_attribute_value(Tango::Attribute *,Tango::DevLong64,long,long);
void set_attribute_value(Tango::Attribute *,double,long);
void set_attribute_value(Tango::Attribute *,double,long,long);
void set_attribute_value(Tango::Attribute *,boost::python::str,long);
void set_attribute_value(Tango::Attribute *,boost::python::str,long,long);

void __set_attribute_value(Tango::Attribute *,boost::python::list,long,long);

void set_attribute_value_dq(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality);
void set_attribute_value_dq(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality,long);
void __set_attribute_value_dq(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality,long,long);
void set_attribute_value_dq(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality,long,long);
void set_attribute_value_dq(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality);
void set_attribute_value_dq(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality,long);
void set_attribute_value_dq(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality,long,long);
void set_attribute_value_dq(Tango::Attribute *,boost::python::str,double,Tango::AttrQuality);
void set_attribute_value_dq(Tango::Attribute *,boost::python::str,double,Tango::AttrQuality,long);
void set_attribute_value_dq(Tango::Attribute *,boost::python::str,double,Tango::AttrQuality,long,long);

void set_attribute_value_dq(Tango::Attribute *,double,double,Tango::AttrQuality);
void set_attribute_value_dq(Tango::Attribute *,double,double,Tango::AttrQuality,long);
void set_attribute_value_dq(Tango::Attribute *,double,double,Tango::AttrQuality,long,long);

void get_write_value(Tango::WAttribute *,boost::python::list);

void set_write_value(Tango::WAttribute *,Tango::DevLong64);
void set_write_value(Tango::WAttribute *,boost::python::str);
void set_write_value(Tango::WAttribute *,double);
void set_write_value(Tango::WAttribute *,boost::python::list);
void set_write_value(Tango::WAttribute *,boost::python::list,long);
void set_write_value(Tango::WAttribute *,boost::python::list,long,long);

void get_min_value(Tango::WAttribute *,Tango::DevLong64 &);
void get_min_value(Tango::WAttribute *,double &);
void set_min_value(Tango::WAttribute *,Tango::DevLong64);
void set_min_value(Tango::WAttribute *,double);
bool is_min_value(Tango::WAttribute *);

void get_max_value(Tango::WAttribute *,Tango::DevLong64 &);
void get_max_value(Tango::WAttribute *,double &);
void set_max_value(Tango::WAttribute *,Tango::DevLong64);
void set_max_value(Tango::WAttribute *,double);
bool is_max_value(Tango::WAttribute *);

//--------------------------------------------------------------------------------
//
// Small template function to help writing writable attribute get_write_value()
//
//--------------------------------------------------------------------------------

template <typename T>
void init_list(long length,Tango::WAttribute *att,boost::python::list &the_list)
{
	const T *ptr;
	att->get_write_value(ptr);
	for (long l = 0;l < length;l++)
		the_list.append(ptr[l]);
}


#endif // _RW_ATTR_H
