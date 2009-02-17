#ifndef PY_ATTRIBUTE_PROXY
#define PY_ATTRIBUTE_PROXY

#include <tango.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/any.hpp>

#include <device_proxy.h>

//
// The per thread data storage key
//

#ifdef WIN32
__declspec(dllimport) omni_thread::key_t key_py_data;
#else
extern omni_thread::key_t key_py_data;
#endif


class PythonPushEventCallbackAttr;
class PythonAttrReadCallbackAttr;
class PythonAttrWriteCallbackAttr;

class AttributeProxyUtils
{
	public:
		AttributeProxyUtils(Tango::AttributeProxy *);
		Tango::AttributeInfoEx &get_info() { return attributeInfo; }
		
	private:
		Tango::AttributeProxy *attributeProxy;
		Tango::AttributeInfoEx attributeInfo;
};


class AttributeProxy : public boost::enable_shared_from_this<AttributeProxy>
{
	private:
		Tango::AttributeProxy 		*attributeProxy;
		AttributeProxyUtils 		*attributeProxyUtils;
		DeviceProxy 			*deviceProxy;
		PythonPushEventCallbackAttr 	*event_callback;
		PythonPushEventCallbackAttr	*att_conf_callback;
		PythonAttrReadCallbackAttr 	*read_cb;
		PythonAttrWriteCallbackAttr 	*write_cb;
		bool				in_serv;
					
	public:
		// constructors and destructor
		AttributeProxy(std::string);
		AttributeProxy(const AttributeProxy &);
		AttributeProxy(DeviceProxy &,std::string);
		~AttributeProxy();
	
		// general methods
		inline std::string name() { return attributeProxy->name(); };
		boost::python::object get_device_proxy();
	/*	void parse_name(string &);*/
		string status(){ return attributeProxy->status(); };
		Tango::DevState state(){ return attributeProxy->state(); };	
		int ping();
		void set_transparency_reconnection(bool val) {attributeProxy->set_transparency_reconnection(val);}
		bool get_transparency_reconnection() {return attributeProxy->get_transparency_reconnection();}
				
		// property methods
		boost::python::dict get_property(boost::python::list);
		void put_property(boost::python::dict); 
		void delete_property(boost::python::list);
	
		// attribute methods
		boost::python::object get_config();	
		void set_config(AttributeInfo);
		void set_config(AttributeInfoEx);	
		boost::python::object read();
		boost::python::object read_as_str();
		void write(AttributeValue);
		
		// history methods
		boost::python::object history(int);
		
		// Polling administration methods
		void poll(int interval){attributeProxy->poll(interval);};
		int get_poll_period(){return attributeProxy->get_poll_period();};
		bool is_polled(){return attributeProxy->is_polled();};
		void stop_poll(){attributeProxy->stop_poll();};
		
		// Asynchronous methods
		long read_asynch() {return attributeProxy->read_asynch();}
		boost::python::object read_reply(long id);
		boost::python::object read_reply(long id,long to);

		/* non funzia */
		long write_asynch(AttributeValue);
		void write_reply(long id) {attributeProxy->write_reply(id);}
		void write_reply(long id,long to) {attributeProxy->write_reply(id,to);}
	
		void read_asynch(boost::python::object callback);
		void write_asynch(AttributeValue attr_val, boost::python::object callback);
	
		// Event methods
		int subscribe_event(Tango::EventType event, boost::python::object call, boost::python::list filters);
		int subscribe_event(Tango::EventType event, boost::python::object call, boost::python::list filters, bool stateless);
		void unsubscribe_event(int event_id);
};
	
class PythonPushEventCallbackAttr : public Tango::CallBack
{
public:
	PythonPushEventCallbackAttr(boost::python::object callback) : python_callback(callback) {};
	virtual ~PythonPushEventCallbackAttr();
		
	void push_event(Tango::EventData*);
	void push_event(Tango::AttrConfEventData *);
	void set_cb(boost::python::object &cb) {python_callback = cb;}
		
	omni_mutex			the_mutex;
	PyInterpreterState		*interp;
private:
	boost::python::object						python_callback;
	map<int,PyThreadState *>					py_tstate_map;
	map<Tango::DeviceProxy*,DeviceProxyUtils*>	utils_map;
	
	DeviceProxyUtils *get_utils(Tango::DeviceProxy *);
};

class PythonAttrReadCallbackAttr : public Tango::CallBack
{
public:
	PythonAttrReadCallbackAttr(boost::python::object callback) : python_callback(callback){};
	virtual ~PythonAttrReadCallbackAttr(){};
		
	void  attr_read(Tango::AttrReadEvent*);
	void set_cb(boost::python::object &cb) {python_callback = cb;}
		
	PyInterpreterState *interp;
private:
	boost::python::object python_callback;
};

class PythonAttrWriteCallbackAttr : public Tango::CallBack
{
public:
	PythonAttrWriteCallbackAttr(boost::python::object callback) : python_callback(callback){};
	virtual ~PythonAttrWriteCallbackAttr(){};
		
	void  attr_written(Tango::AttrWrittenEvent*);
	void set_cb(boost::python::object &cb) {python_callback = cb;}
		
	PyInterpreterState *interp;
private:
	boost::python::object python_callback;
};
#endif
