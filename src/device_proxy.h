//+=============================================================================
//
// file :         device_proxy.h
//
// description :  header file for all principal C++ classes of 
//			the python binding module.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.32 $
//
// $Log: device_proxy.h,v $
// Revision 1.32  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.31  2008/05/02 13:10:45  tiagocoutinho
// added support for stateless event subscription
//
// Revision 1.30  2008/03/10 14:33:45  tiagocoutinho
// fix deadlock between configuration and change events
//
// Revision 1.28  2008/02/12 12:36:49  tiagocoutinho
// removed debugging messages
//
// Revision 1.27  2008/01/28 14:36:50  tiagocoutinho
// removed debugging messages
//
// Revision 1.26  2008/01/22 09:27:17  tiagocoutinho
// small indentation changes
//
// Revision 1.25  2008/01/16 11:08:10  tiagocoutinho
// - fix tango c++ version dependency
// - improved push_event exception handling
// - prepare omni_thread code for future version
//
// Revision 1.24  2007/07/09 14:53:40  tiagocoutinho
// - Fix for previous change
//
// Revision 1.23  2007/06/15 06:45:51  taurel
// - Change macros for GIL management in case of threads not created by omni_thread
//
// Revision 1.22  2007/06/14 12:08:31  taurel
// - Change the way the GIL is managed when calling pure C++
//
// Revision 1.21  2007/06/13 07:22:38  taurel
// Server changes:
// - Display traceback in case of exception thrown by the delete_device method
// - Enable dynamic attributes in Python server
// - Better detection for DS used withn a debugger
// Client changes
// - Receiving an empty Spectrum/Image attribute do not throw exception any more. It simply return an empty list
// - Print traceback in case of exception thrown by the event callback
// - Better thread protection in case of unsubscribe_event while an event in waiting for the GIL
// - Add read_attribute_as_str() metho dto the DeviceProxy and AttributeProxy class
// - Better management of Python GIL in case of multi-threaded client
// (GIL released when calling C++ tango)
//
// Revision 1.20  2007/05/30 15:16:17  taurel
// - Fix bug in case of heavy used of events (differents threads created
// by the ORB could executed the callback)
// - Fix a bug in case an event is pushed while the main thread is doing
// an unsubscribe_event on the same event
//
// Revision 1.19  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.18  2007/03/12 16:54:20  tiagocoutinho
// -added missing AttributeInfoEx API. Fix some bugs
//
// Revision 1.17  2007/03/12 12:39:05  tiagocoutinho
// - check for minimum tango lib version
//
// Revision 1.16  2007/03/07 10:50:55  tiagocoutinho
// - Support for AttributeInfoEx
// - Support for attribute configuration events
//
// Revision 1.15  2006/10/11 10:24:46  taurel
// - Fix bugs and memory leak in asynchronous calls
//
// Revision 1.14  2006/06/28 09:06:13  taurel
// - Fix bug in event management. Noe use the deviceProxyUtils instance of the
// binding DeviceProxy instead of creating a new one for each received event
//
// Revision 1.13  2006/04/13 13:40:03  taurel
// Several changes:
// - Add wrapping of the new set/get_transparency_reconnection AttributeProxy class methods
// - Add wrapping of the new AttributeProxy class ctor (with DeviceProxy)
// - Fix bug: Now all Pthon exceptions inherit from PyTango.DevFailed
// - Exception name from WrongSynTax to WrongNameSyntax and Communication to CommunicationFailed to be coherent with C++
// - Change the way attribute data type are managed in AttributeProxy class to speed up read() and write() methods
// - Change the R/W type for some elements of the AttributeInfo class. All of them were read only
//
// Revision 1.12  2006/03/27 08:50:48  taurel
// - Add the Deviceproxy.alisa() method
// - Fix a 32/64 bits problem in the wrapping of the "at" method  (for DS)
//
// Revision 1.11  2006/03/22 09:08:38  taurel
// - Commit after some changes from Vincenzo in device_proxy files
//
// Revision 1.10  2006/01/27 12:52:51  taurel
// - Remove C++ vector used on client side between CORBA sequences and Python list
// - Replace list by tuple on server side for DevVarLongStringArray and DevVarDoubleStringArray
// - Add defualt command case in Py DS
//
// Revision 1.9  2006/01/18 14:26:57  taurel
// - Add a "typename" definition in order to remove compilation warning
// - Change include file order to also remove compiler warnings
// - Now call device server module wrapped stuff
//
// Revision 1.8  2005/10/31 09:03:01  dusdan
// fixed bug with python2.4 and state attribute subscribe_event;
// added new Database object constructor
//
// Revision 1.7  2005/07/19 12:05:52  dusdan
// by Vincenzo Forchi`
//
// fixed bug in error handling in push_event (segfault)
// unified format of deverrorlist (tuple of dictionaries)
//
// Revision 1.6  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
//
// Revision 1.5  2005/04/14 08:33:22  dusdan
// Vincenzo Forchi`:
// - fields device and event in EventData are filled in push_event
// - unsubscribe_event now decrements reference count for python callback
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
// Revision 1.3  2005/03/24 13:19:43  taurel
// - Added Vincenzo changes for the Tango V5 port
// - Fix bug on Linux in the client_request function (default  argument)
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
#ifndef PY_DEVICE_PROXY
#define PY_DEVICE_PROXY

#include <boost/python.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <tango.h>
#include <map>

#define		MIN_TANGO_VERSION			610

/* VF */
class PythonPushEventCallback;
enum EventType {
	CHANGE_EVENT=0,
	QUALITY_EVENT,
	PERIODIC_EVENT,
	ARCHIVE_EVENT,
	USER_EVENT,
	ATTR_CONF_EVENT
	
};

struct DeviceDataHistory {

    Tango::TimeVal          time;
    int                     cmd_failed;
    boost::python::object   value;
    boost::python::list     errors;
};

struct AttributeAlarmInfo
{
	std::string			min_alarm;
	std::string			max_alarm;
	std::string 		min_warning;
	std::string			max_warning;
	std::string			delta_t;
	std::string			delta_val;
	boost::python::list	extensions;
};

struct ChangeEventInfo
{
	std::string			rel_change;
	std::string			abs_change;
	boost::python::list	extensions;
};

struct PeriodicEventInfo
{
	std::string			period;
	boost::python::list	extensions;
};

struct ArchiveEventInfo
{
	std::string			archive_rel_change;
	std::string			archive_abs_change;
	std::string			archive_period;
	boost::python::list	extensions;
};

struct AttributeEventInfo
{
	ChangeEventInfo		ch_event;
	PeriodicEventInfo	per_event;
	ArchiveEventInfo	arch_event;
};

struct AttributeInfo {

    std::string             name;
    Tango::AttrWriteType    writable;
    Tango::AttrDataFormat   data_format;
    int                     data_type;
    int                     max_dim_x;
    int                     max_dim_y;
    std::string             description;
    std::string             label;
    std::string             unit;
    std::string             standard_unit;
    std::string             display_unit;
    std::string             format;
    std::string             min_value;
    std::string             max_value;
    std::string             min_alarm;
    std::string             max_alarm;
    std::string             writable_attr_name;
    boost::python::list     extensions;
    Tango::DispLevel        disp_level;
};

struct AttributeInfoEx: public AttributeInfo {

    AttributeAlarmInfo	alarms;
    AttributeEventInfo	events;
	boost::python::list	sys_extensions;
};

struct AttributeValue {

    boost::python::object   value;
    boost::python::object   scalar_w_value;
    Tango::TimeVal          time;
    Tango::AttrQuality      quality;
    std::string             name;
    long                    dim_x;
    long                    dim_y;
    long                    w_dim_x;
    long                    w_dim_y;
};

struct DeviceAttributeHistory {

    int                     attr_failed;
    AttributeValue          value;
    boost::python::list     errors;
};



struct CmdDoneEvent {

	boost::python::object   device;
 	boost::python::object   argout;
    std::string             cmd_name;
    int                     err;
    //boost::python::list     errors;
    boost::python::tuple     errors;
};


struct AttrReadEvent {

	boost::python::object   device;
 	boost::python::object   argout;
    boost::python::object   attr_names;
    int                     err;
    //boost::python::list     errors;
    boost::python::tuple     errors;
};

struct AttrWrittenEvent {

	boost::python::object   device;
 	boost::python::object   argout;
    boost::python::object   attr_names;
    int                     err;
    //boost::python::list     errors;
    boost::python::tuple     errors;
};

/* VF */
struct EventData {

	boost::python::object   device;
	std::string   event;
	std::string   attr_name;
 	boost::python::object   attr_value;
	int                     err;
	//boost::python::list     errors;
	boost::python::tuple     errors;
};

struct AttrConfEventData {
	
	boost::python::object	device;
	std::string				event;
	std::string				attr_name;
 	boost::python::object	attr_conf;  ///< AttributeInfoEx
	int						err;
	boost::python::tuple	errors;
};

class DeviceProxyUtils
{
public:
	DeviceProxyUtils(Tango::DeviceProxy *dev);
	//   Utils
	~DeviceProxyUtils();
	
	std::pair<long,long> get_command_args(std::string name);
	//std::pair<long,Tango::AttrDataFormat> get_attribute_type(std::string name);
	
	Tango::AttributeInfoEx *get_attribute_info(string &);
	
	void check_tango_lib_release();
	
private:
	Tango::DeviceProxy *deviceProxy;
	std::map< std::string, std::pair<long,long> > command_args;
	//std::map< std::string, std::pair<long,Tango::AttrDataFormat> > attribute_types;
	
	Tango::AttributeInfoListEx *attr_info_vec;
	std::map< std::string, Tango::AttributeInfoEx *> attr_info_map;
	
	void init_attr_info();
};


class PythonCommandCallback;
class PythonAttrReadCallback;
class PythonAttrWriteCallback;


class DeviceProxy : public boost::enable_shared_from_this<DeviceProxy>
{
public:

      DeviceProxy(std::string name);
      DeviceProxy(Tango::DeviceProxy *dev);
      DeviceProxy(const DeviceProxy&);

      ~DeviceProxy();

// General methods

      boost::python::object state();

      std::string status();

      int ping();

      void set_transparency_reconnection(bool val) {deviceProxy->set_transparency_reconnection(val);}
      bool get_transparency_reconnection() {return deviceProxy->get_transparency_reconnection();}
		
      void set_timeout_millis(int timeout);

      int get_timeout_millis();

      int get_idl_version();

      void set_source(Tango::DevSource source);

      boost::python::object get_source();

      boost::python::tuple black_box(int n);

      std::string name();
      
      std::string alias();

      std::string adm_name();

      std::string dev_name();

      std::string description();

      boost::python::object info();

      boost::python::object import_info();

// Command  related methods

	  boost::python::object command_query(std::string command);

      boost::python::list command_list_query();

      boost::python::object command_inout(std::string name, 
					  boost::python::object pyData=boost::python::object());
    
      boost::python::list command_history(std::string name,int); 


// Asynchronous command  related methods

      void command_inout_asynch_cb(std::string name,boost::python::object callback);

      void command_inout_asynch_cb(std::string name,boost::python::object pyData,boost::python::object callback);

      long command_inout_asynch(std::string name,int forget);

      long command_inout_asynch(std::string name,boost::python::object pyData,int forget);
      
      long command_inout_asynch(std::string name);

      long command_inout_asynch(std::string name,boost::python::object pyData);

      boost::python::object command_inout_reply(long id); 

      boost::python::object command_inout_reply(long id,long timeout); 
        
// Attribute related methods
      boost::python::object attribute_query(std::string attribute);

      boost::python::list attribute_list_query();
      
      boost::python::list attribute_list_query_ex();

      boost::python::list get_attribute_list();

      boost::python::object get_attribute_config(std::string attribute);

      boost::python::list get_attribute_config(boost::python::list att_list);

      boost::python::list get_attribute_config_ex(boost::python::list att_list);

      void set_attribute_config(boost::python::list att_info_list);
      
      void set_attribute_config_ex(boost::python::list att_info_ex_list);

      boost::python::object read_attribute(std::string name);
    
      boost::python::list read_attributes(boost::python::list name_list);
      
      boost::python::object read_attribute_as_str(std::string name);
      
      boost::python::list read_attributes_as_str(boost::python::list name_list);

      void write_attributes(boost::python::list attr_list);

      void write_attribute(AttributeValue attr_val);

      boost::python::object attribute_history(std::string name,int); 

// Asynchronous attribute  related methods

      long read_attribute_asynch(std::string name);

      long read_attributes_asynch(boost::python::list name_list);

      boost::python::object read_attribute_reply(long id);
    
      boost::python::object read_attribute_reply(long id,long timeout);
    
      boost::python::list read_attributes_reply(long id);

      boost::python::list read_attributes_reply(long id,long timeout);
      
      long write_attribute_asynch(AttributeValue attr_val);

      long write_attributes_asynch(boost::python::list attr_list);

      void write_attribute_reply(long id);
    
      void write_attribute_reply(long id,long timeout);
    
      void write_attributes_reply(long id);

      void write_attributes_reply(long id,long timeout);
      
      void read_attribute_asynch(std::string name,boost::python::object callback);

      void read_attributes_asynch(boost::python::list name_list,boost::python::object callback);

      void write_attribute_asynch(AttributeValue attr_val,boost::python::object callback);

      void write_attributes_asynch(boost::python::list attr_list,boost::python::object callback);

// Polling related methods
      boost::python::object is_command_polled(std::string command);

      boost::python::object is_attribute_polled(std::string attribute);
 
      int get_command_poll_period(std::string command);

      int get_attribute_poll_period(std::string attribute);

      boost::python::list polling_status();

      void poll_command(std::string command,int period);

      void poll_attribute(std::string attribute,int period);

      void stop_poll_command(std::string command);

      void stop_poll_attribute(std::string attribute);

// Miscellaneous asynchronous related methods

      long pending_asynch_call(boost::python::object req_type);

      void get_asynch_replies();

      void get_asynch_replies(long timeout);

// Property related methods

      boost::python::dict get_property(boost::python::list list_prop);

      void put_property(boost::python::dict list_prop);

      void delete_property(boost::python::list list_prop);

/* VF */ 
// Event related methods
	int subscribe_event(std::string attr_name, Tango::EventType event, boost::python::object call, boost::python::list filters);
	
	int subscribe_event(std::string attr_name, Tango::EventType event, boost::python::object call, boost::python::list filters, bool stateless);
	
	void unsubscribe_event(int event_id);
/* */

// special methods
      boost::python::object getattr(std::string command);
      boost::python::object getself();

// Helper method
      Tango::DeviceProxy *get_internal_proxy() {return deviceProxy;}
      DeviceProxyUtils *get_dev_utils() {return deviceProxyUtils;}
      Tango::TangoMonitor &get_dev_monitor() {return the_monitor;}
      std::map<int,PythonPushEventCallback *> &get_event_list() {return event_list;}
      
      PyThreadState * get_PyThreadState(PyInterpreterState *);
      
private:

      std::map<int, PythonPushEventCallback *>	event_list;
      map<int,PyThreadState *> 					py_tstate_map; ///<	 used for events
      Tango::TangoMonitor			the_monitor;
      std::string 				deviceName;
      Tango::DeviceProxy 			*deviceProxy;
      bool 					was_allocated;
      DeviceProxyUtils 				*deviceProxyUtils;
      std::map< long, std::string > 		id_to_name;
      boost::python::object 			devname;
      bool					in_serv;
      
      PythonCommandCallback 	*PyCmdCb_ptr;
      PythonAttrReadCallback	*PyAttRCb_ptr;
      PythonAttrWriteCallback	*PyAttWCb_ptr;
};



class ClientRequestHandler
{
public:
	ClientRequestHandler(DeviceProxy &dev,std::string com)
		: dev_proxy(dev),command(com)
	{}

	boost::python::object  call(boost::python::object pyData)
	{ return dev_proxy.command_inout(command,pyData); }
private:
    	DeviceProxy &dev_proxy;
	std::string command;
};
 

class PythonCommandCallback : public Tango::CallBack
{
public:
	PythonCommandCallback(DeviceProxy *dev,boost::python::object &callback,long argtype)
		: tstate(0),python_callback(callback),device(dev),argout_type(argtype)
	{}
	virtual ~PythonCommandCallback(){};

	void set_arg(long argtype) {argout_type = argtype;}
	void set_cb(boost::python::object &cb) {python_callback = cb;}	
	void  cmd_ended(Tango::CmdDoneEvent *cmd_event);
	
	PyInterpreterState *interp;
	PyThreadState *tstate;
	bool cb_thread;
private:
    	boost::python::object python_callback;
    	DeviceProxy *device;
	long argout_type;
};
 


class PythonAttrReadCallback : public Tango::CallBack
{
public:
	PythonAttrReadCallback(DeviceProxy *dev,boost::python::object &callback)
		: tstate(0),python_callback(callback),device(dev)
	{}
	virtual ~PythonAttrReadCallback(){};

	void set_cb(boost::python::object &cb) {python_callback = cb;}		
	void attr_read(Tango::AttrReadEvent *read_event);
	
	PyInterpreterState *interp;
	PyThreadState *tstate;
	bool cb_thread;
private:
    	boost::python::object python_callback;
    	DeviceProxy *device;
};
 

class PythonAttrWriteCallback : public Tango::CallBack
{
public:
	PythonAttrWriteCallback(DeviceProxy *dev,boost::python::object &callback)
		: tstate(0),python_callback(callback),device(dev)
	{}
	virtual ~PythonAttrWriteCallback(){};

	void set_cb(boost::python::object &cb) {python_callback = cb;}		
	void attr_write(Tango::AttrWrittenEvent *write_event);
	
	PyInterpreterState *interp;
	PyThreadState *tstate;
	bool cb_thread;
private:
    	boost::python::object python_callback;
    	DeviceProxy *device;
};
 
/* VF */
class PythonPushEventCallback : public Tango::CallBack
{
public:
	PythonPushEventCallback(DeviceProxy *dev,boost::python::object &callback)
		: python_callback(callback), device(dev), event_id(-1)
	{}
	virtual ~PythonPushEventCallback(){};

	void push_event(Tango::EventData *event_data);
	void push_event(Tango::AttrConfEventData *event_cfg_data);
	void set_cb(boost::python::object &cb) {python_callback = cb;}
	void set_ev_id(int id) {event_id = id;}
	
	PyInterpreterState			*interp;
		
private:
	boost::python::object 		python_callback;
	DeviceProxy 				*device;
	int							event_id;
};
/* */

//
// Four macros to manage Python threading
//

#define TG_INIT_PY_THREADS \
	bool need_restore_thread = false; \
	PyThreadState *_tg_save = NULL; \
	omni_thread::value_t *tmp_py_data; \
	Tango::PyLock *lock_ptr = NULL; \
	bool py_th = false; \
	if (in_serv == true) \
	{ \
		omni_thread::omni_thread *th_ptr = omni_thread::self(); \
		if (th_ptr == 0) \
		{ \
			py_th = true; \
		} \
		else \
		{ \
			tmp_py_data = omni_thread::self()->get_value(key_py_data); \
			if (!tmp_py_data) \
				in_serv = false; \
			else \
				lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock; \
		} \
	}
   
#define TG_BEGIN_ALLOW_THREADS \
	if  ((in_serv == false) || (py_th == true)) \
		_tg_save = PyEval_SaveThread(); \
	else \
	{ \
		lock_ptr->Release(); \
	} \
	need_restore_thread = true;
   
#define TG_END_ALLOW_THREADS \
	if (need_restore_thread == true) \
	{ \
		if ((in_serv == false) || (py_th == true)) \
		{ \
			PyEval_RestoreThread(_tg_save); \
		} \
		else \
		{ \
			lock_ptr->Get(); \
		} \
		need_restore_thread = false; \
	}
   
#define TG_COND_END_ALLOW_THREADS \
   if (need_restore_thread == true) \
   { \
	if ((in_serv == false) || (py_th == true)) \
      		PyEval_RestoreThread(_tg_save); \
	else \
		lock_ptr->Get(); \
   }

void __create_omni_dummy();

void __release_omni_dummy();

#endif

