
#include <boost/python.hpp>
#include <attribute_proxy.h>

void py_ds();

using namespace boost::python;

int (AttributeProxy::*attr_subscribe_event1) (Tango::EventType, boost::python::object, boost::python::list) = &AttributeProxy::subscribe_event;
int (AttributeProxy::*attr_subscribe_event2) (Tango::EventType, boost::python::object, boost::python::list, bool) = &AttributeProxy::subscribe_event;


class ApiUtil
{
	public:
		ApiUtil(){};
		~ApiUtil(){};
		void get_asynch_replies1(){ Tango::ApiUtil::instance()->get_asynch_replies();};
		void get_asynch_replies2(long to){ Tango::ApiUtil::instance()->get_asynch_replies(to);};
		void set_asynch_cb_sub_model(Tango::cb_sub_model mod){ Tango::ApiUtil::instance()->set_asynch_cb_sub_model(mod); };
};

void attribute_proxy()
{
	class_<ApiUtil>("ApiUtil")
		.def("get_asynch_replies", &ApiUtil::get_asynch_replies1)
		.def("get_asynch_replies", &ApiUtil::get_asynch_replies2)
		.def("set_asynch_cb_sub_model", &ApiUtil::set_asynch_cb_sub_model)
	;
	
	class_<AttributeProxy>("AttributeProxy", 
		"AttributeProxy is the high level Tango object which provides the client\n"
		"with an easy-to-use interface to TANGO attributes.\n"
		"To create an AttributeProxy, a complete attribute name must be set in the object\n"
		"constructor.\n"
		"Example :\n"
		"	att = AttributeProxy(""tango/tangotest/1/long_scalar"")",
		 init<std::string>())
	
		.def(init<const AttributeProxy&>())
		.def(init<DeviceProxy&,std::string>())
		.def("ping", &AttributeProxy::ping,
			"A method which sends a ping to the device\n"
			"Parameters : None\n"
			"Return     : time elapsed in milliseconds")
							      
		.def("name", &AttributeProxy::name,
			"Returns the attribute name")
			
		.def("get_transparency_reconnection", &AttributeProxy::get_transparency_reconnection,
			"Returns the device transparency reconnection flag")
				
		.def("set_transparency_reconnection", &AttributeProxy::set_transparency_reconnection,
			"Set the device transparency reconnection flag")
				
		.def("status", &AttributeProxy::status,
			"A method which returns the status of the device as a string.\n"
			"Parameters : None\n"
			"Return     : None")
		
		.def("state", &AttributeProxy::state,
			"A method which returns the state of the device.\n"
			"Parameters : None\n"
			"Return     : DevState constant\n"
			"Example :\n"
			"	dev_st = dev.state()\n"
			"		if dev_st == DevState.ON : ...")
		
		.def("get_device_proxy", &AttributeProxy::get_device_proxy,
			"A method which returns the device associated to the attribute\n"
			"Parameters : None\n"
			"Return     : DeviceProxy object\n"
			)
		
		.def("get_property", &AttributeProxy::get_property,
			"A method to retrieve some properties for the given attribute\n"
			"Parameters : List of strings\n"
			"Return : Dictionary of name/value\n"
			"Example :\n"
			"	prop = att.get_property(['prop1', 'prop2'])\n"
			"	print prop\n"
			"	(stdout) {'prop1': ['1'], 'prop2': ['2']}")
			
		.def("put_property", &AttributeProxy::put_property,
			"A method to add some properties for the given attribute\n"
			"Parameters : Dictionary of name/value\n"
			"Example :\n"
			"	att.put_property({'prop1' : '1', 'prop2' : '2'})")
		
		.def("delete_property", &AttributeProxy::delete_property,
			"A method which deletes some properties for the given attribute\n"
			"Parameters : List of strings\n"
			"Example :\n"
			"	att.delete_property(['prop1', 'prop2'])\n")
			
		.def("get_config", &AttributeProxy::get_config,
			"Ask for information about attribute config\n"
			"Parameters : None\n"
			"Return     : AttributeInfoEx object\n")
		
		.def("set_config",(void (AttributeProxy::*) (AttributeInfo) ) &AttributeProxy::set_config,
			"Changes the attribute configuration\n"
			"Parameters : AttributeInfo object\n"
			"Return     : None")

		.def("set_config",(void (AttributeProxy::*) (AttributeInfoEx) ) &AttributeProxy::set_config,
			"Changes the attribute configuration\n"
			"Parameters : AttributeInfoEx object\n"
			"Return     : None")
							      
		.def("read", &AttributeProxy::read,
			"Reads the attribute value\n"
			"Parameters : None\n"
			"Return     : AttributeValue object\n")
			
		.def("read_as_str", &AttributeProxy::read_as_str,
			"Reads the attribute value as a Python string\n"
			"Parameters : None\n"
			"Return     : AttributeValue object\n")
										 
		.def("write", &AttributeProxy::write,
			"Writes a value\n"
			"Parameters : AttributeValue object\n"
			"Return     : None\n")
		
		.def("history", &AttributeProxy::history,
			"Retrieves attribute history from the polling buffer.\n"
			"Parameters :\n"
			"	- depth : integer representing the desired history depth\n"
			"Return    : a list of DeviceDataHistory types"
			"Example :\n"
			"        hist = att.history(3)\n"
			"        for elem in hist : print elem\n"
			"see DeviceDataHistory documentation for more detail")
		
		.def("poll", &AttributeProxy::poll,
			"Sets polling interval\n"
			"Parameters : interval (milliseconds)\n"
			"Return     : None")
		
		.def("get_poll_period", &AttributeProxy::get_poll_period,
			"Gets polling interval\n"
			"Parameters : None)\n"
			"Return     : interval (milliseconds)")
			
		.def("is_polled", &AttributeProxy::is_polled,
			"Checks wheter the attribute is polled or not\n"
			"Parameters : None)\n"
			"Return     : bool")
			
		.def("stop_poll", &AttributeProxy::stop_poll,
			"Stops polling\n"
			"Parameters : None\n"
			"Return     : None")
			
		.def("read_asynch", (long (AttributeProxy::*) (void)) &AttributeProxy::read_asynch,
			"Reads the attribute value in asynch mode\n"
			"Parameters : None\n"
			"Return     : read action identifier")
		
		.def("read_asynch", (void (AttributeProxy::*) (boost::python::object)) &AttributeProxy::read_asynch,
			"Reads the attribute value in asynch mode\n"
			"Parameters : None\n"
			"Return     : read action identifier")
		
		.def("read_reply", (boost::python::object (AttributeProxy::*) (long)) &AttributeProxy::read_reply,
			"Gets the read reply\n"
			"Parameters : read action identifier\n"
			"Return     : AttributeValue object")
		
		.def("read_reply", (boost::python::object (AttributeProxy::*) (long,long)) &AttributeProxy::read_reply,
			"Gets the read reply\n"
			"Parameters : \n"
			"	- read action identifier\n"
			"	- timeout (milliseconds)\n"
			"Return     : AttributeValue object")
		
	/*	.def("write_asynch", (long (AttributeProxy::*) (AttributeValue)) &AttributeProxy::write_asynch)
		.def("write_asynch", (void (AttributeProxy::*) (AttributeValue, boost::python::object)) &AttributeProxy::write_asynch)
		.def("write_reply", (void (AttributeProxy::*) (long)) &AttributeProxy::write_reply)
		.def("write_reply", (void (AttributeProxy::*) (long, long)) &AttributeProxy::write_reply)*/
		
		.def("subscribe_event", attr_subscribe_event1,
			"Subscribes to event notifier\n"
			"Parameters :\n"
			"	- EventType\n"
			"	- reference to a callback function\n"
			"	- list of strings to filter the events\n"
			"Return     : Event id")
			
		.def("subscribe_event", attr_subscribe_event2,
			"Subscribes to event notifier\n"
			"When the stateless flag is set to False, an exception will be "
			"thrown when the event subscription encounters a problem.\n"
			"With the stateless flag set to True, the event subscription will "
			"always succeed, even if the corresponding device server is not "
			"running. The keep alive thread will try every 10 seconds to "
			"subscribe for the specified event. At every subscription retry, a "
			"callback is executed which contains the corresponding exception.\n"
			"Parameters :\n"
			"	- EventType\n"
			"	- reference to a callback function\n"
			"	- list of strings to filter the events\n"
			"   - stateless: stateless flag. See text above for detailed "
			"description\n"
			"Return     : Event id")
			
		.def("unsubscribe_event", &AttributeProxy::unsubscribe_event,
			"Unsubscribes to event notifier\n"
			"Parameters : Event id \n"
			"Return     : None")
	;
	
	py_ds();
}


































