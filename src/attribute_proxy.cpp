#include "conversion_utils.h"
#include <attribute_proxy.h>
#include <device_proxy.h>
#include <templates.cpp>


PythonPushEventCallbackAttr::~PythonPushEventCallbackAttr()
{
	map<Tango::DeviceProxy*,DeviceProxyUtils*>::iterator it = utils_map.begin();
	
	for(; it != utils_map.end(); ++it)
		delete (*it).second;
}

DeviceProxyUtils *PythonPushEventCallbackAttr::get_utils(Tango::DeviceProxy *d)
{
	DeviceProxyUtils *utils = NULL;
	
	map<Tango::DeviceProxy*,DeviceProxyUtils*>::iterator it = utils_map.find(d);
	
	if(it == utils_map.end())
	{
		utils = new DeviceProxyUtils(d);
		utils_map[d] = utils;
	}
	else
	{
		utils = utils_map[d];
	}
	return utils;
}

void PythonPushEventCallbackAttr::push_event(Tango::EventData *event)
{
	EventData py_event;
	
	py_event.attr_name = event->attr_name;
	
	PyThreadState *tstate;
	DeviceProxyUtils *dev_utils = get_utils(event->device);
	int th_id = -1;
	
	th_id = omni_thread::self()->id();
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
	PyEval_AcquireThread(tstate);

	try 
	{
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
				Utils::translate_from_device_attribute((*dati), attr_value, attr_info);
				py_event.attr_value = (boost::python::object)attr_value;
			}
			catch (Tango::DevFailed &e)
			{
				py_event.errors = Utils::translate_exception_value(e.errors);
				py_event.err = 1;
			}
		}
	
		{
			omni_mutex_lock lo(the_mutex);
			
			try
			{
				boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event);
			}
			catch(...)
			{
				PyThreadState_Swap(NULL);
				PyEval_ReleaseLock();
				cout << "Unexpected exception calling push_event method"<<endl;
				abort();
			}
		}
	}
	catch(...)
	{
		PyThreadState_Swap(NULL);
		PyEval_ReleaseLock();
		cout << "Unexpected exception calling push_event method"<<endl;
		abort();
	}

	PyThreadState_Swap(NULL);
	PyEval_ReleaseLock();
}

void PythonPushEventCallbackAttr::push_event(Tango::AttrConfEventData *event_cfg_data)
{
	AttrConfEventData py_event;

	py_event.attr_name = event_cfg_data->attr_name;	
	PyThreadState *tstate;
	
	int th_id = omni_thread::self()->id();
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

    	PyEval_AcquireThread(tstate);
	
    	if (event_cfg_data->err)
	{
		py_event.err = 1;
		const Tango::DevErrorList &dev_error_list = event_cfg_data->errors;
		py_event.errors = Utils::translate_exception_value(dev_error_list);
    	}
	else
	{
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

	{
        	omni_mutex_lock lo(the_mutex);
		boost::python::call_method<void>(python_callback.ptr(), "push_event", py_event);
	}
	
	PyThreadState_Swap(NULL);
	PyEval_ReleaseLock();
}

void PythonAttrReadCallbackAttr::attr_read(Tango::AttrReadEvent *read_event)
{
	AttrReadEvent py_read_event;
	//py_read_event.device = device->getself();
        PyThreadState *tstate = 0;
        tstate = PyThreadState_New(interp);
        PyEval_AcquireThread(tstate);
	py_read_event.attr_names = get_list_from_vector<std::string>(read_event->attr_names);
	if (read_event->err)
	{
		py_read_event.err = 1;
		const Tango::DevErrorList &dev_error_list = read_event->errors;
		/*CORBA::ULong i;
		for (i=0; i < dev_error_list.length() ; i++)
			py_read_event.errors.append( dev_error_list[i] );*/
		py_read_event.errors = Utils::translate_exception_value(dev_error_list);
	}
	else
	{
		py_read_event.err = 0;
		std::vector<Tango::DeviceAttribute> *dev_attr = read_event->argout;
		boost::python::list pyDevAttrList;
		std::vector<Tango::DeviceAttribute>::iterator i;
		AttributeValue attr_value;
		for (i = dev_attr->begin() ; i < dev_attr->end() ; i++ )
		{
			Tango::AttributeInfoEx attr_info = read_event->device->attribute_query(i->get_name());
			Utils::translate_from_device_attribute(*i, attr_value, &attr_info);
			pyDevAttrList.append(attr_value);
		}
		py_read_event.argout = pyDevAttrList;
	}
	boost::python::call_method<void>(python_callback.ptr(),"attr_read",py_read_event);
        PyThreadState_Clear(tstate);
        PyThreadState_DeleteCurrent();
}

void PythonAttrWriteCallbackAttr::attr_written(Tango::AttrWrittenEvent *write_event)
{	
	AttrWrittenEvent py_write_event;
	//py_write_event.device = device->getself();

        PyThreadState *tstate = 0;
        tstate = PyThreadState_New(interp);
        PyEval_AcquireThread(tstate);
	py_write_event.attr_names = get_list_from_vector<std::string>(write_event->attr_names);
	if (write_event->err)
	{
		py_write_event.err = 1;
		/* commented out to compile in Tango V5: check */
		/*const Tango::DevErrorList &dev_error_list = write_event->errors;
		CORBA::ULong i;
		for (i=0; i < dev_error_list.length() ; i++)
		{
			py_write_event.errors.append( dev_error_list[i] );
		}*/
	}
	else
		py_write_event.err = 0;
	boost::python::call_method<void>(python_callback.ptr(),"attr_written",py_write_event); 
        PyThreadState_Clear(tstate);
        PyThreadState_DeleteCurrent();
}

AttributeProxyUtils::AttributeProxyUtils(Tango::AttributeProxy *attr) : attributeProxy(attr)
{
	attributeInfo = attributeProxy->get_config();
}


AttributeProxy::AttributeProxy(std::string name):event_callback(NULL),att_conf_callback(NULL),read_cb(NULL),write_cb(NULL),in_serv(false)
{		
	attributeProxy = new Tango::AttributeProxy(name);
	attributeProxyUtils = new AttributeProxyUtils(attributeProxy);
	bool from_env = attributeProxy->get_device_proxy()->get_from_env_var();
	string dev_name;
	if (from_env == false)
	{
		dev_name = name;
		string::size_type pos = dev_name.rfind('/');
		dev_name.erase(pos);
	}
	else
	{
		dev_name = attributeProxy->get_device_proxy()->dev_name();
	}
	
	deviceProxy = new DeviceProxy(dev_name);
	
	try
	{
		Tango::Util::instance(false);
		in_serv = true;
	}
	catch (Tango::DevFailed &e) {}
}

AttributeProxy::AttributeProxy(const AttributeProxy& proxy):event_callback(NULL),att_conf_callback(NULL),read_cb(NULL),write_cb(NULL),in_serv(false)
{
	attributeProxy = new Tango::AttributeProxy(*proxy.attributeProxy);
	attributeProxyUtils = new AttributeProxyUtils(attributeProxy);
	bool from_env = attributeProxy->get_device_proxy()->get_from_env_var();
	string dev_name;
	if (from_env == false)
	{
		dev_name = attributeProxy->get_device_proxy()->dev_name();
		string tg_host(attributeProxy->get_device_proxy()->get_db_host());
		tg_host = tg_host + ':' + attributeProxy->get_device_proxy()->get_db_port() + '/';
		dev_name.insert(0,tg_host);
		deviceProxy = new DeviceProxy(dev_name);
	}
	else
	{
		dev_name = attributeProxy->get_device_proxy()->dev_name();
	}
	
	deviceProxy = new DeviceProxy(dev_name);
	
	try
	{
		Tango::Util::instance(false);
		in_serv = true;
	}
	catch (Tango::DevFailed &e) {}
}

AttributeProxy::AttributeProxy(DeviceProxy& d_proxy,std::string at_name):event_callback(NULL),att_conf_callback(NULL),read_cb(NULL),write_cb(NULL),in_serv(false)
{
	attributeProxy = new Tango::AttributeProxy(d_proxy.get_internal_proxy(),at_name);
	attributeProxyUtils = new AttributeProxyUtils(attributeProxy);
	deviceProxy = new DeviceProxy(d_proxy.get_internal_proxy());
	
	try
	{
		Tango::Util::instance(false);
		in_serv = true;
	}
	catch (Tango::DevFailed &e) {}
}

AttributeProxy::~AttributeProxy()
{
	delete deviceProxy;
	delete attributeProxy;
	delete attributeProxyUtils;
	
	if (event_callback != NULL)
		delete event_callback;
	if (att_conf_callback != NULL)
		delete att_conf_callback;
	if (read_cb != NULL)
		delete read_cb;
	if (write_cb != NULL)
		delete write_cb;
		
}

int AttributeProxy::ping()
{
	return attributeProxy->ping();
}

boost::python::object AttributeProxy::get_device_proxy()
{
	std::string name = attributeProxy->get_device_proxy()->dev_name();
	DeviceProxy* proxy = new DeviceProxy(name);
	return (boost::python::object) proxy;
}

boost::python::dict AttributeProxy::get_property(boost::python::list list_prop)
{
	Tango::DbData db_data;
	std::vector<std::string> vstr_array;
	get_array_value<std::string>(vstr_array,"String", (boost::python::object)list_prop);
	attributeProxy->get_property(vstr_array,db_data);
	std::vector<Tango::DbDatum>::iterator i;
	boost::python::dict py_list_prop;
	int k;
	for (k=0, i=db_data.begin(); i < db_data.end(); k++,i++)
	{
		boost::python::list py_list_val = get_list_from_vector<std::string>(i->value_string);
		py_list_prop[ vstr_array[k] ] = py_list_val;
	}
	return py_list_prop;
}

void AttributeProxy::put_property(boost::python::dict py_list_prop)
{
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
	attributeProxy->put_property(db_data);
}

void AttributeProxy::delete_property(boost::python::list list_prop)
{
	std::vector<std::string> vstr_array;
	get_array_value<std::string>(vstr_array,"String list", (boost::python::object)list_prop);
	attributeProxy->delete_property(vstr_array);
}

boost::python::object AttributeProxy::get_config()
{
	Tango::AttributeInfoEx attributeInfo = attributeProxy->get_config();
	return boost::python::object (Utils::translate_attribute_info_ex(&attributeInfo) );
}

void AttributeProxy::set_config(AttributeInfo info)
{
	Tango::AttributeInfo t_info = Utils::translate_to_attribute_info(info);
	attributeProxy->set_config(t_info);
}

void AttributeProxy::set_config(AttributeInfoEx info)
{
	Tango::AttributeInfoEx t_info = Utils::translate_to_attribute_info_ex(info);
	attributeProxy->set_config(t_info);
}

boost::python::object AttributeProxy::read()
{
	TG_INIT_PY_THREADS

	boost::python::object result;	
	try
	{
		TG_BEGIN_ALLOW_THREADS
		Tango::DeviceAttribute tangoOutData = attributeProxy->read();
		TG_END_ALLOW_THREADS
		
		AttributeValue attr_value;
		Utils::translate_from_device_attribute(tangoOutData, attr_value, 
				&attributeProxyUtils->get_info());
		return (boost::python::object)attr_value;
	}
	catch (...)
	{
		TG_COND_END_ALLOW_THREADS
		throw;
	}
}

boost::python::object AttributeProxy::read_as_str()
{
	TG_INIT_PY_THREADS

	boost::python::object result;
	try
	{
		TG_BEGIN_ALLOW_THREADS
		Tango::DeviceAttribute tangoOutData = attributeProxy->read();
		TG_END_ALLOW_THREADS
	
		AttributeValue attr_value;
		Utils::translate_from_device_attribute(tangoOutData, attr_value,
				&attributeProxyUtils->get_info(), true);
		return (boost::python::object)attr_value;
	}
	catch(...)
	{
		TG_COND_END_ALLOW_THREADS
		throw;
	}
	return result;
}

void AttributeProxy::write(AttributeValue val)
{
	Tango::DeviceAttribute dev_attr;
	Tango::AttributeInfoEx &attr_info = attributeProxyUtils->get_info();
	Utils::translate_to_device_attribute(dev_attr, val, attr_info.data_type, attr_info.data_format);
	
	TG_INIT_PY_THREADS

	try
	{	
		TG_BEGIN_ALLOW_THREADS
		attributeProxy->write(dev_attr);
		TG_END_ALLOW_THREADS
	}
	catch (...)
	{
		TG_COND_END_ALLOW_THREADS
		throw;
	}
}

boost::python::object AttributeProxy::history(int depth)
{
	Tango::AttributeInfoEx &attr_info = attributeProxyUtils->get_info();
	std::vector<Tango::DeviceAttributeHistory> *dev_attr_hist = attributeProxy->history(depth);
	boost::python::list pyDevAttrList;
	std::vector<Tango::DeviceAttributeHistory>::iterator i;
	for (i = dev_attr_hist->begin() ; i < dev_attr_hist->end() ; i++)
		pyDevAttrList.append(Utils::translate_from_attribute_history(*i , &attr_info));
	delete dev_attr_hist;
	return pyDevAttrList;
}
	
boost::python::object AttributeProxy::read_reply(long id)
{
	Tango::DeviceAttribute *tangoOutData = attributeProxy->read_reply(id);
	AttributeValue attr_value;
	Utils::translate_from_device_attribute(*tangoOutData, attr_value,
			&attributeProxyUtils->get_info());
	return (boost::python::object)attr_value;
}

boost::python::object AttributeProxy::read_reply(long id, long to)
{
	Tango::DeviceAttribute *tangoOutData = attributeProxy->read_reply(id, to);
	AttributeValue attr_value;
	Utils::translate_from_device_attribute(*tangoOutData, attr_value,
			&attributeProxyUtils->get_info());
	return (boost::python::object)attr_value;
}

long AttributeProxy::write_asynch(AttributeValue val)
{
	Tango::DeviceAttribute dev_attr;
	Tango::AttributeInfoEx &attr_info = attributeProxyUtils->get_info();
	Utils::translate_to_device_attribute(dev_attr, val, attr_info.data_type, attr_info.data_format);
	return attributeProxy->write_asynch(dev_attr);
}

void AttributeProxy::read_asynch(boost::python::object callback)
{
	PyThreadState *tstate = 0;
	
	try
	{
		if (read_cb == NULL)
			read_cb = new PythonAttrReadCallbackAttr(callback);
		else
			read_cb->set_cb(callback);
			
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		read_cb->interp = tstate->interp;
		attributeProxy->read_asynch((Tango::CallBack&) *read_cb);
	}
	catch (Tango::DevFailed &e)
	{
		if (tstate != 0)		
			PyEval_RestoreThread(tstate);
		throw e;
	}
	PyEval_RestoreThread(tstate);
}

void AttributeProxy::write_asynch(AttributeValue attr_val, boost::python::object callback)
{
	PyThreadState *tstate = 0;
	
	try
	{
		if (write_cb == NULL)
			write_cb = new PythonAttrWriteCallbackAttr(callback);
		else
			write_cb->set_cb(callback);
			
		Tango::DeviceAttribute dev_attr;
		Tango::AttributeInfoEx &attr_info = attributeProxyUtils->get_info();
		Utils::translate_to_device_attribute(dev_attr, attr_val, attr_info.data_type, attr_info.data_format);
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		write_cb->interp = tstate->interp;
		attributeProxy->write_asynch(dev_attr, (Tango::CallBack&) *write_cb);
	}
	catch (Tango::DevFailed &e)
	{
		if (tstate != 0)
			PyEval_RestoreThread(tstate);
		throw e;
	}
	
	PyEval_RestoreThread(tstate);
}

int AttributeProxy::subscribe_event(Tango::EventType event, boost::python::object call, boost::python::list filters)
{
	PyThreadState *tstate = 0;
	int event_id = 0;
	try
	{	
		std::vector<std::string> str_array;
		get_array_value<std::string>(str_array, "String", filters);

		PythonPushEventCallbackAttr *ptr;
		
		if (event == Tango::ATTR_CONF_EVENT)
		{
			ptr = att_conf_callback;
		}
		else
		{
			ptr = event_callback;
		}
				
		if (ptr == NULL)
			ptr = new PythonPushEventCallbackAttr(call);
		else
		{
			omni_mutex_lock lo(ptr->the_mutex);		
			ptr->set_cb(call);
		}
			
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		ptr->interp = tstate->interp;
		event_id = attributeProxy->subscribe_event((Tango::EventType) event, (Tango::CallBack *) ptr, (const std::vector<string>&) str_array);
	}
	catch (Tango::EventSystemFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	catch (Tango::DevFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	
	PyEval_RestoreThread(tstate);
	return event_id;

}

int AttributeProxy::subscribe_event(Tango::EventType event, boost::python::object call, boost::python::list filters, bool stateless)
{
	PyThreadState *tstate = 0;
	int event_id = 0;
	try
	{	
		std::vector<std::string> str_array;
		get_array_value<std::string>(str_array, "String", filters);

		PythonPushEventCallbackAttr *ptr;
		
		if (event == Tango::ATTR_CONF_EVENT)
		{
			ptr = att_conf_callback;
		}
		else
		{
			ptr = event_callback;
		}
				
		if (ptr == NULL)
			ptr = new PythonPushEventCallbackAttr(call);
		else
		{
			omni_mutex_lock lo(ptr->the_mutex);		
			ptr->set_cb(call);
		}
			
		PyEval_InitThreads();
		tstate = PyEval_SaveThread();
		ptr->interp = tstate->interp;
		event_id = attributeProxy->subscribe_event(event, ptr, (const std::vector<string>&)str_array, stateless);
	}
	catch (Tango::EventSystemFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	catch (Tango::DevFailed &e)
	{
		if (tstate != 0)
		{
			PyEval_RestoreThread(tstate);
		}
		throw e;
	}
	
	PyEval_RestoreThread(tstate);
	return event_id;

}


void AttributeProxy::unsubscribe_event(int event_id)
{
	TG_INIT_PY_THREADS
	
	try
	{
		TG_BEGIN_ALLOW_THREADS
		attributeProxy->unsubscribe_event(event_id);
		TG_END_ALLOW_THREADS
	}
	catch (...)
	{
		TG_COND_END_ALLOW_THREADS
		throw;
	}
}
