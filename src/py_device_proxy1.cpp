static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/py_device_proxy1.cpp,v 1.14 2008/05/02 13:45:24 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         py_device_proxy1.cpp
//
// description :  C++ source using BOOST macros to ease mapping
//                between C++ objects and C/C++ (hidden) instructions
//                for embedding thease objects into a Python run time
//                This file introduces the PyTango module and defines
//                 Tango enumerations and exception set.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.14 $
//
// $Log: py_device_proxy1.cpp,v $
// Revision 1.14  2008/05/02 13:45:24  tiagocoutinho
// added version information
//
// Revision 1.13  2008/01/16 11:09:09  tiagocoutinho
// - prepare omni_thread code for future version
//
// Revision 1.12  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.11  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.10  2007/03/07 10:50:55  tiagocoutinho
// - Support for AttributeInfoEx
// - Support for attribute configuration events
//
// Revision 1.9  2006/10/13 12:47:07  taurel
// - Added INIT state
//
// Revision 1.8  2006/04/13 13:40:03  taurel
// Several changes:
// - Add wrapping of the new set/get_transparency_reconnection AttributeProxy class methods
// - Add wrapping of the new AttributeProxy class ctor (with DeviceProxy)
// - Fix bug: Now all Pthon exceptions inherit from PyTango.DevFailed
// - Exception name from WrongSynTax to WrongNameSyntax and Communication to CommunicationFailed to be coherent with C++
// - Change the way attribute data type are managed in AttributeProxy class to speed up read() and write() methods
// - Change the R/W type for some elements of the AttributeInfo class. All of them were read only
//
// Revision 1.7  2006/02/06 14:14:12  taurel
// - Use a Python package in order to hide the TgFactory package from the user point of view
//
// Revision 1.6  2006/01/18 14:26:57  taurel
// - Add a "typename" definition in order to remove compilation warning
// - Change include file order to also remove compiler warnings
// - Now call device server module wrapped stuff
//
// Revision 1.5  2005/06/14 09:09:42  dusdan
// *** empty log message ***
//
// Revision 1.4  2005/06/02 17:10:54  ounsy
// First introduction of group related methods
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

#include <device_proxy.h>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/exception_translator.hpp>
#include <boost/python/scope.hpp>
#include <exception>

#include "conversion_utils.h"

boost::python::object __tango_dev_failed;
static boost::python::object tango_connection_failed;
static boost::python::object tango_communication_failed;
static boost::python::object tango_wrong_syntax;
static boost::python::object tango_non_db_device;
static boost::python::object tango_wrong_data;
static boost::python::object tango_non_supported_feature;
static boost::python::object tango_asyn_call;
static boost::python::object tango_asyn_reply_not_arrived;
static boost::python::object tango_event_system_failed;


void translate_dev_failed(const Tango::DevFailed &dev_failed)
{
	boost::python::tuple py_value = Utils::translate_exception_value(dev_failed.errors);
     	PyErr_SetObject(__tango_dev_failed.ptr(), py_value.ptr());
}

void translate_connection_failed(const Tango::ConnectionFailed &con_failed)
{
	boost::python::tuple py_value = Utils::translate_exception_value(con_failed.errors);
    	PyErr_SetObject(tango_connection_failed.ptr(), py_value.ptr());
}

void translate_communication_failed(const Tango::CommunicationFailed &com_failed)
{
	boost::python::tuple py_value = Utils::translate_exception_value(com_failed.errors);
    	PyErr_SetObject(tango_communication_failed.ptr(), py_value.ptr());
}

void translate_wrong_syntax(const Tango::WrongNameSyntax &wrong_syntax)
{
	boost::python::tuple py_value = Utils::translate_exception_value(wrong_syntax.errors);
    	PyErr_SetObject(tango_wrong_syntax.ptr(), py_value.ptr());
}

void translate_non_db_device(const Tango::NonDbDevice &non_dbdev)
{
	boost::python::tuple py_value = Utils::translate_exception_value(non_dbdev.errors);
    	PyErr_SetObject(tango_non_db_device.ptr(), py_value.ptr());
}

void translate_wrong_data(const Tango::WrongData &wrong_data)
{
	boost::python::tuple py_value = Utils::translate_exception_value(wrong_data.errors);
    	PyErr_SetObject(tango_wrong_data.ptr(), py_value.ptr());
}

void translate_non_supported_feature(const Tango::NonSupportedFeature &non_supported)
{
	boost::python::tuple py_value = Utils::translate_exception_value(non_supported.errors);
    	PyErr_SetObject(tango_non_supported_feature.ptr(), py_value.ptr());
}

void translate_asyn_call(const Tango::AsynCall &asyn_call)
{
	boost::python::tuple py_value = Utils::translate_exception_value(asyn_call.errors);
    	PyErr_SetObject(tango_asyn_call.ptr(), py_value.ptr());
}

void translate_asyn_reply_not_arrived(const Tango::AsynReplyNotArrived &asyn_reply)
{
	boost::python::tuple py_value = Utils::translate_exception_value(asyn_reply.errors);
    	PyErr_SetObject(tango_asyn_reply_not_arrived.ptr(), py_value.ptr());
}

void translate_event_system_failed(const Tango::EventSystemFailed &esf)
{
	boost::python::tuple py_value = Utils::translate_exception_value(esf.errors);
    	PyErr_SetObject(tango_event_system_failed.ptr(), py_value.ptr());
}

void declare_base_types();
void init_module();

BOOST_PYTHON_MODULE(_PyTango)
{
    using namespace boost::python;


	init_module();
	
	scope().attr("__version__") = "3.0.4";
	
	scope().attr("__version_description__") = "This version implements the C++"
			" Tango 6.1.0 API.";
	
	scope().attr("__doc__") =
		"This module implements the Python Tango Device API mapping\n"
		"It allows access from Tango-Python modules to the Tango high level classes\n"
		"and structures :\n"
		"   - DeviceInfo\n"
		"   - CommandInfo\n"
		"   - TimeVal\n"
		"   - DeviceDataHistory\n" 
		"   - AttributeInfo\n"
		"   - AttributeInfoEx\n"
		"   - AttributeAlarmInfo\n"
		"   - ChangeEventInfo\n"
		"   - ChangeEventInfo\n"
		"   - PeriodicEventInfo\n"
		"   - ArchiveEventInfo\n"
		"   - AttributeEventInfo\n"
		"   - AttributeValue\n"
		"   - DeviceAttributeHistory\n"
		"   - DeviceProxy\n"
		"   - DbDevImportInfo\n"
		"   - DbDevInfo\n"
		"   - DbDevImportInfo\n"
		"   - DbDevExportInfo\n"
		"   - Database\n"
		"Additionally, Tango enumerated types are mapped to named python constants\n"
		"as follows, \n"
		"Tango DevState enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - DevState.ON\n"
		"   - DevState.OFF\n"
		"   - DevState.CLOSE\n"
		"   - DevState.OPEN\n"
		"   - DevState.INSERT\n"
		"   - DevState.EXTRACT\n"
		"   - DevState.MOVING\n"
		"   - DevState.STANDBY\n"
		"   - DevState.FAULT\n"
		"   - DevState.INIT\n"
		"   - DevState.RUNNING\n"
		"   - DevState.ALARM\n"
		"   - DevState.DISABLE\n"
		"   - DevState.UNKNOWN\n"
		"Tango DevSource enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - DevSource.DEV\n"
		"   - DevSource.CACHE\n"
		"   - DevSource.CACHE_DEV\n"
		"Tango DispLevel enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - DispLevel.OPERATOR\n"
		"   - DevSource.EXPERT\n"
		"Tango AttrWriteType enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - AttrWriteType.READ\n"
		"   - AttrWriteType.READ_WITH_WRITE\n"
		"   - AttrWriteType.WRITE\n"
		"   - AttrWriteType.READ_WRITE\n"
		"Tango AttrDataFormat enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - AttrDataFormat.SCALAR\n"
		"   - AttrDataFormat.SPECTRUM\n"
		"   - AttrDataFormat.IMAGE\n"
		"Tango AttrQuality enumeration values are mapped to the following integer\n"
		"constants :\n"
		"   - AttrQuality.ATTR_VALID\n"
		"   - AttrQuality.ATTR_INVALID\n"
		"   - AttrQuality.ATTR_ALARM"
		;

    
	enum_<Tango::DevState>("DevState")
     	.value("ON",Tango::ON)
     	.value("OFF",Tango::OFF)
     	.value("CLOSE",Tango::CLOSE)
     	.value("OPEN",Tango::OPEN)
     	.value("INSERT",Tango::INSERT)
     	.value("EXTRACT",Tango::EXTRACT)
     	.value("MOVING",Tango::MOVING)
     	.value("STANDBY",Tango::STANDBY)
     	.value("FAULT",Tango::FAULT)
	.value("INIT",Tango::INIT)
     	.value("RUNNING",Tango::RUNNING)
     	.value("ALARM",Tango::ALARM)
     	.value("DISABLE",Tango::DISABLE)
     	.value("UNKNOWN",Tango::UNKNOWN)
     ;

	enum_<Tango::CmdArgType>("ArgType")
     	.value("DevVoid",Tango::DEV_VOID)
     	.value("DevBoolean",Tango::DEV_BOOLEAN)
     	.value("DevShort",Tango::DEV_SHORT)
     	.value("DevLong",Tango::DEV_LONG)
     	.value("DevFloat",Tango::DEV_FLOAT)
     	.value("DevDouble",Tango::DEV_DOUBLE)
     	.value("DevUShort",Tango::DEV_USHORT)
     	.value("DevULong",Tango::DEV_ULONG)
     	.value("DevString",Tango::DEV_STRING)
     	.value("DevVarCharArray",Tango::DEVVAR_CHARARRAY)
     	.value("DevVarShortArray",Tango::DEVVAR_SHORTARRAY)
     	.value("DevVarLongArray",Tango::DEVVAR_LONGARRAY)
     	.value("DevVarFloatArray",Tango::DEVVAR_FLOATARRAY)
     	.value("DevVarDoubleArray",Tango::DEVVAR_DOUBLEARRAY)
     	.value("DevVarUShortArray",Tango::DEVVAR_USHORTARRAY)
     	.value("DevVarULongArray",Tango::DEVVAR_ULONGARRAY)
     	.value("DevVarStringArray",Tango::DEVVAR_STRINGARRAY)
     	.value("DevVarLongStringArray",Tango::DEVVAR_LONGSTRINGARRAY)
     	.value("DevVarDoubleStringArray",Tango::DEVVAR_DOUBLESTRINGARRAY)
     	.value("DevState",Tango::DEV_STATE)
     	.value("ConstDevString",Tango::CONST_DEV_STRING)
     	.value("DevVarBooleanArray",Tango::DEVVAR_BOOLEANARRAY)
     	.value("DevUChar",Tango::DEV_UCHAR)
     	.value("DevLong64",Tango::DEV_LONG64)
     	.value("DevULong64",Tango::DEV_ULONG64)
     	.value("DevVarLong64Array",Tango::DEVVAR_LONG64ARRAY)
     	.value("DevVarULong64Array",Tango::DEVVAR_ULONG64ARRAY)
     ;

     enum_<Tango::DevSource>("DevSource")
     	.value("DEV",Tango::DEV)
     	.value("CACHE",Tango::CACHE)
     	.value("CACHE_DEV",Tango::CACHE_DEV)
     ;


     enum_<Tango::DispLevel>("DispLevel")
     	.value("OPERATOR",Tango::OPERATOR)
     	.value("EXPERT",Tango::EXPERT)
     ;


     enum_<Tango::AttrWriteType>("AttrWriteType")
     	.value("READ",Tango::READ)
     	.value("READ_WITH_WRITE",Tango::READ_WITH_WRITE)
     	.value("WRITE",Tango::WRITE)
     	.value("READ_WRITE",Tango::READ_WRITE)
     ;


     enum_<Tango::AttrDataFormat>("AttrDataFormat")
     	.value("SCALAR",Tango::SCALAR)
     	.value("SPECTRUM",Tango::SPECTRUM)
     	.value("IMAGE",Tango::IMAGE)
     ;


     enum_<Tango::AttrQuality>("AttrQuality")
     	.value("ATTR_VALID",Tango::ATTR_VALID)
     	.value("ATTR_INVALID",Tango::ATTR_INVALID)
     	.value("ATTR_ALARM",Tango::ATTR_ALARM)
     	.value("ATTR_CHANGING",Tango::ATTR_CHANGING)
     	.value("ATTR_WARNING",Tango::ATTR_WARNING)
     ;


     enum_<Tango::asyn_req_type>("asyn_req_type")
     	.value("POLLING",Tango::POLLING)
     	.value("CALLBACK",Tango::CALL_BACK)
     	.value("ALL_ASYNCH",Tango::ALL_ASYNCH)
     ;

     /* VF 21/03/2005*/
     enum_<Tango::EventType>("EventType")
	     .value("CHANGE", Tango::CHANGE_EVENT)
	     .value("QUALITY", Tango::QUALITY_EVENT)
	     .value("PERIODIC", Tango::PERIODIC_EVENT)
	     .value("ARCHIVE", Tango::ARCHIVE_EVENT)
	     .value("USER", Tango::USER_EVENT)
	     .value("ATTR_CONF_EVENT", Tango::ATTR_CONF_EVENT)
     ;	 

     enum_<Tango::cb_sub_model>("cb_sub_model")
	     .value("PUSH_CALLBACK", Tango::PUSH_CALLBACK)
	     .value("PULL_CALLBACK", Tango::PULL_CALLBACK)
     ;
     /* */
     
     __tango_dev_failed = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.DevFailed",NULL,NULL)));
     scope().attr("DevFailed") = __tango_dev_failed;	
	register_exception_translator<Tango::DevFailed>(&translate_dev_failed);

     tango_connection_failed = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.ConnectionFailed",__tango_dev_failed.ptr(),NULL)));
     scope().attr("ConnectionFailed") = tango_connection_failed;	
     register_exception_translator<Tango::ConnectionFailed>(&translate_connection_failed);

     tango_communication_failed = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.CommunicationFailed",__tango_dev_failed.ptr(),NULL)));
     scope().attr("CommunicationFailed") = tango_communication_failed;	
     register_exception_translator<Tango::CommunicationFailed>(&translate_communication_failed);

     tango_wrong_syntax = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.WrongNameSyntax",__tango_dev_failed.ptr(),NULL)));
     scope().attr("WrongNameSyntax") = tango_wrong_syntax;	
     register_exception_translator<Tango::WrongNameSyntax>(&translate_wrong_syntax);

     tango_non_db_device = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.NonDbDevice",__tango_dev_failed.ptr(),NULL)));
     scope().attr("NonDbDevice") = tango_non_db_device;	
     register_exception_translator<Tango::NonDbDevice>(&translate_non_db_device);

     tango_wrong_data = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.WrongData",__tango_dev_failed.ptr(),NULL)));
     scope().attr("WrongData") = tango_wrong_data;	
     register_exception_translator<Tango::WrongData>(&translate_wrong_data);

     tango_non_supported_feature = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.NonSupportedFeature",__tango_dev_failed.ptr(),NULL)));
     scope().attr("NonSupportedFeature") = tango_non_supported_feature;	
     register_exception_translator<Tango::NonSupportedFeature>(&translate_non_supported_feature);

     tango_asyn_call = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.AsynCall",__tango_dev_failed.ptr(),NULL)));
     scope().attr("AsynCall") = tango_asyn_call;	
     register_exception_translator<Tango::AsynCall>(&translate_asyn_call);

     tango_asyn_reply_not_arrived = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.AsynReplyNotArrived",__tango_dev_failed.ptr(),NULL)));
     scope().attr("AsynReplyNotArrived") = tango_asyn_reply_not_arrived;	
     register_exception_translator<Tango::AsynReplyNotArrived>(&translate_asyn_reply_not_arrived);

     tango_event_system_failed = boost::python::object(
            boost::python::handle<>(PyErr_NewException("PyTango.EventSystemFailed",__tango_dev_failed.ptr(),NULL)));
     scope().attr("EventSystemFailed") = tango_event_system_failed;	
     register_exception_translator<Tango::EventSystemFailed>(&translate_event_system_failed);

     
     //def("_create_omni_dummy", __create_omni_dummy);
     //def("_release_omni_dummy", __release_omni_dummy);
     
     declare_base_types();
}



























