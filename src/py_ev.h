#ifndef _PY_EV_H
#define _PY_EV_H

#include <boost/python.hpp>
#include <tango.h>


using namespace boost::python;


void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality,long);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality,long,long);

void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevFailed *ex);
void push_att_event(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &);

//
//--------------------------------------------------------------------------------
//

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long,long);

void push_att_ch_event(Tango::DeviceImpl *,boost::python::str,Tango::DevFailed *ex);
void push_att_ch_event(Tango::DeviceImpl *,boost::python::str);

//
//--------------------------------------------------------------------------------
//

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long,long);

void push_att_ar_event(Tango::DeviceImpl *,boost::python::str,Tango::DevFailed *ex);
void push_att_ar_event(Tango::DeviceImpl *,boost::python::str);


//--------------------------------------------------------------------------------
//
// Macro for handling Python GIL and Tango device synchronization
//
//--------------------------------------------------------------------------------

#define PyTango_Device_Sync(dev) \
	Py_BEGIN_ALLOW_THREADS \
	Tango::AutoTangoMonitor synch(dev); \
	Py_END_ALLOW_THREADS


//--------------------------------------------------------------------------------
//
// Small template function to help writing writable attribute get_write_value()
//
//--------------------------------------------------------------------------------

template <typename T>
void p_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data);
	attr.fire_event(filt_names,filt_vals);	
}


template <typename T>
void p_event_x(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x);
	attr.fire_event(filt_names,filt_vals);	
}


template <typename T>
void p_event_xy(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x,y);
	attr.fire_event(filt_names,filt_vals);	
}

template <typename T>
void p_event_dq(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data,double ti,Tango::AttrQuality qual)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual);
	attr.fire_event(filt_names,filt_vals);	
}


template <typename T>
void p_event_dq_x(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data,double ti,Tango::AttrQuality qual,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x);
	attr.fire_event(filt_names,filt_vals);	
}


template <typename T>
void p_event_dq_xy(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,T data,double ti,Tango::AttrQuality qual,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x,y);
	attr.fire_event(filt_names,filt_vals);	
}

//
//---------------------------------------------------------------------------------
//

template <typename T>
void p_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,T data)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data);
	attr.fire_change_event();	
}


template <typename T>
void p_ch_event_x(Tango::DeviceImpl *dev,boost::python::str att_name,T data,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x);
	attr.fire_change_event();	
}


template <typename T>
void p_ch_event_xy(Tango::DeviceImpl *dev,boost::python::str att_name,T data,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x,y);
	attr.fire_change_event();	
}

template <typename T>
void p_ch_event_dq(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual);
	attr.fire_change_event();	
}


template <typename T>
void p_ch_event_dq_x(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x);
	attr.fire_change_event();	
}


template <typename T>
void p_ch_event_dq_xy(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x,y);
	attr.fire_change_event();	
}

//
//---------------------------------------------------------------------------------
//

template <typename T>
void p_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,T data)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data);
	attr.fire_archive_event();	
}


template <typename T>
void p_ar_event_x(Tango::DeviceImpl *dev,boost::python::str att_name,T data,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x);
	attr.fire_archive_event();	
}


template <typename T>
void p_ar_event_xy(Tango::DeviceImpl *dev,boost::python::str att_name,T data,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value(&attr,data,x,y);
	attr.fire_archive_event();	
}

template <typename T>
void p_ar_event_dq(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual);
	attr.fire_archive_event();	
}


template <typename T>
void p_ar_event_dq_x(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual,long x)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x);
	attr.fire_archive_event();	
}


template <typename T>
void p_ar_event_dq_xy(Tango::DeviceImpl *dev,boost::python::str att_name,T data,double ti,Tango::AttrQuality qual,long x,long y)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name(att_name_ptr);	

	set_attribute_value_dq(&attr,data,ti,qual,x,y);
	attr.fire_archive_event();	
}


#endif // _PY_EV_H
