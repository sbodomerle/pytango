#ifndef _PY_DS_H
#define _PY_DS_H

#include <boost/python.hpp>
#include <tango.h>
#include <cppdeviceclass.h>

using namespace boost::python;

tuple translate_exception_value(const Tango::DevErrorList &);
void Py_re_throw_dev_failed(PyObject *,PyObject *,PyObject *,Tango::PyLock *);
void Py_throw_dev_failed(PyObject *,PyObject *,PyObject *,Tango::PyLock *);
void PyDF_2_TgDF(PyObject *,Tango::DevErrorList &);

class RealPyLock:public Tango::PyLock
{
public:
	RealPyLock();
	virtual ~RealPyLock();
	
	virtual void Get();
	virtual void Release();
	
	void set_state(PyThreadState *sta) {tstate = sta;}
	int get_th_id() {return th_id;}
	bool get_py_dbg() {return py_dbg;}

protected:	
	PyThreadState *tstate;
	long locking_ctr;
	int th_id;
	bool py_dbg;
	int poll_th_id;
};

class CreateRealPyLock:public Tango::CreatePyLock
{
public:
	CreateRealPyLock() {}
	virtual ~CreateRealPyLock() {}
	
	virtual Tango::PyLock *create();
};


class CppDeviceClassWrap : public CppDeviceClass
{
public:
	CppDeviceClassWrap(PyObject *self,string na):CppDeviceClass(na),m_self(self) {init_class();}
	virtual ~CppDeviceClassWrap() {}

	void PyTg_call_dev_factory(const Tango::DevVarStringArray *);
	void PyTg_call_class_sig_handler(long signo);
	void init_class();
	
    	void device_factory(const Tango::DevVarStringArray *ptr)
    	{
		PyTg_call_dev_factory(ptr);
    	}
	
	virtual void delete_class();

    	void signal_handler(long signo)
    	{
		if (class_sig_handler_defined == true)
			PyTg_call_class_sig_handler(signo);
		else
			Tango::DeviceClass::signal_handler(signo);
    	}
    	void default_signal_handler(long signo) {this->Tango::DeviceClass::signal_handler(signo);}
	
	
    	PyObject *m_self;
	bool class_sig_handler_defined;
};



class DeviceImplWrap : public Tango::DeviceImpl, public wrapper<Tango::DeviceImpl>
{
public:
    	DeviceImplWrap(PyObject *self,CppDeviceClass *cl,std::string &st):Tango::DeviceImpl(cl,st),m_self(self) {Py_INCREF(m_self);};
   	 DeviceImplWrap(PyObject *self,CppDeviceClass *cl,const char *name,
                   const char *desc = "A Tango device",Tango::DevState sta = Tango::UNKNOWN,
		   const char *status = StatusNotSet):Tango::DeviceImpl(cl,name,desc,sta,status),m_self(self) {Py_INCREF(m_self);};
    
    	void init_device() {this->get_override("init_device")();}
    
    	PyObject *m_self;
    	
    	string PyTg_call_adm_name() { string str(Tango::DeviceImpl::adm_name()); return str; } 
    	//string get_adm_name() { string str(Tango::DeviceImpl::adm_name()); return str; }
};


class Device_2ImplWrap : public Tango::Device_2Impl, public wrapper<Tango::Device_2Impl>
{
public:
    	Device_2ImplWrap(PyObject *self,CppDeviceClass *cl,std::string &st):Tango::Device_2Impl(cl,st),m_self(self) {Py_INCREF(m_self);};
    	Device_2ImplWrap(PyObject *self,CppDeviceClass *cl,const char *name,
                   const char *desc = "A Tango device",Tango::DevState sta = Tango::UNKNOWN,
		   const char *status = StatusNotSet):Tango::Device_2Impl(cl,name,desc,sta,status),m_self(self) {Py_INCREF(m_self);};
    
    	void init_device()  {this->get_override("init_device")();}
    
    	PyObject *m_self;
};


class Device_3ImplWrap : public Tango::Device_3Impl
{
public:
    	Device_3ImplWrap(PyObject *self,CppDeviceClass *cl,std::string &st):Tango::Device_3Impl(cl,st),m_self(self) {init_instance();}
    	Device_3ImplWrap(PyObject *self,CppDeviceClass *cl,const char *name,
                   const char *desc = "A Tango device",Tango::DevState sta = Tango::UNKNOWN,
		   const char *status = StatusNotSet):Tango::Device_3Impl(cl,name,desc,sta,status),m_self(self) {init_instance();}
		   
 	virtual ~Device_3ImplWrap() {}

	void init_instance();
	void PyTg_call_void_void(const char *);
	void PyTg_call_read_hard(vector<long> &);
	void PyTg_call_delete_device();
	void PyTg_call_sig_handler(long);
	Tango::DevState PyTg_call_State();
	Tango::ConstDevString PyTg_call_Status();

    	void init_device();
	void delete_dev();
	
	void cpp_add_attribute(Tango::Attr *att);
	void cpp_remove_attribute(const char *att_name) {string str(att_name);Tango::DeviceImpl::remove_attribute(str,false);}
	
    	void always_executed_hook()
    	{
		if (always_defined == true)
			PyTg_call_void_void("always_executed_hook");
		else
			Tango::DeviceImpl::always_executed_hook();
    	}
    	void default_always_executed_hook() {this->Tango::DeviceImpl::always_executed_hook();}

    	void read_attr_hardware(vector<long> &attr_list)
    	{
		if (read_attr_defined == true)
			PyTg_call_read_hard(attr_list);
		else
			Tango::DeviceImpl::read_attr_hardware(attr_list);
    	}
    	void default_read_attr_hardware(vector<long> &attr_list) {this->Tango::DeviceImpl::read_attr_hardware(attr_list);}
	
    	void delete_device()
    	{
		if (delete_dev_defined == true)
			PyTg_call_delete_device();
		else
			Tango::DeviceImpl::delete_device();
    	}
    	void default_delete_device() {this->Tango::DeviceImpl::delete_device();}
	
    	void signal_handler(long signo)
    	{
		if (sig_handler_defined == true)
			PyTg_call_sig_handler(signo);
		else
			Tango::DeviceImpl::signal_handler(signo);
    	}
    	void default_signal_handler(long signo) {this->Tango::DeviceImpl::signal_handler(signo);}

    	Tango::DevState dev_state()
    	{
		if (State_defined == true)
			return PyTg_call_State();
		else
			return Tango::DeviceImpl::dev_state();
    	}
    	Tango::DevState default_dev_state() {return this->Tango::DeviceImpl::dev_state();}

    	Tango::ConstDevString dev_status()
    	{
		if (Status_defined == true)
			return PyTg_call_Status();
		else
			return Tango::DeviceImpl::dev_status();
    	}
    	Tango::ConstDevString default_dev_status() {return this->Tango::DeviceImpl::dev_status();}	
	        
    	PyObject *m_self;
	

protected:
	void met_exist_callable(string &,string &,PyObject *);

	bool always_defined;
	bool read_attr_defined;
	bool delete_dev_defined;
	bool sig_handler_defined;
	bool State_defined;
	bool Status_defined;
};

class Py_Device_3ImplExt:public Tango::Device_3ImplExt
{
public:
	Py_Device_3ImplExt(Device_3ImplWrap *ptr):Tango::Device_3ImplExt(),my_dev(ptr) {}
	virtual ~Py_Device_3ImplExt() {}
	
	virtual	void	delete_dev() {my_dev->delete_dev();}
	
	Device_3ImplWrap		*my_dev;
};


//--------------------------------------------------------------------------------
//
// Small template structure to defined to python conversion of Tango
// CORBA sequences
//
//--------------------------------------------------------------------------------


template <typename T>
struct CORBASequence_to_python
{
	static PyObject *convert(T const &str_s)
	{
		boost::python::list the_list;
		for (unsigned int i = 0;i < str_s.length();i++)
		{
			the_list.append(str_s[i]);
		}
		return incref(the_list.ptr());
	}
};


#endif // _PY_DS 
