static const char *RcsId = "$Id: py_ev.cpp,v 1.3 2007/07/09 14:50:20 tiagocoutinho Exp $\n$Name:  $";

//+============================================================================
//
// file :               py_ev.cpp
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.3 $
//
// $Log: py_ev.cpp,v $
// Revision 1.3  2007/07/09 14:50:20  tiagocoutinho
// Added macro for handling Python GIL and Tango device synchronization
//
// Revision 1.2  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.1  2006/07/26 07:06:43  taurel
// - Add wrapping of all the new calls added in Tango V5.5 to manually firing events
//
//
// copyleft :           CELLS - ALBA
//			Campus Universitari de Bellaterra. Universitat 
//			Aut�noma de Barcelona
//			08193 Bellaterra, Barcelona
//		  	SPAIN			
//
//-============================================================================


#include <py_ev.h>
#include <rw_attr.h>


//+-------------------------------------------------------------------------
//
// function : 		push_att_event
// 
// description : Push a user event	
//
// argin(s) :   - 
//
//--------------------------------------------------------------------------

//
// For long scalar attribute
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data)
{
	p_event<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data,long x)
{
	p_event_x<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data,x);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data,long x,long y)
{
	p_event_xy<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data,x,y);
}

//
// For double
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data)
{
	p_event<double>(dev,att_name,filt_names,filt_vals,data);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data,long x)
{
	p_event_x<double>(dev,att_name,filt_names,filt_vals,data,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data,long x,long y)
{
	p_event_xy<double>(dev,att_name,filt_names,filt_vals,data,x,y);
}

//
// For string
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data)
{
	p_event<boost::python::str>(dev,att_name,filt_names,filt_vals,data);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data,long x)
{
	p_event_x<boost::python::str>(dev,att_name,filt_names,filt_vals,data,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data,long x,long y)
{
	p_event_xy<boost::python::str>(dev,att_name,filt_names,filt_vals,data,x,y);
}

//
// For list (spectrum and image attributes)
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data)
{
	p_event<boost::python::list>(dev,att_name,filt_names,filt_vals,data);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data,long x)
{
	p_event_x<boost::python::list>(dev,att_name,filt_names,filt_vals,data,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data,long x,long y)
{
	p_event_xy<boost::python::list>(dev,att_name,filt_names,filt_vals,data,x,y);
}

//
// For long scalar attribute with date and quality specified
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data,double ti,Tango::AttrQuality qual)
{
	p_event_dq<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data,ti,qual);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x)
{
	p_event_dq_x<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data,ti,qual,x);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_event_dq_xy<Tango::DevLong64>(dev,att_name,filt_names,filt_vals,data,ti,qual,x,y);
}

//
// For double attribute with date and quality specified
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data,double ti,Tango::AttrQuality qual)
{
	p_event_dq<double>(dev,att_name,filt_names,filt_vals,data,ti,qual);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data,double ti,Tango::AttrQuality qual,long x)
{
	p_event_dq_x<double>(dev,att_name,filt_names,filt_vals,data,ti,qual,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,double data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_event_dq_xy<double>(dev,att_name,filt_names,filt_vals,data,ti,qual,x,y);
}

//
// For string attribute with date and quality specified
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data,double ti,Tango::AttrQuality qual)
{
	p_event_dq<boost::python::str>(dev,att_name,filt_names,filt_vals,data,ti,qual);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data,double ti,Tango::AttrQuality qual,long x)
{
	p_event_dq_x<boost::python::str>(dev,att_name,filt_names,filt_vals,data,ti,qual,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::str data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_event_dq_xy<boost::python::str>(dev,att_name,filt_names,filt_vals,data,ti,qual,x,y);
}

//
// For list (spectrum and image attributes) with date and quality specified
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data,double ti,Tango::AttrQuality qual)
{
	p_event_dq<boost::python::list>(dev,att_name,filt_names,filt_vals,data,ti,qual);
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data,double ti,Tango::AttrQuality qual,long x)
{
	p_event_dq_x<boost::python::list>(dev,att_name,filt_names,filt_vals,data,ti,qual,x);	
}

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,boost::python::list data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_event_dq_xy<boost::python::list>(dev,att_name,filt_names,filt_vals,data,ti,qual,x,y);
}

//
// For event which are exceptions
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals,Tango::DevFailed *except)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_event (filt_names,filt_vals,except);
}

//
// For event on state or status attributes
//

void push_att_event(Tango::DeviceImpl *dev,boost::python::str att_name,vector<string> &filt_names,vector<double> &filt_vals)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_event (filt_names,filt_vals);
}


//+-------------------------------------------------------------------------
//
// function : 		push_change_event
// 
// description : Push a user event	
//
// argin(s) :   - 
//
//--------------------------------------------------------------------------

//
// For long scalar attribute
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data)
{
	p_ch_event<Tango::DevLong64>(dev,att_name,data);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,long x)
{
	p_ch_event_x<Tango::DevLong64>(dev,att_name,data,x);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,long x,long y)
{
	p_ch_event_xy<Tango::DevLong64>(dev,att_name,data,x,y);
}

//
// For double
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data)
{
	p_ch_event<double>(dev,att_name,data);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,long x)
{
	p_ch_event_x<double>(dev,att_name,data,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,long x,long y)
{
	p_ch_event_xy<double>(dev,att_name,data,x,y);
}

//
// For string
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data)
{
	p_ch_event<boost::python::str>(dev,att_name,data);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,long x)
{
	p_ch_event_x<boost::python::str>(dev,att_name,data,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,long x,long y)
{
	p_ch_event_xy<boost::python::str>(dev,att_name,data,x,y);
}

//
// For list (spectrum and image attributes)
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data)
{
	p_ch_event<boost::python::list>(dev,att_name,data);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,long x)
{
	p_ch_event_x<boost::python::list>(dev,att_name,data,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,long x,long y)
{
	p_ch_event_xy<boost::python::list>(dev,att_name,data,x,y);
}

//
// For long scalar attribute with date and quality specified
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual)
{
	p_ch_event_dq<Tango::DevLong64>(dev,att_name,data,ti,qual);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x)
{
	p_ch_event_dq_x<Tango::DevLong64>(dev,att_name,data,ti,qual,x);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ch_event_dq_xy<Tango::DevLong64>(dev,att_name,data,ti,qual,x,y);
}

//
// For double attribute with date and quality specified
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual)
{
	p_ch_event_dq<double>(dev,att_name,data,ti,qual);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual,long x)
{
	p_ch_event_dq_x<double>(dev,att_name,data,ti,qual,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ch_event_dq_xy<double>(dev,att_name,data,ti,qual,x,y);
}

//
// For string attribute with date and quality specified
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual)
{
	p_ch_event_dq<boost::python::str>(dev,att_name,data,ti,qual);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual,long x)
{
	p_ch_event_dq_x<boost::python::str>(dev,att_name,data,ti,qual,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ch_event_dq_xy<boost::python::str>(dev,att_name,data,ti,qual,x,y);
}

//
// For list (spectrum and image attributes) with date and quality specified
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual)
{
	p_ch_event_dq<boost::python::list>(dev,att_name,data,ti,qual);
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual,long x)
{
	p_ch_event_dq_x<boost::python::list>(dev,att_name,data,ti,qual,x);	
}

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ch_event_dq_xy<boost::python::list>(dev,att_name,data,ti,qual,x,y);
}

//
// For event which are exceptions
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevFailed *except)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_change_event (except);
}

//
// For event on state or status attributes
//

void push_att_ch_event(Tango::DeviceImpl *dev,boost::python::str att_name)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_change_event ();
}


//+-------------------------------------------------------------------------
//
// function : 		push_archive_event
// 
// description : Push a user event	
//
// argin(s) :   - 
//
//--------------------------------------------------------------------------

//
// For long scalar attribute
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data)
{
	p_ar_event<Tango::DevLong64>(dev,att_name,data);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,long x)
{
	p_ar_event_x<Tango::DevLong64>(dev,att_name,data,x);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,long x,long y)
{
	p_ar_event_xy<Tango::DevLong64>(dev,att_name,data,x,y);
}

//
// For double
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data)
{
	p_ar_event<double>(dev,att_name,data);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,long x)
{
	p_ar_event_x<double>(dev,att_name,data,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,long x,long y)
{
	p_ar_event_xy<double>(dev,att_name,data,x,y);
}

//
// For string
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data)
{
	p_ar_event<boost::python::str>(dev,att_name,data);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,long x)
{
	p_ar_event_x<boost::python::str>(dev,att_name,data,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,long x,long y)
{
	p_ar_event_xy<boost::python::str>(dev,att_name,data,x,y);
}

//
// For list (spectrum and image attributes)
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data)
{
	p_ar_event<boost::python::list>(dev,att_name,data);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,long x)
{
	p_ar_event_x<boost::python::list>(dev,att_name,data,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,long x,long y)
{
	p_ar_event_xy<boost::python::list>(dev,att_name,data,x,y);
}

//
// For long scalar attribute with date and quality specified
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual)
{
	p_ar_event_dq<Tango::DevLong64>(dev,att_name,data,ti,qual);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x)
{
	p_ar_event_dq_x<Tango::DevLong64>(dev,att_name,data,ti,qual,x);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevLong64 data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ar_event_dq_xy<Tango::DevLong64>(dev,att_name,data,ti,qual,x,y);
}

//
// For double attribute with date and quality specified
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual)
{
	p_ar_event_dq<double>(dev,att_name,data,ti,qual);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual,long x)
{
	p_ar_event_dq_x<double>(dev,att_name,data,ti,qual,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,double data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ar_event_dq_xy<double>(dev,att_name,data,ti,qual,x,y);
}

//
// For string attribute with date and quality specified
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual)
{
	p_ar_event_dq<boost::python::str>(dev,att_name,data,ti,qual);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual,long x)
{
	p_ar_event_dq_x<boost::python::str>(dev,att_name,data,ti,qual,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::str data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ar_event_dq_xy<boost::python::str>(dev,att_name,data,ti,qual,x,y);
}

//
// For list (spectrum and image attributes) with date and quality specified
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual)
{
	p_ar_event_dq<boost::python::list>(dev,att_name,data,ti,qual);
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual,long x)
{
	p_ar_event_dq_x<boost::python::list>(dev,att_name,data,ti,qual,x);	
}

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,boost::python::list data,double ti,Tango::AttrQuality qual,long x,long y)
{
	p_ar_event_dq_xy<boost::python::list>(dev,att_name,data,ti,qual,x,y);
}

//
// For event which are exceptions
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name,Tango::DevFailed *except)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_archive_event (except);
}

//
// For event on state or status attributes
//

void push_att_ar_event(Tango::DeviceImpl *dev,boost::python::str att_name)
{
	PyTango_Device_Sync(dev);
	
	Tango::MultiAttribute *attr_list = dev->get_device_attr();
	char *att_name_ptr = PyString_AsString(att_name.ptr());
	Tango::Attribute &attr           = attr_list->get_attr_by_name (att_name_ptr);

	attr.fire_archive_event ();
}

