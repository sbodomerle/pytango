#ifndef _CPPDEVICECLASS_H
#define _CPPDEVICECLASS_H

#include <boost/python.hpp>
#include <tango.h>

using namespace boost::python;


#define 	DISPLAY_LEVEL			"display level"
#define		POLL_PERIOD			"polling period"
#define		MEMORIZED			"memorized"
#define		DEFAULT_CMD			"default command"

#define		LABEL_PROP			"label"
#define		DESC_PROP			"description"
#define		UNIT_PROP			"unit"
#define		STD_UNIT_PROP			"standard unit"
#define		DISPLAY_UNIT_PROP		"display unit"
#define		FORMAT_PROP			"format"
#define		MAX_VALUE_PROP			"max value"
#define		MIN_VALUE_PROP			"min value"
#define		MIN_ALARM_PROP			"min alarm"
#define		MAX_ALARM_PROP			"max alarm"
#define		MIN_WARNING_PROP		"min warning"
#define		MAX_WARNING_PROP		"max warning"
#define		DELTA_T_PROP			"delta time"
#define		DELTA_VAL_PROP			"delta val"

#define		MAX_ATTR_OPT			17

#define		PY_MET_INC_REF			"py_exec_wir"

class CppDeviceClass: public Tango::DeviceClass
{
public:
	CppDeviceClass(string &na):Tango::DeviceClass(na) {}
	virtual ~CppDeviceClass() {}

	virtual void attribute_factory(vector<Tango::Attr *> &);
	virtual void command_factory();

	void export_device(Tango::DeviceImpl *dev,const char *na="Unused") {Tango::DeviceClass::export_device(dev,na);}

	PyInterpreterState *get_py_interp() {return interp;}
	void set_py_interp(PyInterpreterState *in) {interp=in;}


	typedef struct disp_poll
	{
		disp_poll() {period_defined=false;disp_defined=false;memorized_defined=false;with_hard_applied=true;}
		
		long 			period;
		bool 			period_defined;
		Tango::DispLevel 	attr_disp;
		bool 			disp_defined;
		bool			memorized_defined;
		bool			with_hard_applied;
	};
		
protected:
	PyInterpreterState *interp;
	
	void met_exist_callable(string &,string &,PyObject *);
	void create_attr(boost::python::dict &,boost::python::list &,short, PyObject *,vector<Tango::Attr *> &);
	void create_cmd(boost::python::dict &,boost::python::list &,short,PyObject *);
	void analyse_opt(boost::python::dict &,Tango::UserDefaultAttrProp &,disp_poll &,string &);
	void check_number_type(PyObject *,string &,const char *,string &);
	void check_string_type(PyObject *,string &,const char *,string &);
};


//------------------------------------------------------------------------------------------------

enum CmdType
{
	VOID_VOID = 0,
	VOID_OUT,
	IN_VOID,
	IN_OUT
};


class PyCmd : public Tango::Command
{
public:
	PyCmd(string &name, Tango::CmdArgType in, Tango::CmdArgType out,
	      string &in_desc, string &out_desc, Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level),py_allowed_defined(false)	{};

	PyCmd(const char *name, Tango::CmdArgType in, Tango::CmdArgType out)
	:Command(name,in,out),py_allowed_defined(false)	{};

	virtual ~PyCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
	
	void set_allowed(string &name) {py_allowed_defined=true;py_allowed_name=name;}
	void set_cmd_type(CmdType ct) {cmd_type = ct;}
	
private:
	bool py_allowed_defined;
	string	py_allowed_name;
	CmdType cmd_type;
};

//------------------------------------------------------------------------------------------------
class PyAttr
{
public:
	PyAttr():py_allowed_defined(false) {}
	virtual ~PyAttr() {}
	
	void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
	void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
	bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty);

	void set_allowed(string &name) {py_allowed_defined=true;py_allowed_name=name;}
	void set_read_name(string &name) {read_name=name;}
	void set_write_name(string &name) {write_name=name;}
	void set_user_prop(vector<Tango::AttrProperty> &,Tango::UserDefaultAttrProp &);
	
private:
	bool py_allowed_defined;
	string	py_allowed_name;
	string read_name;
	string write_name;	
};


class PyScaAttr: public Tango::Attr,public PyAttr
{
public:
	PyScaAttr(const char *na,long type,Tango::AttrWriteType w):Tango::Attr(na,type,w) {};
	PyScaAttr(const char *,long,Tango::AttrWriteType,vector<Tango::AttrProperty> &);
	~PyScaAttr() {};
	

	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) {return PyAttr::is_allowed(dev,ty);}
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att) {return PyAttr::read(dev,att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {return PyAttr::write(dev,att);}
};

//------------------------------------------------------------------------------------------------


class PySpecAttr: public Tango::SpectrumAttr,public PyAttr
{
public:
	PySpecAttr(const char *na,long type,Tango::AttrWriteType w,long xx):Tango::SpectrumAttr(na,type,w,xx) {}
	PySpecAttr(const char *,long,Tango::AttrWriteType,long,vector<Tango::AttrProperty> &);
	~PySpecAttr() {};

	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) {return PyAttr::is_allowed(dev,ty);}
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att) {return PyAttr::read(dev,att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {return PyAttr::write(dev,att);}
};

//------------------------------------------------------------------------------------------------

class PyImaAttr: public Tango::ImageAttr,public PyAttr
{
public:
	PyImaAttr(const char *na,long type,Tango::AttrWriteType w,long xx,long yy):Tango::ImageAttr(na,type,w,xx,yy) {};
	PyImaAttr(const char *,long,Tango::AttrWriteType,long,long,vector<Tango::AttrProperty> &);
	~PyImaAttr() {};

	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) {return PyAttr::is_allowed(dev,ty);}
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att) {return PyAttr::read(dev,att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {return PyAttr::write(dev,att);}
};

//------------------------------------------------------------------------------------------------

#define CALL_MAC(A,B) \
	switch(in_type) \
	{ \
	case Tango::DEV_BOOLEAN:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_bool);\
		break;\
	case Tango::DEV_SHORT:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_short);\
		break;\
	case Tango::DEV_LONG:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_long);\
		break;\
	case Tango::DEV_FLOAT:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_float);\
		break;\
	case Tango::DEV_DOUBLE:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_double);\
		break;\
	case Tango::DEV_USHORT:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ushort);\
		break;\
	case Tango::DEV_ULONG:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ulong);\
		break;\
	case Tango::DEV_STRING:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_string);\
		break;\
	case Tango::DEV_LONG64:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_long64);\
		break;\
	case Tango::DEV_ULONG64:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ulong64);\
		break;\
	case Tango::DEVVAR_CHARARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_char_array);\
		break;\
	case Tango::DEVVAR_SHORTARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_short_array);\
		break;\
	case Tango::DEVVAR_LONGARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_long_array);\
		break;\
	case Tango::DEVVAR_FLOATARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_float_array);\
		break;\
	case Tango::DEVVAR_DOUBLEARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_double_array);\
		break;\
	case Tango::DEVVAR_USHORTARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ushort_array);\
		break;\
	case Tango::DEVVAR_ULONGARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ulong_array);\
		break;\
	case Tango::DEVVAR_STRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_string_array);\
		break;\
	case Tango::DEVVAR_LONGSTRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_long_string_array);\
		break;\
	case Tango::DEVVAR_DOUBLESTRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_double_string_array);\
		break;\
	case Tango::DEVVAR_LONG64ARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_long64_array);\
		break;\
	case Tango::DEVVAR_ULONG64ARRAY:\
		A = call_method<B>(dev_ptr->m_self,name.c_str(),in_ulong64_array);\
		break;\
	default:\
		break;\
	}


#define CALL_MAC_WIR(A,B) \
	switch(in_type) \
	{ \
	case Tango::DEV_BOOLEAN:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_bool);\
		break;\
	case Tango::DEV_SHORT:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_short);\
		break;\
	case Tango::DEV_LONG:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_long);\
		break;\
	case Tango::DEV_FLOAT:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_float);\
		break;\
	case Tango::DEV_DOUBLE:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_double);\
		break;\
	case Tango::DEV_USHORT:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ushort);\
		break;\
	case Tango::DEV_ULONG:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ulong);\
		break;\
	case Tango::DEV_STRING:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_string);\
		break;\
	case Tango::DEV_LONG64:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_long64);\
		break;\
	case Tango::DEV_ULONG64:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ulong64);\
		break;\
	case Tango::DEVVAR_CHARARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_char_array);\
		break;\
	case Tango::DEVVAR_SHORTARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_short_array);\
		break;\
	case Tango::DEVVAR_LONGARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_long_array);\
		break;\
	case Tango::DEVVAR_FLOATARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_float_array);\
		break;\
	case Tango::DEVVAR_DOUBLEARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_double_array);\
		break;\
	case Tango::DEVVAR_USHORTARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ushort_array);\
		break;\
	case Tango::DEVVAR_ULONGARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ulong_array);\
		break;\
	case Tango::DEVVAR_STRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_string_array);\
		break;\
	case Tango::DEVVAR_LONGSTRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_long_string_array);\
		break;\
	case Tango::DEVVAR_DOUBLESTRINGARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_double_string_array);\
		break;\
	case Tango::DEVVAR_LONG64ARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_long64_array);\
		break;\
	case Tango::DEVVAR_ULONG64ARRAY:\
		A = call_method<B>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str(),in_ulong64_array);\
		break;\
	default:\
		break;\
	}
	
			
#endif // CPPTango::DEVICECLASS
