static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/device_proxy.cpp,v 1.31 2008/05/06 11:52:27 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         device_proxy.cpp
//
// description :  C++ source for the light DeviceProxy class
//                intented to be exported to python world.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.31 $
//
// $Log: device_proxy.cpp,v $
// Revision 1.31  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.30  2008/05/02 13:10:45  tiagocoutinho
// added support for stateless event subscription
//
// Revision 1.29  2008/03/10 14:33:45  tiagocoutinho
// fix deadlock between configuration and change events
//
// Revision 1.28  2008/03/10 10:22:18  tiagocoutinho
// fix deadlock between configuration and change events
//
// Revision 1.27  2008/01/21 11:57:09  taurel
// - Add omni_thread "ensure_self" object in the push_event() methods.
// This allow user to create their own Python threads and to use
// Tango event in these threads.
//
// Revision 1.26  2008/01/16 11:08:10  tiagocoutinho
// - fix tango c++ version dependency
// - improved push_event exception handling
// - prepare omni_thread code for future version
//
// Revision 1.25  2007/06/14 12:08:31  taurel
// - Change the way the GIL is managed when calling pure C++
//
// Revision 1.24  2007/06/13 07:22:38  taurel
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
// Revision 1.23  2007/05/30 15:16:17  taurel
// - Fix bug in case of heavy used of events (differents threads created
// by the ORB could executed the callback)
// - Fix a bug in case an event is pushed while the main thread is doing
// an unsubscribe_event on the same event
//
// Revision 1.22  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.21  2007/03/12 16:54:19  tiagocoutinho
// -added missing AttributeInfoEx API. Fix some bugs
//
// Revision 1.20  2007/03/12 12:39:04  tiagocoutinho
// - check for minimum tango lib version
//
// Revision 1.19  2007/03/07 10:50:55  tiagocoutinho
// - Support for AttributeInfoEx
// - Support for attribute configuration events
//
// Revision 1.18  2007/02/16 10:24:29  taurel
// - A better fix for the dead lock i case of subscribe_event throwing errors
//
// Revision 1.17  2007/02/09 08:32:02  taurel
// - Fix possible dead lock in case the underlying C++ subscribe_event()
// method throws an exception
//
// Revision 1.16  2006/11/21 10:36:04  taurel
// Several bug fixes:
//   - Added INIT state
//   - Some memory leak
//   - Bug in asynchronous calls
//   - Abort in case of exception thrown by asynchrnous call-back
//   - Bug with Image attribute and is_allowed method
//   - Display level instead of display type
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
// Revision 1.12  2006/03/27 08:50:47  taurel
// - Add the Deviceproxy.alisa() method
// - Fix a 32/64 bits problem in the wrapping of the "at" method  (for DS)
//
// Revision 1.11  2006/03/22 09:08:37  taurel
// - Commit after some changes from Vincenzo in device_proxy files
//
// Revision 1.10  2006/01/25 15:49:52  taurel
// - Add the DEV_UCHAR case in the DeviceAttribute to a Python string. It is not
// yet completely implemented. Will come soon
//
// Revision 1.9  2005/10/31 09:03:01  dusdan
// fixed bug with python2.4 and state attribute subscribe_event;
// added new Database object constructor
//
// Revision 1.8  2005/07/19 12:05:51  dusdan
// by Vincenzo Forchi`
//
// fixed bug in error handling in push_event (segfault)
// unified format of deverrorlist (tuple of dictionaries)
//
// Revision 1.7  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
//
// Revision 1.6  2005/04/14 08:33:21  dusdan
// Vincenzo Forchi`:
// - fields device and event in EventData are filled in push_event
// - unsubscribe_event now decrements reference count for python callback
//
// Revision 1.5  2005/04/08 11:49:23  dusdan
// Vincenzo Forchi`
// - now all tango types _should_ be supported
// - modified the get_scalar_value in templates.cpp: now it tries to get a suitable c++ type from the string representation of the python object (if the boost::python::extract fails)
// - I grouped all the utility functions in a file called conversion_utils.cpp to avoid duplicated code
// - attribute_proxy_utils.cpp and device_proxy_utils.cpp aren't needed anymore
// - added a test_att_proxy.py to test the new object
// - added tango_client.py: a small command line client
//
// Revision 1.4  2005/03/29 11:41:41  taurel
// - Add try/catch block in the subscribe_event and unsubscribe_event call
//
// Revision 1.3  2005/03/24 13:19:42  taurel
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

#include <device_proxy.h>
#include <templates.cpp>
#include <pystate.h>
#include "conversion_utils.h"
#include <boost/python/errors.hpp>

#include <iostream>
using namespace std;

#include <semaphore.h>
//
// The per thread data storage key
//

#ifdef WIN32
__declspec(dllimport) omni_thread::key_t key_py_data;
#else
extern omni_thread::key_t key_py_data;
#endif

void
PythonCommandCallback::cmd_ended(Tango::CmdDoneEvent *cmd_event)
{
	CmdDoneEvent py_cmd_event;
	py_cmd_event.device = device->getself();

	if (cb_thread == true)
	{	
		if (tstate == 0)
			tstate = PyThreadState_New(interp);
		PyEval_AcquireLock();
		PyThreadState_Swap(tstate);
	}	

	py_cmd_event.cmd_name = cmd_event->cmd_name;
	if (cmd_event->err)
	{
		py_cmd_event.err = 1;
		const Tango::DevErrorList &dev_error_list = cmd_event->errors;
		py_cmd_event.errors = Utils::translate_exception_value(dev_error_list);
	}
	else
	{
		py_cmd_event.err = 0;
		py_cmd_event.argout = Utils::translate_from_device_data(cmd_event->argout,argout_type);
	}
	try
	{
		boost::python::call_method<void>(python_callback.ptr(),"cmd_ended", py_cmd_event);
	}
	catch(boost::python::error_already_set)
	{
		std::cout << "A Python exception has been sent by the Python callback" << std::endl;
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
			_exit(-1);
		abort();
	}

	if (cb_thread == true)
	{	
		PyThreadState_Swap(NULL);
		PyEval_ReleaseLock();
	}

}

void  
PythonAttrReadCallback::attr_read(Tango::AttrReadEvent *read_event)
{
	AttrReadEvent py_read_event;

	if (cb_thread == true)
	{	
		if (tstate == 0)
			tstate = PyThreadState_New(interp);
		PyEval_AcquireLock();
		PyThreadState_Swap(tstate);
	}

	py_read_event.attr_names = get_list_from_vector<std::string>(read_event->attr_names);
	if (read_event->err)
	{
		py_read_event.err = 1;
		const Tango::DevErrorList &dev_error_list = read_event->errors;
		py_read_event.errors = Utils::translate_exception_value(dev_error_list);
	}
	else
	{
		py_read_event.err = 0;
		std::vector<Tango::DeviceAttribute> *dev_attr = read_event->argout;
		boost::python::list pyDevAttrList;
		AttributeValue attr_value;
		std::vector<Tango::DeviceAttribute>::iterator i;
		for (i = dev_attr->begin() ; i != dev_attr->end() ; ++i)
		{
			Tango::AttributeInfoEx attr_info = read_event->device->attribute_query(i->get_name());
			Utils::translate_from_device_attribute(*i, attr_value, &attr_info); 
			pyDevAttrList.append(attr_value);
		}
		py_read_event.argout = pyDevAttrList;
	}
	try
	{
		boost::python::call_method<void>(python_callback.ptr(),"attr_read",py_read_event);
	}
	catch(boost::python::error_already_set)
	{
		std::cout << "A Python exception has been sent by the Python callback" << std::endl;
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
			_exit(-1);
		abort();
	}

	if (cb_thread == true)
	{	
		PyThreadState_Clear(tstate);
		PyThreadState_DeleteCurrent();
	}
}

void  
PythonAttrWriteCallback::attr_write(Tango::AttrWrittenEvent *write_event)
{	
	AttrWrittenEvent py_write_event;
	py_write_event.device = device->getself();

	if (cb_thread == true)
	{	
		if (tstate == 0)
			tstate = PyThreadState_New(interp);
		PyEval_AcquireLock();
		PyThreadState_Swap(tstate);
	}

	py_write_event.attr_names = get_list_from_vector<std::string>(write_event->attr_names);
	if (write_event->err)
	{
		py_write_event.err = 1;
		/* VF */ 
		/* commented out to compile in Tango V5: check */
		/*const Tango::DevErrorList &dev_error_list = write_event->errors;
		CORBA::ULong i;
		for (i=0; i < dev_error_list.length() ; i++)
		{
			py_write_event.errors.append( dev_error_list[i] );
		}*/
	}
	else
	{
		py_write_event.err = 0;
	}

	try
	{
		boost::python::call_method<void>(python_callback.ptr(),"attr_write",py_write_event); 
	}
	catch(boost::python::error_already_set)
	{
		std::cout << "A Python exception has been sent by the Python callback" << std::endl;
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
			_exit(-1);
		abort();
	}

	if (cb_thread == true)
	{
		PyThreadState_Clear(tstate);
		PyThreadState_DeleteCurrent();
	}
}

/* VF 21/03/2005 */	
void  
PythonPushEventCallback::push_event(Tango::EventData *event)
{
	EventData py_event;
	DeviceProxyUtils *dev_utils = device->get_dev_utils();

	py_event.device = device->getself();
	py_event.event = event->event;
	py_event.attr_name = event->attr_name;

	omni_thread::ensure_self omni_th;

	PyThreadState *tstate = device->get_PyThreadState(interp);
	PyEval_RestoreThread(tstate);

	if (event->err)
	{
		py_event.err = 1;
		const Tango::DevErrorList &dev_error_list = event->errors;
		py_event.errors = Utils::translate_exception_value(dev_error_list);
	}
	else
	{
		py_event.err = 0;
		Tango::DeviceAttribute *dati = event->attr_value;
		int pos = event->attr_name.find_last_of('/');
		std::string name = event->attr_name.substr(pos + 1, event->attr_name.size() - pos - 1);
		Tango::AttributeInfoEx *attr_info = dev_utils->get_attribute_info(name);
		try
		{
			AttributeValue attr_value;
			Utils::translate_from_device_attribute(*dati, attr_value, attr_info);
			py_event.attr_value = (boost::python::object)attr_value;
		}
		catch (Tango::DevFailed &e)
		{
			py_event.errors = Utils::translate_exception_value(e.errors);
			py_event.err = 1;
		}
	}

	try 
	{

		//
		// Before calling the python callback, we have to check that it has not been
		// already deleted due to another thread which has called the unsubscribe_event
		//
		Tango::AutoTangoMonitor lo(&(device->get_dev_monitor()));
		if (event_id != -1)
		{
			std::map<int,PythonPushEventCallback *> &ev_list = device->get_event_list();
			std::map<int, PythonPushEventCallback *>::iterator it = ev_list.find(event_id);
			if (it != ev_list.end())
				boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event); 
		}
		else
			boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event);
	} 
	catch(boost::python::error_already_set)
	{
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
			_exit(-1);	
		std::cout << "A Python exception has been thrown by the Python callback" << std::endl;
		PyErr_Print();
		PyEval_SaveThread();
		abort();
	}
	catch (Tango::DevFailed &e)
	{
		PyEval_SaveThread();
		std::cout << "A Tango exception has been thrown by the Python callback" << std::endl;
		Tango::Except::print_exception(e);
		abort();
	}
	catch(...)
	{
		PyEval_SaveThread();
		std::cout << "An unexpected exception has been thrown by the Python callback" << std::endl;
		abort();
	}

	PyEval_SaveThread();
}
/* */

void  
PythonPushEventCallback::push_event(Tango::AttrConfEventData *event_cfg_data)
{
	AttrConfEventData py_event;

	py_event.device = device->getself();
	py_event.event = event_cfg_data->event;
	py_event.attr_name = event_cfg_data->attr_name;

	omni_thread::ensure_self omni_th;

	PyThreadState *tstate = device->get_PyThreadState(interp);
	PyEval_RestoreThread(tstate);

	if (event_cfg_data->err) {
		py_event.err = 1;
		const Tango::DevErrorList &dev_error_list = event_cfg_data->errors;
		py_event.errors = Utils::translate_exception_value(dev_error_list);
	} else {
		py_event.err = 0;
		try
		{
			py_event.attr_conf = (boost::python::object)Utils::translate_attribute_info_ex(event_cfg_data->attr_conf);
		}
		catch (Tango::DevFailed &e)
		{
			py_event.errors = Utils::translate_exception_value(e.errors);
			py_event.err = 1;
		}
	}

	try 
	{

		//
		// Before calling the python callback, we have to check that it has not been
		// already deleted due to another thread which has called the unsubscribe_event
		//

		Tango::AutoTangoMonitor lo(&(device->get_dev_monitor()));
		if (event_id != -1)
		{
			std::map<int,PythonPushEventCallback *> &ev_list = device->get_event_list();
			std::map<int, PythonPushEventCallback *>::iterator it = ev_list.find(event_id);
			if (it != ev_list.end())
				boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event); 
		}
		else
			boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event);	
	}
	catch(boost::python::error_already_set)
	{
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
			_exit(-1);		
		std::cout << "A Python exception has been thrown by the Python callback (conf event)" << std::endl; 
		PyErr_Print();
		PyEval_SaveThread();
		abort();
	}
	catch (Tango::DevFailed &e)
	{
		PyEval_SaveThread();
		std::cout << "A Tango exception has been thrown by the Python callback (conf event)" << std::endl;
		Tango::Except::print_exception(e);
		abort();
	}
	catch(...)
	{
		PyEval_SaveThread();
		std::cout << "An unexpected exception has been thrown by the Python callback (conf event)" << std::endl;
		abort();
	}
	PyEval_SaveThread();
}

DeviceProxyUtils::DeviceProxyUtils(Tango::DeviceProxy *dev) : deviceProxy(dev), attr_info_vec(NULL)
{
	check_tango_lib_release();
}

DeviceProxyUtils::~DeviceProxyUtils()
{
	if(attr_info_vec)
		delete attr_info_vec;
}

void DeviceProxyUtils::check_tango_lib_release()
{
	long vers = Tango::_convert_tango_lib_release(TgLibVers);
	if (vers < MIN_TANGO_VERSION)
	{
		TangoSys_OMemStream o;
		o << "You are using Tango release " << vers;
		o << "\nThe PyTango module requires Tango release " << MIN_TANGO_VERSION << " or more";
		o << "\nPlease update Tango" << ends;

		Tango::Except::throw_exception((const char *)"PyTango_WrongTangoRelease",o.str(),
				(const char *)"DeviceProxy::check_tango_lib_release");
	}		
}

std::pair<long,long>
DeviceProxyUtils::get_command_args(std::string name)
{
	if ( command_args.count(name) == 0 ) {
		Tango::CommandInfo commandInfo = deviceProxy->command_query(name);
		command_args[name] = pair<long,long>( commandInfo.in_type , commandInfo.out_type );	
	}
	return command_args[name]; 
}

Tango::AttributeInfoEx *
DeviceProxyUtils::get_attribute_info(std::string &name)
{
	if(!attr_info_vec)
		init_attr_info();
	
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	return attr_info_map[name_lower];
}

void DeviceProxyUtils::init_attr_info()
{
	if(attr_info_vec)
		return;
	
	attr_info_vec = deviceProxy->attribute_list_query_ex();
	
	Tango::AttributeInfoListEx::iterator it = attr_info_vec->begin();
	for(; it != attr_info_vec->end(); ++it)
	{
		Tango::AttributeInfoEx &attr_info = *it;
		string name(attr_info.name);
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		attr_info_map[name] = &attr_info;
	}
}


DeviceProxy::DeviceProxy(std::string name) : deviceName(name), was_allocated(true),in_serv(false),
PyCmdCb_ptr(NULL),PyAttRCb_ptr(NULL),PyAttWCb_ptr(NULL)
{
	try {
		deviceProxy = new Tango::DeviceProxy(deviceName);
		devname = (boost::python::object) deviceProxy->dev_name();
		deviceProxyUtils = new DeviceProxyUtils(deviceProxy);

		try
		{
			Tango::Util::instance(false);
			in_serv = true;
		}
		catch (Tango::DevFailed &e) {}
	} catch(const Tango::WrongNameSyntax &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

DeviceProxy::DeviceProxy(Tango::DeviceProxy *dev) : deviceProxy(dev), was_allocated(false),in_serv(false),
PyCmdCb_ptr(NULL),PyAttRCb_ptr(NULL),PyAttWCb_ptr(NULL)
{
	try {
		deviceName = deviceProxy->dev_name(); 
		devname = (boost::python::object) deviceProxy->dev_name();
		deviceProxyUtils = new DeviceProxyUtils(deviceProxy);

		try
		{
			Tango::Util::instance(false);
			in_serv = true;
		}
		catch (Tango::DevFailed &e) {}

	} catch(const Tango::WrongNameSyntax &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

DeviceProxy::DeviceProxy(const DeviceProxy &cp)
{
	//
	// Copy ctor required by boost. Why ??
	//
}

DeviceProxy::~DeviceProxy()
{
	if (was_allocated) 
		delete deviceProxy;
	delete deviceProxyUtils;

	if (PyCmdCb_ptr != NULL)
		delete PyCmdCb_ptr;
	if (PyAttRCb_ptr != NULL)
		delete PyAttRCb_ptr;
	if (PyAttWCb_ptr != NULL)
		delete PyAttWCb_ptr;
}

boost::python::object 
DeviceProxy::getattr(std::string command)
{	
	return boost::python::object ( ClientRequestHandler(*this,command) ); 
}

boost::python::object 
DeviceProxy::getself()
{
	//	    boost::shared_ptr<DeviceProxy>  self =  shared_from_this();
	//		assert(self != 0);
	//return boost::python::object(self);
	//		return (boost::python::object) self;
	return devname;
	//	return (boost::python::object) deviceProxy->dev_name();
}

boost::python::object 
DeviceProxy::state()
{
	TG_INIT_PY_THREADS 
	try { 
		TG_BEGIN_ALLOW_THREADS
		Tango::DevState d_state = deviceProxy->state();
		TG_END_ALLOW_THREADS
		boost::python::object obj = (boost::python::object)(d_state);
		return obj; 
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
} 


std::string 
DeviceProxy::status()
{ 
	TG_INIT_PY_THREADS
	try {
		TG_BEGIN_ALLOW_THREADS 
		std::string d_status = deviceProxy->status();
		TG_END_ALLOW_THREADS
		return d_status;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}

int 
DeviceProxy::ping()
{ 
	try { 
		return deviceProxy->ping(); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} 
}     

void 
DeviceProxy::set_timeout_millis(int timeout)
{ 
	deviceProxy->set_timeout_millis(timeout); 
}     

int 
DeviceProxy::get_timeout_millis()
{ 
	return deviceProxy->get_timeout_millis(); 
}     

int 
DeviceProxy::get_idl_version()
{ 
	return deviceProxy->get_idl_version(); 
}     

void 
DeviceProxy::set_source(Tango::DevSource source)
{ 
	deviceProxy->set_source(source); 
}     

boost::python::object 
DeviceProxy::get_source()
{ 
	return (boost::python::object)(deviceProxy->get_source()); 
}     

boost::python::tuple 
DeviceProxy::black_box(int n)
{ 
	try { 
		std::vector<string> *comHist = deviceProxy->black_box(n);
		return Utils::translate_black_box(comHist);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

std::string 
DeviceProxy::name()
{ 
	try { 
		return deviceProxy->name(); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	}
}

std::string 
DeviceProxy::alias()
{ 
	try { 
		return deviceProxy->alias(); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

std::string 
DeviceProxy::adm_name()
{ 
	try { 
		return deviceProxy->adm_name(); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	}
}

std::string 
DeviceProxy::dev_name()
{ 
	return deviceProxy->dev_name(); 
}

std::string 
DeviceProxy::description()
{ 
	try { 
		return deviceProxy->description(); 
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	}
}

boost::python::object
DeviceProxy::info()
{ 
	try {
		return (boost::python::object)(deviceProxy->info());
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::object 
DeviceProxy::import_info()
{ 
	try {
		return (boost::python::object)(deviceProxy->import_info()); 
	} catch(const Tango::NonDbDevice &e) {
		throw e;
	} 
} 

boost::python::object 
DeviceProxy::command_query(std::string command)
{ 
	try { 
		return (boost::python::object)(deviceProxy->command_query(command));
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
DeviceProxy::command_list_query()
{ 
	try { 
		Tango::CommandInfoList *commandInfoList = deviceProxy->command_list_query();
		boost::python::list py_list = get_list_from_vector<Tango::CommandInfo>(*commandInfoList);
		delete commandInfoList;
		return py_list;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::object 
DeviceProxy::command_inout(std::string name, boost::python::object pyData)
{
	TG_INIT_PY_THREADS
	try { 
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		Tango::DeviceData tangoOutData;
		if (pyData.ptr() == Py_None) {
			TG_BEGIN_ALLOW_THREADS
			tangoOutData = deviceProxy->command_inout(name);
			TG_END_ALLOW_THREADS
		} else {
			Tango::DeviceData tangoInData;
			Utils::translate_to_device_data(tangoInData,pyData,arg_types.first);
			TG_BEGIN_ALLOW_THREADS
			tangoOutData = deviceProxy->command_inout(name,tangoInData);
			TG_END_ALLOW_THREADS
		}
		boost::python::object result = Utils::translate_from_device_data(tangoOutData,arg_types.second);
		return result;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	}
} 


void 
DeviceProxy::command_inout_asynch_cb(std::string name,boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		if (PyCmdCb_ptr == NULL)
			PyCmdCb_ptr = new PythonCommandCallback(this,callback,arg_types.second);
		else
		{
			PyCmdCb_ptr->set_arg(arg_types.second);
			PyCmdCb_ptr->set_cb(callback);
		}

		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		PyCmdCb_ptr->interp = tstate->interp;
		PyCmdCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyCmdCb_ptr->cb_thread = true;
		else
			PyCmdCb_ptr->cb_thread = false;

		deviceProxy->command_inout_asynch(name,*PyCmdCb_ptr);
		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_SaveThread();
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	catch(const Tango::DevFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_SaveThread();
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
} 

void 
DeviceProxy::command_inout_asynch_cb(std::string name,boost::python::object pyData, boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		Tango::DeviceData tangoInData;
		Utils::translate_to_device_data(tangoInData,pyData,arg_types.first);
		if (PyCmdCb_ptr == NULL)
			PyCmdCb_ptr = new PythonCommandCallback(this,callback,arg_types.second);
		else
		{
			PyCmdCb_ptr->set_arg(arg_types.second);
			PyCmdCb_ptr->set_cb(callback);
		}

		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		PyCmdCb_ptr->interp = tstate->interp;
		PyCmdCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyCmdCb_ptr->cb_thread = true;
		else
			PyCmdCb_ptr->cb_thread = false;

		deviceProxy->command_inout_asynch(name,tangoInData,*PyCmdCb_ptr);
		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	catch (const Tango::DevFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
}


long 
DeviceProxy::command_inout_asynch(std::string name,int forget)
{
	try
	{
		long id = deviceProxy->command_inout_asynch(name,forget);
		id_to_name[id] = name;
		return id;
	}
	catch(const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	}
}

long 
DeviceProxy::command_inout_asynch(std::string name,boost::python::object pyData,int forget)
{
	try
	{
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		Tango::DeviceData tangoInData;
		Utils::translate_to_device_data(tangoInData,pyData,arg_types.first);
		long id = deviceProxy->command_inout_asynch(name,tangoInData,forget);
		id_to_name[id] = name;
		return id;
	}
	catch(const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch (Tango::DevFailed &e)
	{
		throw e;
	}
}

long 
DeviceProxy::command_inout_asynch(std::string name)
{
	try
	{
		long id = deviceProxy->command_inout_asynch(name);
		id_to_name[id] = name;
		return id;
	}
	catch(const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch(const Tango::DevFailed &e)
	{
		throw e;
	}
}

long 
DeviceProxy::command_inout_asynch(std::string name,boost::python::object pyData)
{
	try
	{
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		Tango::DeviceData tangoInData;
		Utils::translate_to_device_data(tangoInData,pyData,arg_types.first);
		long id = deviceProxy->command_inout_asynch(name,tangoInData);
		id_to_name[id] = name;
		return id;
	}
	catch(const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch (Tango::DevFailed &e)
	{
		throw e;
	}
}


boost::python::object 
DeviceProxy::command_inout_reply(long id)
{
	try
	{
		boost::python::object result;
		std::map<long,std::string>:: iterator pos;
		if ((pos = id_to_name.find(id)) != id_to_name.end())
		{
			pair<long,long> arg_types = deviceProxyUtils->get_command_args(pos->second);
			Tango::DeviceData tangoOutData;
			tangoOutData = deviceProxy->command_inout_reply(id);
			result = Utils::translate_from_device_data(tangoOutData,arg_types.second);
		}
		else
		{
			deviceProxy->command_inout_reply(id);
		}
		return result;
	}
	catch (const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch (const Tango::DevFailed &e)
	{
		throw e;
	}
}


boost::python::object 
DeviceProxy::command_inout_reply(long id,long timeout)
{
	try
	{
		boost::python::object result;
		std::map<long,std::string>:: iterator pos;
		if ((pos = id_to_name.find(id)) != id_to_name.end())
		{
			pair<long,long> arg_types = deviceProxyUtils->get_command_args(pos->second);
			Tango::DeviceData tangoOutData;
			tangoOutData = deviceProxy->command_inout_reply(id,timeout);
			result = Utils::translate_from_device_data(tangoOutData,arg_types.second);
		}
		else
		{
			deviceProxy->command_inout_reply(id,timeout);
		}
		return result;
	}
	catch(const Tango::ConnectionFailed &e)
	{
		throw e;
	}
	catch (const Tango::DevFailed &e)
	{
		throw e;
	}
}


boost::python::list 
DeviceProxy::command_history(std::string name, int depth)
{
	try { 
		pair<long,long> arg_types = deviceProxyUtils->get_command_args(name);
		std::vector<Tango::DeviceDataHistory> *dev_data_hist = deviceProxy->command_history(name,depth);
		boost::python::list pyDevDataList;
		std::vector<Tango::DeviceDataHistory>::iterator i;
		for (i = dev_data_hist->begin() ; i < dev_data_hist->end() ; i++ )
		{
			pyDevDataList.append(Utils::translate_from_data_history(*i, arg_types.second));
		}
		delete dev_data_hist;
		return pyDevDataList;
	} catch(const Tango::NonSupportedFeature &e) {
		throw e;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	}
} 

boost::python::object 
DeviceProxy::attribute_history(std::string name, int depth)
{
	try 
	{
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(name);
		std::vector<Tango::DeviceAttributeHistory> *dev_attr_hist = deviceProxy->attribute_history(name,depth);
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttributeHistory>::iterator i;
		for (i = dev_attr_hist->begin() ; i < dev_attr_hist->end() ; i++ )
		{
			pyDevAttrList.append(Utils::translate_from_attribute_history( *i , attr_info ) );
		}
		delete dev_attr_hist;
		return pyDevAttrList;
	} catch(const Tango::NonSupportedFeature &e) {
		throw e;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	}
} 

boost::python::object 
DeviceProxy::attribute_query(std::string attribute)
{ 
	try { 
		Tango::AttributeInfoEx attributeInfo = deviceProxy->attribute_query(attribute);
		return boost::python::object ( Utils::translate_attribute_info_ex(&attributeInfo) );
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
DeviceProxy::attribute_list_query()
{ 
	try { 
		Tango::AttributeInfoList *attributeInfoList = deviceProxy->attribute_list_query();
		return Utils::translate_attribute_list(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
DeviceProxy::attribute_list_query_ex()
{ 
	try { 
		Tango::AttributeInfoListEx *attributeInfoList = deviceProxy->attribute_list_query_ex();
		return Utils::translate_attribute_list_ex(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
DeviceProxy::get_attribute_list()
{ 
	try { 
		std::vector<std::string> *attrList = deviceProxy->get_attribute_list();
		boost::python::list pyAttrList = get_list_from_vector<std::string>(*attrList);
		delete attrList;
		return pyAttrList;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e; 
	} 
} 


boost::python::object
DeviceProxy::get_attribute_config(std::string attribute)
{ 
	try { 
		Tango::AttributeInfoEx attributeInfo = deviceProxy->get_attribute_config(attribute);
		return boost::python::object ( Utils::translate_attribute_info_ex(&attributeInfo) );
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 


boost::python::list 
DeviceProxy::get_attribute_config(boost::python::list att_list)
{ 
	try { 
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)att_list);
		Tango::AttributeInfoList *attributeInfoList = deviceProxy->get_attribute_config(vstr_array);
		return Utils::translate_attribute_list(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

boost::python::list 
DeviceProxy::get_attribute_config_ex(boost::python::list att_list)
{ 
	try { 
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)att_list);
		Tango::AttributeInfoListEx *attributeInfoList = deviceProxy->get_attribute_config_ex(vstr_array);
		return Utils::translate_attribute_list_ex(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
} 

void 
DeviceProxy::set_attribute_config(boost::python::list att_info_list)
{ 
	try { 
		Tango::AttributeInfoList attributeInfoList;
		int s_len = boost::python::extract<int>(att_info_list.attr("__len__")()) ;
		for (int i = 0 ; i < s_len; i++ )
		{
			AttributeInfo att_info =  boost::python::extract<AttributeInfo>(att_info_list[i]) ;
			attributeInfoList.push_back( Utils::translate_to_attribute_info(att_info) );		  		  
		}
		deviceProxy->set_attribute_config(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

void 
DeviceProxy::set_attribute_config_ex(boost::python::list att_info_ex_list)
{ 
	try { 
		Tango::AttributeInfoListEx attributeInfoList;
		int s_len = boost::python::extract<int>(att_info_ex_list.attr("__len__")()) ;
		for (int i = 0 ; i < s_len; i++ )
		{
			AttributeInfoEx att_info =  boost::python::extract<AttributeInfoEx>(att_info_ex_list[i]) ;
			attributeInfoList.push_back( Utils::translate_to_attribute_info_ex(att_info) );		  		  
		}
		deviceProxy->set_attribute_config(attributeInfoList);
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

boost::python::object 
DeviceProxy::read_attribute(std::string name)
{
	TG_INIT_PY_THREADS
	try {
		TG_BEGIN_ALLOW_THREADS
		Tango::DeviceAttribute tangoOutData = deviceProxy->read_attribute(name);
		TG_END_ALLOW_THREADS
		AttributeValue attr_value;
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(name);
		Utils::translate_from_device_attribute(tangoOutData, attr_value, attr_info);
		return (boost::python::object)attr_value;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}


boost::python::list 
DeviceProxy::read_attributes(boost::python::list name_list)
{
	TG_INIT_PY_THREADS
	try { 
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)name_list);
		TG_BEGIN_ALLOW_THREADS
		std::vector<Tango::DeviceAttribute> *dev_attr = deviceProxy->read_attributes(vstr_array);
		TG_END_ALLOW_THREADS
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttribute>::iterator i;
		for (i = dev_attr->begin() ; i < dev_attr->end() ; i++ )
		{
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(i->get_name());
			AttributeValue attr_value;
			Utils::translate_from_device_attribute(*i, attr_value, attr_info);
			pyDevAttrList.append( attr_value );
		}
		delete dev_attr;
		return pyDevAttrList;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}

boost::python::object 
DeviceProxy::read_attribute_as_str(std::string name)
{
	TG_INIT_PY_THREADS
	try {
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(name);
		TG_BEGIN_ALLOW_THREADS
		Tango::DeviceAttribute tangoOutData = deviceProxy->read_attribute(name);
		TG_END_ALLOW_THREADS
		AttributeValue attr_value;
		Utils::translate_from_device_attribute(tangoOutData, attr_value, attr_info, true);
		return (boost::python::object)attr_value;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}


boost::python::list 
DeviceProxy::read_attributes_as_str(boost::python::list name_list)
{
	TG_INIT_PY_THREADS
	try { 
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)name_list);
		TG_BEGIN_ALLOW_THREADS
		std::vector<Tango::DeviceAttribute> *dev_attr = deviceProxy->read_attributes(vstr_array);
		TG_END_ALLOW_THREADS
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttribute>::iterator i;
		for (i = dev_attr->begin() ; i < dev_attr->end() ; i++ )
		{
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(i->get_name());
			AttributeValue attr_value;
			Utils::translate_from_device_attribute(*i , attr_value, attr_info, true);
			pyDevAttrList.append(attr_value);
		}
		delete dev_attr;
		return pyDevAttrList;
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}
void 
DeviceProxy::write_attributes(boost::python::list attr_list)
{
	TG_INIT_PY_THREADS
	try { 
		std::vector<Tango::DeviceAttribute> dev_attr_array;
		Tango::DeviceAttribute dev_attr;
		int s_len = boost::python::extract<int>(attr_list.attr("__len__")()) ;
		for (int i = 0 ; i < s_len; i++ )
		{
			AttributeValue attr_val = boost::python::extract<AttributeValue>(attr_list[i]);
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
			long arg_type = attr_info->data_type;
			Tango::AttrDataFormat arg_format = attr_info->data_format;
			Utils::translate_to_device_attribute(dev_attr, attr_val, arg_type, arg_format);
			dev_attr_array.push_back( dev_attr );		  		  
		}
		TG_BEGIN_ALLOW_THREADS
		deviceProxy->write_attributes(dev_attr_array);
		TG_END_ALLOW_THREADS
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}

void 
DeviceProxy::write_attribute(AttributeValue attr_val)
{
	TG_INIT_PY_THREADS
	try { 
		Tango::DeviceAttribute dev_attr;
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
		long arg_type = attr_info->data_type;
		Tango::AttrDataFormat arg_format = attr_info->data_format;
		Utils::translate_to_device_attribute(dev_attr, attr_val, arg_type, arg_format);
		TG_BEGIN_ALLOW_THREADS
		deviceProxy->write_attribute( dev_attr );
		TG_END_ALLOW_THREADS
	} catch(const Tango::ConnectionFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	} 
}


long 
DeviceProxy::read_attribute_asynch(std::string name)
{
	try {
		long id = deviceProxy->read_attribute_asynch(name);
		return id;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	}
}


long 
DeviceProxy::read_attributes_asynch(boost::python::list name_list)
{
	try {
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)name_list);
		long id = deviceProxy->read_attributes_asynch(vstr_array);
		return id;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	}
}


boost::python::object 
DeviceProxy::read_attribute_reply(long id)
{
	try { 
		Tango::DeviceAttribute *tangoOutData = deviceProxy->read_attribute_reply(id);
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(tangoOutData->get_name());
		AttributeValue attr_value;
		Utils::translate_from_device_attribute(*tangoOutData, attr_value, attr_info);
		return (boost::python::object)attr_value;
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


boost::python::object 
DeviceProxy::read_attribute_reply(long id,long timeout)
{
	try { 
		Tango::DeviceAttribute *tangoOutData = deviceProxy->read_attribute_reply(id,timeout);
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(tangoOutData->get_name());
		AttributeValue attr_value;
		Utils::translate_from_device_attribute(*tangoOutData, attr_value, attr_info);
		return (boost::python::object)attr_value;
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


boost::python::list 
DeviceProxy::read_attributes_reply(long id)
{
	try { 
		std::vector<Tango::DeviceAttribute> *dev_attr = deviceProxy->read_attributes_reply(id);
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttribute>::iterator i;
		AttributeValue attr_value;
		for (i = dev_attr->begin() ; i < dev_attr->end() ; i++ )
		{
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(i->get_name());
			Utils::translate_from_device_attribute(*i, attr_value, attr_info);
			pyDevAttrList.append(attr_value);
		}
		delete dev_attr;
		return pyDevAttrList;
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


boost::python::list 
DeviceProxy::read_attributes_reply(long id,long timeout)
{
	try { 
		std::vector<Tango::DeviceAttribute> *dev_attr = deviceProxy->read_attributes_reply(id,timeout);
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttribute>::iterator i;
		AttributeValue attr_value;
		for (i = dev_attr->begin() ; i < dev_attr->end() ; i++ )
		{
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(i->get_name());
			Utils::translate_from_device_attribute(*i, attr_value, attr_info);
			pyDevAttrList.append(attr_value);
		}
		delete dev_attr;
		return pyDevAttrList;
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


long 
DeviceProxy::write_attribute_asynch(AttributeValue attr_val)
{
	try { 
		Tango::DeviceAttribute dev_attr;
		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
		long arg_type = attr_info->data_type;
		Tango::AttrDataFormat arg_format = attr_info->data_format;
		Utils::translate_to_device_attribute(dev_attr, attr_val, arg_type, arg_format);
		long id = deviceProxy->write_attribute_asynch( dev_attr );
		return id;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	}
}


long 
DeviceProxy::write_attributes_asynch(boost::python::list attr_list)
{
	try { 
		std::vector<Tango::DeviceAttribute> dev_attr_array;
		Tango::DeviceAttribute dev_attr;
		int s_len = boost::python::extract<int>(attr_list.attr("__len__")()) ;
		for (int i = 0 ; i < s_len; i++ )
		{
			AttributeValue attr_val = boost::python::extract<AttributeValue>(attr_list[i]);
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
			long arg_type = attr_info->data_type;
			Tango::AttrDataFormat arg_format = attr_info->data_format;
			Utils::translate_to_device_attribute(dev_attr, attr_val, arg_type, arg_format);
			dev_attr_array.push_back( dev_attr );		  		  
		}
		long id = deviceProxy->write_attributes_asynch(dev_attr_array);
		return id;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	}
}


void 
DeviceProxy::write_attribute_reply(long id)
{
	try { 
		deviceProxy->write_attribute_reply(id);
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


void 
DeviceProxy::write_attribute_reply(long id,long timeout)
{
	try { 
		deviceProxy->write_attribute_reply(id,timeout);
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


void 
DeviceProxy::write_attributes_reply(long id)
{
	try { 
		deviceProxy->write_attributes_reply(id);
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


void 
DeviceProxy::write_attributes_reply(long id,long timeout)
{
	try { 
		deviceProxy->write_attributes_reply(id,timeout);
	} catch(const Tango::AsynCall &e) {
		throw e;
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


void 
DeviceProxy::read_attribute_asynch(std::string name,boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{
		if (PyAttRCb_ptr == NULL)
			PyAttRCb_ptr = new PythonAttrReadCallback(this,callback);
		else
			PyAttRCb_ptr->set_cb(callback);

		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		PyAttRCb_ptr->interp = tstate->interp;
		PyAttRCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyAttRCb_ptr->cb_thread = true;
		else
			PyAttRCb_ptr->cb_thread = false;

		deviceProxy->read_attribute_asynch(name,*PyAttRCb_ptr);
		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate!= 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
}


void 
DeviceProxy::read_attributes_asynch(boost::python::list name_list,boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{
		if (PyAttRCb_ptr == NULL)
			PyAttRCb_ptr = new PythonAttrReadCallback(this,callback);
		else
			PyAttRCb_ptr->set_cb(callback);

		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		PyAttRCb_ptr->interp = tstate->interp;
		PyAttRCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyAttRCb_ptr->cb_thread = true;
		else
			PyAttRCb_ptr->cb_thread = false;

		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",(boost::python::object)name_list);

		deviceProxy->read_attributes_asynch(vstr_array,*PyAttRCb_ptr);
		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
}


void 
DeviceProxy::write_attribute_asynch(AttributeValue attr_val,boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{
		if (PyAttWCb_ptr == NULL)
			PyAttWCb_ptr = new PythonAttrWriteCallback(this,callback);
		else
			PyAttWCb_ptr->set_cb(callback);

		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		PyAttWCb_ptr->interp = tstate->interp;
		PyAttWCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyAttWCb_ptr->cb_thread = true;
		else
			PyAttWCb_ptr->cb_thread = false;

		Tango::DeviceAttribute dev_attr;

		Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
		long arg_type = attr_info->data_type;
		Tango::AttrDataFormat arg_format = attr_info->data_format;
		Utils::translate_to_device_attribute(dev_attr, attr_val, arg_type, arg_format);
		deviceProxy->write_attribute_asynch( dev_attr , *PyAttWCb_ptr);

		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
}


void 
DeviceProxy::write_attributes_asynch(boost::python::list attr_list,boost::python::object callback)
{
	PyThreadState *tstate = 0;
	try
	{ 
		if (PyAttWCb_ptr == NULL)
			PyAttWCb_ptr = new PythonAttrWriteCallback(this,callback);
		else
			PyAttWCb_ptr->set_cb(callback);

		tstate = PyEval_SaveThread();
		PyAttWCb_ptr->interp = tstate->interp;
		PyAttWCb_ptr->tstate = tstate;
		Tango::ApiUtil *au = Tango::ApiUtil::instance();

		if (au->get_asynch_cb_sub_model() == Tango::PUSH_CALLBACK)
			PyAttWCb_ptr->cb_thread = true;
		else
			PyAttWCb_ptr->cb_thread = false;

		std::vector<Tango::DeviceAttribute> dev_attr_array;
		Tango::DeviceAttribute dev_attr;
		int s_len = boost::python::extract<int>(attr_list.attr("__len__")()) ;
		for (int i = 0 ; i < s_len; i++ )
		{
			AttributeValue attr_val = boost::python::extract<AttributeValue>(attr_list[i]);
			Tango::AttributeInfoEx *attr_info = deviceProxyUtils->get_attribute_info(attr_val.name);
			Utils::translate_to_device_attribute(dev_attr, attr_val, attr_info->data_type, attr_info->data_format);
			dev_attr_array.push_back( dev_attr );		  		  
		}
		deviceProxy->write_attributes_asynch(dev_attr_array,*PyAttWCb_ptr);

		PyEval_RestoreThread(tstate);
	}
	catch(const Tango::ConnectionFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
}



boost::python::object 
DeviceProxy::is_command_polled(std::string command)
{ 
	return boost::python::object( deviceProxy->is_command_polled(command) );
} 

boost::python::object 
DeviceProxy::is_attribute_polled(std::string attribute)
{ 
	return boost::python::object( deviceProxy->is_attribute_polled(attribute) );
}

int 
DeviceProxy::get_command_poll_period(std::string command)
{ 
	return deviceProxy->get_command_poll_period(command);
}

int 
DeviceProxy::get_attribute_poll_period(std::string attribute)
{ 
	return deviceProxy->get_attribute_poll_period(attribute);
}


boost::python::list 
DeviceProxy::polling_status()
{
	std::vector<std::string> *vstr_array = deviceProxy->polling_status();
	boost::python::list pyStatList = get_list_from_vector<std::string>(*vstr_array);
	delete vstr_array;
	return pyStatList;
}

void 
DeviceProxy::poll_command(std::string command,int period)
{ 
	deviceProxy->poll_command(command,period);
}

void 
DeviceProxy::poll_attribute(std::string attribute,int period)
{ 
	deviceProxy->poll_attribute(attribute,period);
}

void 
DeviceProxy::stop_poll_command(std::string command)
{ 
	deviceProxy->stop_poll_command(command);
}

void 
DeviceProxy::stop_poll_attribute(std::string attribute)
{ 
	deviceProxy->stop_poll_attribute(attribute);
}


long 
DeviceProxy::pending_asynch_call(boost::python::object req_type)
{
	Tango::asyn_req_type r_type = boost::python::extract<Tango::asyn_req_type>(req_type) ;
	return deviceProxy->pending_asynch_call(r_type);
}

void 
DeviceProxy::get_asynch_replies()
{
	deviceProxy->get_asynch_replies();
}


void 
DeviceProxy::get_asynch_replies(long timeout)
{
	try {
		deviceProxy->get_asynch_replies(timeout);
	} catch(const Tango::AsynReplyNotArrived &e) {
		throw e;
	}
}



boost::python::dict 
DeviceProxy::get_property(boost::python::list list_prop)
{
	try { 
		Tango::DbData db_data;
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String",
				(boost::python::object)list_prop);
		deviceProxy->get_property(vstr_array,db_data);
		std::vector<Tango::DbDatum>::iterator i;
		boost::python::dict py_list_prop;
		int k;
		for ( k=0,i=db_data.begin() ; i < db_data.end() ; k++,i++)
		{
			boost::python::list py_list_val = get_list_from_vector<std::string>(i->value_string);
			py_list_prop[ vstr_array[k] ] = py_list_val; 	
		}
		return py_list_prop;
	} catch(const Tango::NonDbDevice &e) {
		throw e;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}


void 
DeviceProxy::put_property(boost::python::dict py_list_prop)
{
	try { 
		Tango::DbData db_data;
		std::string prop_name;
		std::string i_string;
		boost::python::list list_prop = py_list_prop.keys();
		int p_len = boost::python::extract<int>(list_prop.attr("__len__")()) ;

		for (int i = 0 ; i < p_len; i++ )
		{
			get_scalar_value<std::string>(prop_name,"String",list_prop[i]);
			Tango::DbDatum db_datum(prop_name);
			get_array_value<std::string>(db_datum.value_string,"String list",py_list_prop[prop_name]);
			db_data.push_back(db_datum);    
		}
		deviceProxy->put_property(db_data);
	} catch(const Tango::NonDbDevice &e) {
		throw e;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

void 
DeviceProxy::delete_property(boost::python::list list_prop)
{
	try { 
		std::vector<std::string> vstr_array;
		get_array_value<std::string>(vstr_array,"String list",
				(boost::python::object)list_prop);
		deviceProxy->delete_property(vstr_array);
	} catch(const Tango::NonDbDevice &e) {
		throw e;
	} catch(const Tango::ConnectionFailed &e) {
		throw e;
	} catch(const Tango::CommunicationFailed &e) {
		throw e;
	} catch(const Tango::DevFailed &e) {
		throw e;
	} 
}

/* VF */
int DeviceProxy::subscribe_event(std::string attr_name, Tango::EventType event,
		                         boost::python::object call, 
		                         boost::python::list filters)
{
	PyThreadState *tstate = 0;
	PythonPushEventCallback *event_callback = NULL;
	try
	{
		// boost::weak_ptr<boost::python::object> temp_call(call);
		std::vector<std::string> str_array;
		get_array_value<std::string>(str_array, "String", filters);
		event_callback =  new PythonPushEventCallback(this, call);
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		event_callback->interp = tstate->interp;
		int event_id = deviceProxy->subscribe_event(attr_name, (Tango::EventType) event, (Tango::CallBack *) event_callback, (const std::vector<string>&) str_array);
		event_list.insert(std::pair<int, PythonPushEventCallback *>(event_id, event_callback));
		event_callback->set_ev_id(event_id);
		PyEval_RestoreThread(tstate);
		return event_id;
	}
	catch(const Tango::DevFailed &e) {
		delete event_callback;
		PyEval_RestoreThread(tstate);
		throw e;
	}
}

int DeviceProxy::subscribe_event(std::string attr_name, Tango::EventType event, 
                                 boost::python::object call, 
                                 boost::python::list filters, bool stateless)
{
	PyThreadState *tstate = 0;
	PythonPushEventCallback *event_callback = NULL;
	try
	{
		// boost::weak_ptr<boost::python::object> temp_call(call);
		std::vector<std::string> str_array;
		get_array_value<std::string>(str_array, "String", filters);
		event_callback =  new PythonPushEventCallback(this, call);
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		event_callback->interp = tstate->interp;
		int event_id = deviceProxy->subscribe_event(attr_name, event, 
				event_callback, (const std::vector<string>&) str_array, 
				stateless);
		event_list.insert(std::pair<int, PythonPushEventCallback *>(event_id, event_callback));
		event_callback->set_ev_id(event_id);
		PyEval_RestoreThread(tstate);
		return event_id;
	}
	catch(const Tango::DevFailed &e) {
		delete event_callback;
		PyEval_RestoreThread(tstate);
		throw e;
	}
}
void DeviceProxy::unsubscribe_event(int event_id)
{
	TG_INIT_PY_THREADS
	try
	{
		TG_BEGIN_ALLOW_THREADS
		deviceProxy->unsubscribe_event(event_id);
		TG_END_ALLOW_THREADS
		std::map<int, PythonPushEventCallback *>::iterator it = event_list.find(event_id);
		if (it != event_list.end())
		{
			Tango::AutoTangoMonitor lo(&the_monitor);
			delete (*it).second;
			event_list.erase(it);
		}
	}
	catch(const Tango::DevFailed &e) {
		TG_COND_END_ALLOW_THREADS
		throw e;
	}
}

PyThreadState * DeviceProxy::get_PyThreadState(PyInterpreterState *interp)
{
	PyThreadState *tstate = NULL;
	int th_id = omni_thread::self()->id();

	Tango::AutoTangoMonitor lo(&the_monitor);

	map<int,PyThreadState *>::iterator map_ite = py_tstate_map.find(th_id);

	if (map_ite == py_tstate_map.end())
	{
		PyThreadState *local_tstate = PyThreadState_New(interp);
		pair<map<int,PyThreadState *>::iterator,bool> status;
		status = py_tstate_map.insert(make_pair(th_id,local_tstate));
		if (status.second == false)
		{
			std::cout << "Error while inserting thread Python descriptor in map. Exiting !" << std::endl;
			abort();
		}
		tstate = local_tstate;
	}
	else
	{
		tstate = map_ite->second;
	}
	return tstate;
}


void __create_omni_dummy()
{
	omni_thread* _self = omni_thread::self();
	if (!_self)
	{
		try	{ _self  = omni_thread::create_dummy(); }
		catch(omni_thread_invalid &oti) {}
	}
}

void __release_omni_dummy()
{
	omni_thread* _self = omni_thread::self();
	if (!_self) 
	{
		try { omni_thread::release_dummy(); }
		catch(omni_thread_invalid &oti) {}
	}
}
