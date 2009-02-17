static const char *RcsId = "$Id: py_ds.cpp,v 1.27 2008/05/06 11:52:27 tiagocoutinho Exp $\n$Name:  $";

//+============================================================================
//
// file :               py_ds.cpp
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.27 $
//
// $Log: py_ds.cpp,v $
// Revision 1.27  2008/05/06 11:52:27  tiagocoutinho
// Tango 6.1 implementation.
// Added documentation related to PyTango release
//
// Revision 1.26  2008/05/02 13:12:30  tiagocoutinho
// Added missing methods to PyTango::Util :
// - get_ds_inst_name
// - get_ds_exec_name
// - get_ds_name
// - get_host_name
// - get_pid_str
// - get_tango_lib_release
// - get_version_str
// - get_ds_name
//
// Revision 1.25  2008/03/04 12:04:55  tiagocoutinho
// added adm_name() to DeviceImpl
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
// Revision 1.23  2007/06/08 09:23:46  taurel
// - Add a more precise test before deciding that we are in a debugger
//
// Revision 1.22  2007/05/30 15:16:17  taurel
// - Fix bug in case of heavy used of events (differents threads created
// by the ORB could executed the callback)
// - Fix a bug in case an event is pushed while the main thread is doing
// an unsubscribe_event on the same event
//
// Revision 1.21  2007/05/18 10:38:10  taurel
// - Change the way the get_device_xxx methods are coded
//
// Revision 1.20  2007/05/11 07:32:11  taurel
// - Added wrapping of some new methods (Database::put_device_alias and
// delete_device_alias and DeviceClass::device_destroyer)
//
// Revision 1.19  2007/04/02 07:41:59  tiagocoutinho
// - client Group feature
//
// Revision 1.18  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.17  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.16  2006/12/22 07:53:25  taurel
// - Fix a compilation problem with gcc 4.1
//
// Revision 1.15  2006/12/05 08:24:47  taurel
// - Automatic management of reference count increase for BOOST
// - min_warning and max-waarning now supported
//
// Revision 1.14  2006/07/26 09:50:15  taurel
// - Simply remove some commented out lines
//
// Revision 1.13  2006/07/26 07:06:43  taurel
// - Add wrapping of all the new calls added in Tango V5.5 to manually firing events
//
// Revision 1.12  2006/05/17 15:56:36  taurel
// - Changes due to the Windows port using XP and VC8 (Visual Studio 2005)
//
// Revision 1.11  2006/04/28 07:17:29  taurel
// - Trying to fix some portability issue with Solaris
//
// Revision 1.10  2006/04/27 09:19:21  taurel
// - Add some PyErr_Clear() call to be on the safest way
//
// Revision 1.9  2006/03/27 08:50:48  taurel
// - Add the Deviceproxy.alisa() method
// - Fix a 32/64 bits problem in the wrapping of the "at" method  (for DS)
//
// Revision 1.8  2006/03/03 10:56:19  taurel
// - Add code to be able to run a Tango device server under Python debugger
//
// Revision 1.7  2006/02/07 13:51:35  taurel
// - Miscellaneous changes to even simplify the way to write Tango DS in python
//
// Revision 1.6  2006/02/06 14:14:12  taurel
// - Use a Python package in order to hide the TgFactory package from the user point of view
//
// Revision 1.5  2006/01/30 15:36:24  taurel
// - Add the vector<DeviceImpl *>::size() method
//
// Revision 1.4  2006/01/27 12:52:51  taurel
// - Remove C++ vector used on client side between CORBA sequences and Python list
// - Replace list by tuple on server side for DevVarLongStringArray and DevVarDoubleStringArray
// - Add defualt command case in Py DS
//
// Revision 1.3  2006/01/24 15:33:56  taurel
// - Change the way how Device::delete_dev() is implemented. The old one
// introduced a severe compatibility problem.
// - Fix bug in rw_attr.cpp file in the set_attribute_date_quality family
// functions
//
// Revision 1.2  2006/01/19 11:54:04  taurel
// - Small changes to keep compiler quiet (some warning)
//
// Revision 1.1  2006/01/18 14:47:33  taurel
// - Added files when adding device server features in the Python binding
//
// Revision 1.17  2006/01/12 11:41:17  etaurel
// - Changes to have only one Python module instead of PyTango and PyTangoDs
//
// Revision 1.16  2006/01/10 11:35:28  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.15  2006/01/10 09:38:54  etaurel
// - Change the way python lock is managed
//
// Revision 1.14  2006/01/04 12:59:53  etaurel
// - Other changes to add the add_attribute feature
//
// Revision 1.13  2006/01/03 10:16:16  etaurel
// - Commit after many changes and beautifullying code
//
// Revision 1.12  2005/12/22 13:36:43  etaurel
// - Now, all the asyn tests run correctly
//
// Revision 1.11  2005/12/20 16:40:49  etaurel
// - Many changes in locking mechanism
// - Fix bug in PyAttr ctor
// - Make some new methods known to python (via Boost)
//
// Revision 1.10  2005/12/15 11:35:04  etaurel
// - Device Server without database now works
//
// Revision 1.9  2005/12/14 15:44:07  etaurel
// - Now the AttrWrite test sequence is running well
//
// Revision 1.8  2005/12/14 10:24:15  etaurel
// - Now, the AttrMisc test file pass correctly
//
// Revision 1.7  2005/12/13 17:23:11  etaurel
// - Before I leave
//
// Revision 1.6  2005/12/13 15:59:55  etaurel
// - The Attr.tst old Tango test system test file run without errors
//
// Revision 1.5  2005/12/13 11:52:01  etaurel
// - Fix a bug in set_attribute_value for Double
//
// Revision 1.4  2005/12/13 10:47:40  etaurel
// - Commit before beginning testing attribute
//
// Revision 1.3  2005/12/12 15:35:38  etaurel
// - Miscellanous addition when porting test suite
//
// Revision 1.2  2005/12/12 10:44:10  etaurel
// - First release with Restart and CTRL-C running
//
// Revision 1.1.1.1  2005/12/02 13:44:34  etaurel
// Initial import
//
//
//
// copyleft :           CELLS - ALBA
//			Campus Universitari de Bellaterra. Universitat 
//			Aut�noma de Barcelona
//			08193 Bellaterra, Barcelona
//		  	SPAIN			
//
//-============================================================================


#include <py_ds.h>
#include <from_py.h>
#include <rw_attr.h>
#include <py_ev.h>

//
// The per thread data storage key
//

#ifdef WIN32
__declspec(dllimport) omni_thread::key_t key_py_data;
#else
extern omni_thread::key_t key_py_data;
#endif

//
// Defined in original PyTango 
//

extern boost::python::object __tango_dev_failed;


//+-------------------------------------------------------------------------
//
// method : 		PyCmd::is_allowed
// 
// description : 	Decide if it is allowed to execute the command
//
// argin : - dev : The device on which the command has to be excuted
//	   - any : The input data
//
// This method returns a boolean set to True if it is allowed to execute
// the command. Otherwise, returns false
//
//--------------------------------------------------------------------------


bool PyCmd::is_allowed(Tango::DeviceImpl *dev, const CORBA::Any &any)
{
	if (py_allowed_defined == true)
	{
		Device_3ImplWrap *dev_ptr = static_cast<Device_3ImplWrap *>(dev);
		
		omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
		Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
		lock_ptr->Get();

		bool returned_value = true;
		try
		{
			returned_value = call_method<bool>(dev_ptr->m_self,py_allowed_name.c_str());
		}
		catch(error_already_set)
		{
			PyObject *ex_exec,*ex_value,*ex_tb;
			if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
			{
				PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
				Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
			}
			else
			{
				PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
				Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
			}
		}

		lock_ptr->Release();
					
		return returned_value;
	}
	else
		return true;
}



//+-------------------------------------------------------------------------
//
// method : 		PyCmd::execute
// 
// description : 	Unpack the input data from the CORBA Any, execute
//			the requested command and pack the output parammeters
//			in a new CORBA Any object
//
// argin : - dev : The device on which the command has to be excuted
//	   - any : The input data
//
// This method returns a pointer to the Any used to sent back dara to the
// command caller
//
//--------------------------------------------------------------------------

CORBA::Any *PyCmd::execute (Tango::DeviceImpl *dev, const CORBA::Any &any)
{
	Device_3ImplWrap *dev_ptr = static_cast<Device_3ImplWrap *>(dev);
	
	Tango::DevBoolean in_bool,out_bool;
	Tango::DevShort in_short,out_short;
	Tango::DevLong in_long,out_long;
	Tango::DevFloat in_float,out_float;
	Tango::DevDouble in_double,out_double;
	Tango::DevUShort in_ushort,out_ushort;
	Tango::DevULong in_ulong,out_ulong;
	Tango::ConstDevString in_string;
	Tango::ConstDevString out_string = NULL;
	Tango::DevLong64 in_long64,out_long64;
	Tango::DevULong64 in_ulong64,out_ulong64;

	const Tango::DevVarCharArray *in_char_array;
	const Tango::DevVarShortArray *in_short_array;
	const Tango::DevVarLongArray *in_long_array;
	const Tango::DevVarFloatArray *in_float_array;
	const Tango::DevVarDoubleArray *in_double_array;
	const Tango::DevVarUShortArray *in_ushort_array;
	const Tango::DevVarULongArray *in_ulong_array;
	const Tango::DevVarStringArray *in_string_array;
	const Tango::DevVarLongStringArray *in_long_string_array;
	const Tango::DevVarDoubleStringArray *in_double_string_array;
	const Tango::DevVarLong64Array *in_long64_array;
	const Tango::DevVarULong64Array *in_ulong64_array;
	
	Tango::DevVarCharArray *out_char_array = NULL;
	Tango::DevVarShortArray *out_short_array = NULL;
	Tango::DevVarLongArray *out_long_array = NULL;
	Tango::DevVarFloatArray *out_float_array = NULL;
	Tango::DevVarDoubleArray *out_double_array = NULL;
	Tango::DevVarUShortArray *out_ushort_array = NULL;
	Tango::DevVarULongArray *out_ulong_array = NULL;
	Tango::DevVarStringArray *out_string_array = NULL;
	Tango::DevVarLongStringArray *out_long_string_array = NULL;
	Tango::DevVarDoubleStringArray *out_double_string_array = NULL;
	Tango::DevVarLong64Array *out_long64_array = NULL;
	Tango::DevVarULong64Array *out_ulong64_array = NULL;
		
	Tango::DevState in_state,out_state;

//
// Some default init to keep compiler quiet
//

	out_bool = true;
	out_short = -99;
	out_long = -99;
	out_float = -99.9;
	out_double = -99.9;
	out_ushort = 99;
	out_ulong = 99;
	out_long64 = -99;
	out_ulong64 = 99;
	out_state = Tango::UNKNOWN;
	
//
// Manage the command input data (if any)
//

	if ((cmd_type == IN_OUT) || (cmd_type == IN_VOID))
	{	
		switch (in_type)
		{
		case Tango::DEV_BOOLEAN:
			extract(any,in_bool);
			break;
			
		case Tango::DEV_SHORT:
			extract(any,in_short);

			break;
		case Tango::DEV_LONG:
			extract(any,in_long);
			break;

		case Tango::DEV_FLOAT:
			extract(any,in_float);
			break;

		case Tango::DEV_DOUBLE:
			extract(any,in_double);
			break;
			
		case Tango::DEV_USHORT:
			extract(any,in_ushort);
			break;
			
		case Tango::DEV_ULONG:
			extract(any,in_ulong);
			break;
			
		case Tango::DEV_STRING:
			extract(any,in_string);
			break;

		case Tango::DEV_LONG64:
			extract(any,in_long64);
			break;

		case Tango::DEV_ULONG64:
			extract(any,in_ulong64);
			break;
			
		case Tango::DEVVAR_CHARARRAY:
			extract(any,in_char_array);
			break;
			
		case Tango::DEVVAR_SHORTARRAY:
			extract(any,in_short_array);
			break;

		case Tango::DEVVAR_LONGARRAY:
			extract(any,in_long_array);
			break;

		case Tango::DEVVAR_FLOATARRAY:
			extract(any,in_float_array);
			break;
			
		case Tango::DEVVAR_DOUBLEARRAY:
			extract(any,in_double_array);
			break;
						
		case Tango::DEVVAR_USHORTARRAY:
			extract(any,in_ushort_array);
			break;
			
		case Tango::DEVVAR_ULONGARRAY:
			extract(any,in_ulong_array);
			break;
			
		case Tango::DEVVAR_STRINGARRAY:
			extract(any,in_string_array);
			break;

		case Tango::DEVVAR_LONGSTRINGARRAY:
			extract(any,in_long_string_array);
			break;
			
		case Tango::DEVVAR_DOUBLESTRINGARRAY:
			extract(any,in_double_string_array);
			break;
		
		case Tango::DEVVAR_LONG64ARRAY:
			extract(any,in_long64_array);
			break;
			
		case Tango::DEVVAR_ULONG64ARRAY:
			extract(any,in_ulong64_array);
			break;
		
		case Tango::DEV_STATE:
			extract(any,in_state);
			break;		
						
		default:
			break;
		}
	}

//
// Call command method but first, get Python global lock
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		switch (cmd_type)
		{
		case VOID_VOID :
		
//
// A command without inout args nor output
//

			call_method<void>(dev_ptr->m_self,name.c_str());
			break;
		
		case IN_VOID:
		
//
// A command with input args but without output args
//

			switch (in_type)
			{
			case Tango::DEV_BOOLEAN:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_bool);
				break;

			case Tango::DEV_SHORT:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_short);
				break;
				
			case Tango::DEV_LONG:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_long);
				break;

			case Tango::DEV_FLOAT:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_float);
				break;

			case Tango::DEV_DOUBLE:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_double);
				break;

			case Tango::DEV_USHORT:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ushort);
				break;
			
			case Tango::DEV_ULONG:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ulong);
				break;

			case Tango::DEV_STRING:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_string);
				break;

			case Tango::DEV_LONG64:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_long64);
				break;

			case Tango::DEV_ULONG64:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ulong64);
				break;
				
			case Tango::DEVVAR_CHARARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_char_array);
				break;

			case Tango::DEVVAR_SHORTARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_short_array);
				break;

			case Tango::DEVVAR_LONGARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_long_array);
				break;

			case Tango::DEVVAR_FLOATARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_float_array);
				break;

			case Tango::DEVVAR_DOUBLEARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_double_array);
				break;

			case Tango::DEVVAR_USHORTARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ushort_array);
				break;
			
			case Tango::DEVVAR_ULONGARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ulong_array);
				break;

			case Tango::DEVVAR_STRINGARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_string_array);
				break;

			case Tango::DEVVAR_LONGSTRINGARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_long_string_array);
				break;

			case Tango::DEVVAR_DOUBLESTRINGARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_double_string_array);
				break;

			case Tango::DEVVAR_LONG64ARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_long64_array);
				break;

			case Tango::DEVVAR_ULONG64ARRAY:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_ulong64_array);
				break;
												
			case Tango::DEV_STATE:
				call_method<void>(dev_ptr->m_self,name.c_str(),in_state);
				break;
				
			default:
				break;
			}
			
		case VOID_OUT:
		
//
// A command with output args but without input args
//

			switch (out_type)
			{
			case Tango::DEV_BOOLEAN:
				out_bool = call_method<Tango::DevBoolean>(dev_ptr->m_self,name.c_str());
				break;

			case Tango::DEV_SHORT:
				out_short = call_method<Tango::DevShort>(dev_ptr->m_self,name.c_str());
				break;
				
			case Tango::DEV_LONG:
				out_long = call_method<Tango::DevLong>(dev_ptr->m_self,name.c_str());
				break;

			case Tango::DEV_FLOAT:
				out_float = call_method<Tango::DevFloat>(dev_ptr->m_self,name.c_str());
				break;

			case Tango::DEV_DOUBLE:
				out_double = call_method<Tango::DevDouble>(dev_ptr->m_self,name.c_str());
				break;

			case Tango::DEV_USHORT:
				out_ushort = call_method<Tango::DevUShort>(dev_ptr->m_self,name.c_str());
				break;
			
			case Tango::DEV_ULONG:
				out_ulong = call_method<Tango::DevULong>(dev_ptr->m_self,name.c_str());
				break;

			case Tango::DEV_STRING:
				out_string = call_method<Tango::DevString>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEV_LONG64:
				out_long64 = call_method<Tango::DevLong64>(dev_ptr->m_self,name.c_str());
				break;				

			case Tango::DEV_ULONG64:
				out_ulong64 = call_method<Tango::DevULong64>(dev_ptr->m_self,name.c_str());
				break;				

			case Tango::DEVVAR_CHARARRAY:
				out_char_array = call_method<Tango::DevVarCharArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_SHORTARRAY:
				out_short_array = call_method<Tango::DevVarShortArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_LONGARRAY:
				out_long_array = call_method<Tango::DevVarLongArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_FLOATARRAY:
				out_float_array = call_method<Tango::DevVarFloatArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_DOUBLEARRAY:
				out_double_array = call_method<Tango::DevVarDoubleArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_USHORTARRAY:
				out_ushort_array = call_method<Tango::DevVarUShortArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;
			
			case Tango::DEVVAR_ULONGARRAY:
				out_ulong_array = call_method<Tango::DevVarULongArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_STRINGARRAY:
				out_string_array = call_method<Tango::DevVarStringArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_LONGSTRINGARRAY:
				out_long_string_array = call_method<Tango::DevVarLongStringArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_DOUBLESTRINGARRAY:
				out_double_string_array = call_method<Tango::DevVarDoubleStringArray *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;

			case Tango::DEVVAR_LONG64ARRAY:
				out_long64_array = call_method<Tango::DevVarLong64Array *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;
				
			case Tango::DEVVAR_ULONG64ARRAY:
				out_ulong64_array = call_method<Tango::DevVarULong64Array *>(dev_ptr->m_self,PY_MET_INC_REF,name.c_str());
				break;
								
			case Tango::DEV_STATE:
				out_state = call_method<Tango::DevState>(dev_ptr->m_self,name.c_str());
				
			default:
				break;
			}
			break;		
			
		case IN_OUT:
		
//
// A command with input and output args
//

			switch (out_type)
			{
			case Tango::DEV_BOOLEAN:
				CALL_MAC(out_bool,Tango::DevBoolean);
				break;

			case Tango::DEV_SHORT:
				CALL_MAC(out_short,Tango::DevShort);
				break;
				
			case Tango::DEV_LONG:
				CALL_MAC(out_long,Tango::DevLong);
				break;

			case Tango::DEV_FLOAT:
				CALL_MAC(out_float,Tango::DevFloat);
				break;

			case Tango::DEV_DOUBLE:
				CALL_MAC(out_double,Tango::DevDouble);
				break;

			case Tango::DEV_USHORT:
				CALL_MAC(out_ushort,Tango::DevUShort);
				break;
			
			case Tango::DEV_ULONG:
				CALL_MAC(out_ulong,Tango::DevULong);
				break;

			case Tango::DEV_STRING:
				CALL_MAC_WIR(out_string,Tango::DevString);
				break;
				
			case Tango::DEV_LONG64:
				CALL_MAC(out_long64,Tango::DevLong64);
				break;				

			case Tango::DEV_ULONG64:
				CALL_MAC(out_ulong64,Tango::DevULong64);
				break;				

			case Tango::DEVVAR_CHARARRAY:
				CALL_MAC_WIR(out_char_array,Tango::DevVarCharArray *);
				break;

			case Tango::DEVVAR_SHORTARRAY:
				CALL_MAC_WIR(out_short_array,Tango::DevVarShortArray *);
				break;

			case Tango::DEVVAR_LONGARRAY:
				CALL_MAC_WIR(out_long_array,Tango::DevVarLongArray *);
				break;

			case Tango::DEVVAR_FLOATARRAY:
				CALL_MAC_WIR(out_float_array,Tango::DevVarFloatArray *);
				break;

			case Tango::DEVVAR_DOUBLEARRAY:
				CALL_MAC_WIR(out_double_array,Tango::DevVarDoubleArray *);
				break;

			case Tango::DEVVAR_USHORTARRAY:
				CALL_MAC_WIR(out_ushort_array,Tango::DevVarUShortArray *);
				break;
			
			case Tango::DEVVAR_ULONGARRAY:
				CALL_MAC_WIR(out_ulong_array,Tango::DevVarULongArray *);
				break;

			case Tango::DEVVAR_STRINGARRAY:
				CALL_MAC_WIR(out_string_array,Tango::DevVarStringArray *);
				break;

			case Tango::DEVVAR_LONGSTRINGARRAY:
				CALL_MAC_WIR(out_long_string_array,Tango::DevVarLongStringArray *);
				break;

			case Tango::DEVVAR_DOUBLESTRINGARRAY:
				CALL_MAC_WIR(out_double_string_array,Tango::DevVarDoubleStringArray *);
				break;

			case Tango::DEVVAR_LONG64ARRAY:
				CALL_MAC_WIR(out_long64_array,Tango::DevVarLong64Array *);
				break;
		
			case Tango::DEVVAR_ULONG64ARRAY:
				CALL_MAC_WIR(out_ulong64_array,Tango::DevVarULong64Array *);
				break;	
								
			case Tango::DEV_STATE:
				CALL_MAC(out_state,Tango::DevState);
				
			default:
				break;
			}
			break;
			

		}
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}
			
//
// Manage the command return value (if any)
//
		
	CORBA::Any *ret_any = NULL;

	if ((cmd_type == IN_OUT) || (cmd_type == VOID_OUT))
	{
		switch (out_type)
		{
		case Tango::DEV_BOOLEAN:
			ret_any = insert(out_bool);
			break;
			
		case Tango::DEV_SHORT:
			ret_any = insert(out_short);
			break;
			
		case Tango::DEV_LONG:
			ret_any = insert(out_long);
			break;

		case Tango::DEV_FLOAT:
			ret_any = insert(out_float);
			break;

		case Tango::DEV_DOUBLE:
			ret_any = insert(out_double);
			break;
			
		case Tango::DEV_USHORT:
			ret_any = insert(out_ushort);
			break;
			
		case Tango::DEV_ULONG:
			ret_any = insert(out_ulong);
			break;
			
		case Tango::DEV_STRING:
			ret_any = insert(out_string);
			break;

		case Tango::DEV_LONG64:
			ret_any = insert(out_long64);
			break;

		case Tango::DEV_ULONG64:
			ret_any = insert(out_ulong64);
			break;
			
		case Tango::DEVVAR_CHARARRAY:
			ret_any = insert(*out_char_array);
			break;
			
		case Tango::DEVVAR_SHORTARRAY:
			ret_any = insert(*out_short_array);
			break;

		case Tango::DEVVAR_LONGARRAY:
			ret_any = insert(*out_long_array);
			break;

		case Tango::DEVVAR_FLOATARRAY:
			ret_any = insert(*out_float_array);
			break;
			
		case Tango::DEVVAR_DOUBLEARRAY:
			ret_any = insert(*out_double_array);
			break;
						
		case Tango::DEVVAR_USHORTARRAY:
			ret_any = insert(*out_ushort_array);
			break;
			
		case Tango::DEVVAR_ULONGARRAY:
			ret_any = insert(*out_ulong_array);
			break;
			
		case Tango::DEVVAR_STRINGARRAY:
			ret_any = insert(*out_string_array);
			break;

		case Tango::DEVVAR_LONGSTRINGARRAY:
			ret_any = insert(*out_long_string_array);
			break;
			
		case Tango::DEVVAR_DOUBLESTRINGARRAY:
			ret_any = insert(*out_double_string_array);
			break;

		case Tango::DEVVAR_LONG64ARRAY:
			ret_any = insert(*out_long64_array);
			break;

		case Tango::DEVVAR_ULONG64ARRAY:
			ret_any = insert(*out_ulong64_array);
			break;
			
		case Tango::DEV_STATE:
			ret_any = insert(out_state);
			break;
			
		default:
			break;
		}
	}
	else
		ret_any = new CORBA::Any();	
		
	lock_ptr->Release();
	
	return ret_any;
}


//+-------------------------------------------------------------------------
//
// method : 		PyAttr::is_allowed
// 
// description : 	Decide if it is allowed to read/write the attribute
//
// argin : - dev : The device on which the attribute has to be read/written
//	   - ty : The requets type (read or write)
//
// This method returns a boolean set to True if it is allowed to execute
// the command. Otherwise, returns false
//
//--------------------------------------------------------------------------


bool PyAttr::is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
{

	if (py_allowed_defined == true)
	{
		Device_3ImplWrap *dev_ptr = static_cast<Device_3ImplWrap *>(dev);

		omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
		Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
		lock_ptr->Get();
		
		bool returned_value = true;
		try
		{
			returned_value = call_method<bool>(dev_ptr->m_self,py_allowed_name.c_str(),ty);
		}
		catch(error_already_set)
		{
			PyObject *ex_exec,*ex_value,*ex_tb;
			if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
			{
				PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
				Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
			}
			else
			{
				PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
				Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
			}
		}

		lock_ptr->Release();
					
		return returned_value;
	}
	else
		return true;
}


//+-------------------------------------------------------------------------
//
// method : 		PyAttr::read
// 
// description : 	Read one attribute. This method forward the action
//			to the python method.
//
// argin : - dev : The device on which the attribute has to be read
//	   - att : The attribute
//
//--------------------------------------------------------------------------



void PyAttr::read(Tango::DeviceImpl *dev,Tango::Attribute &att)
{

	Device_3ImplWrap *dev_ptr = static_cast<Device_3ImplWrap *>(dev);

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		call_method<void>(dev_ptr->m_self,read_name.c_str(),boost::ref(att));
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}


//+-------------------------------------------------------------------------
//
// method : 		PyAttr::write
// 
// description : 	Write one attribute. This method forward the action
//			to the python method.
//
// argin : - dev : The device on which the attribute has to be written
//	   - att : The attribute
//
//--------------------------------------------------------------------------


void PyAttr::write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
{

	Device_3ImplWrap *dev_ptr = static_cast<Device_3ImplWrap *>(dev);

//
// The write call can be called by the main thread in case of memorized attribute
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		call_method<void>(dev_ptr->m_self,write_name.c_str(),boost::ref(att));
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}

//+-------------------------------------------------------------------------
//
// method : 		PyAttr::set_user_prop
// 
// description : 	Transfer user property from a vector of AttrProperty
//			to a UserDefaultAttrProp
//
// argin : - user_prop : The AttrProperty vector
//	   - def_prop : The UserDefaultAttrProp instance
//
//--------------------------------------------------------------------------

void PyAttr::set_user_prop(vector<Tango::AttrProperty> &user_prop,Tango::UserDefaultAttrProp &def_prop)
{

//
// Is there any user defined prop. defined ?
//

	long nb_prop = user_prop.size();
	if (nb_prop != 0)
	{
		for (long loop = 0;loop < nb_prop;loop++)
		{
			Tango::AttrProperty  prop = user_prop[loop];
			string &prop_name = prop.get_name();
			string &prop_value = prop.get_value();

			if (prop_name == "label")
				def_prop.set_label(prop_value.c_str());
			else if (prop_name == "description")
				def_prop.set_description(prop_value.c_str());
			else if (prop_name == "unit")
				def_prop.set_unit(prop_value.c_str());
			else if (prop_name == "standard_unit")
				def_prop.set_standard_unit(prop_value.c_str());
			else if (prop_name == "display_unit")
				def_prop.set_display_unit(prop_value.c_str());
			else if (prop_name == "format")
				def_prop.set_format(prop_value.c_str());
			else if (prop_name == "min_value")
				def_prop.set_min_value(prop_value.c_str());
			else if (prop_name == "max_value")
				def_prop.set_max_value(prop_value.c_str());
			else if (prop_name == "min_alarm")
				def_prop.set_min_alarm(prop_value.c_str());
			else if (prop_name == "max_alarm")
				def_prop.set_max_alarm(prop_value.c_str());
			else if (prop_name == "min_warning")
				def_prop.set_min_warning(prop_value.c_str());
			else if (prop_name == "max_warning")
				def_prop.set_max_warning(prop_value.c_str());
			else if (prop_name == "delta_val")
				def_prop.set_delta_val(prop_value.c_str());
			else if (prop_name == "delta_t")
				def_prop.set_delta_t(prop_value.c_str());
			else if (prop_name == "abs_change")
				def_prop.set_abs_change(prop_value.c_str());
			else if (prop_name == "rel_change")
				def_prop.set_rel_change(prop_value.c_str());
			else if (prop_name == "period")
				def_prop.set_period(prop_value.c_str());
			else if (prop_name == "archive_abs_change")
				def_prop.set_archive_abs_change(prop_value.c_str());
			else if (prop_name == "archive_rel_change")
				def_prop.set_archive_rel_change(prop_value.c_str());
			else if (prop_name == "archive_period")
				def_prop.set_archive_period(prop_value.c_str());
		}
		
	}
}


//-------------------------------------------------------------------------------------------
//
//			BOOST data type converters
//
//	Some of them are defined as a template structures in a file called py_ds.h
//	The from_python converters are defined in the from_py.cpp and from_py.h files
//	except the onr for DevFailed exception
//
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
//
//	Some to_python converters
//
//-------------------------------------------------------------------------------------------


struct CORBA_LongString_to_python
{
	static PyObject *convert(Tango::DevVarLongStringArray const &str_s)
	{
		boost::python::list the_string_list;
		for (unsigned int i = 0;i < str_s.svalue.length();i++)
		{
			the_string_list.append(str_s.svalue[i]);
		}
		
		boost::python::list the_long_list;
		for (unsigned int i = 0;i < str_s.lvalue.length();i++)
		{
			the_long_list.append(str_s.lvalue[i]);
		}
				
		boost::python::tuple tup = boost::python::make_tuple(the_long_list,the_string_list);
		
		return incref(tup.ptr());
	}
};


struct CORBA_DoubleString_to_python
{
	static PyObject *convert(Tango::DevVarDoubleStringArray const &str_s)
	{
		boost::python::list the_string_list;
		for (unsigned int i = 0;i < str_s.svalue.length();i++)
		{
			the_string_list.append(str_s.svalue[i]);
		}
		
		boost::python::list the_double_list;
		for (unsigned int i = 0;i < str_s.dvalue.length();i++)
		{
			the_double_list.append(str_s.dvalue[i]);
		}
				
		boost::python::tuple tup = boost::python::make_tuple(the_double_list,the_string_list);
		
		return incref(tup.ptr());
	}
};


//
// Define the method used to convert a CORBA string
// element  to a python string
//

struct CORBA_StringElt_to_python
{
	static PyObject *convert(_CORBA_String_element const &str_s)
	{
		boost::python::str the_str(str_s.in());

		return incref(the_str.ptr());
	}
};


struct vector_long_to_python
{
	static PyObject *convert(vector<long> const &v_long)
	{
		boost::python::list the_list;
		for (unsigned int i = 0;i < v_long.size();i++)
		{
			the_list.append(v_long[i]);
		}

		return incref(the_list.ptr());
	}
};

struct vector_deviceimpl_to_python
{
	static PyObject *convert(vector<Tango::DeviceImpl *> const &v_di)
	{
		boost::python::list the_list;
		for (unsigned int i = 0;i < v_di.size();i++)
		{
//			the_list.append(v_di[i]);
			the_list.append("aaa");
		}

		return incref(the_list.ptr());
	}
};

//-------------------------------------------------------------------------------------------
//
//	A from_python converter for the DevFailed exception
//
//-------------------------------------------------------------------------------------------

//
// From python converter from a Python DevFailed exception
// to a Tango DevFailed.
// It uses a per thread DevFailed data
// stored in a per thread data structure
// defined in Tango lib
//

struct DevFailed_from_python
{
	DevFailed_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevFailed>());
	}

	static void *convert_from_python_lvalue(PyObject *py_ptr)
	{
//		std::cout << "In the DevFailed from_python lvalue converter" << endl;

//
// Check that the received Python object is convertible
//
		
		if (PyObject_IsInstance(py_ptr,__tango_dev_failed.ptr()) == false)
		{
			return(0);
		}
			
		object obj((handle<>(py_ptr)));
		tuple tup(obj.attr("args"));
		
//
// TAKE CARE, the following static data MUST be a per-thread data
//

		omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
		Tango::DevFailed *df_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_df);
		
		PyDF_2_TgDF(tup.ptr(),df_ptr->errors);			

		return (void *)(df_ptr);
	}
};


//+-------------------------------------------------------------------------
//
// function : 		PyDF_2_TgDF
// 
// description : 	This function unpack data from a Python DevFailed 
//			exception and translate them into a Tango DevFailed
//			exception
//
// argin : - value_ptr : The python exception value object
//	   - del : A reference to the DevErrorList sequence
//
//--------------------------------------------------------------------------

void PyDF_2_TgDF(PyObject *value_ptr,Tango::DevErrorList &del)
{

	int except_level = PyTuple_Size(value_ptr);
	del.length(except_level);

	for (int loop = 0;loop < except_level;loop++)
	{
		PyObject *dict_ptr = PyTuple_GetItem(value_ptr,loop);

		PyObject *desc_key = PyString_FromString("desc");
		PyObject *desc_item = PyDict_GetItem(dict_ptr,desc_key);
		del[loop].desc = CORBA::string_dup(PyString_AsString(desc_item));
		Py_DECREF(desc_key);

		PyObject *reason_key = PyString_FromString("reason");
		PyObject *reason_item = PyDict_GetItem(dict_ptr,reason_key);
		del[loop].reason = CORBA::string_dup(PyString_AsString(reason_item));
		Py_DECREF(reason_key);				

		PyObject *origin_key = PyString_FromString("origin");
		PyObject *origin_item = PyDict_GetItem(dict_ptr,origin_key);
		del[loop].origin = CORBA::string_dup(PyString_AsString(origin_item));
		Py_DECREF(origin_key);	

		PyObject *sev_key = PyString_FromString("severity");
		PyObject *sev_item = PyDict_GetItem(dict_ptr,sev_key);
		char *sev_str = PyString_AsString(sev_item);
		if (::strcmp(sev_str,"WARN") == 0)
			del[loop].severity = Tango::WARN;
		else if (::strcmp(sev_str,"ERROR") == 0)
			del[loop].severity = Tango::ERR;
		else
			del[loop].severity = Tango::PANIC;
		Py_DECREF(sev_key);	
	}
}



//+-------------------------------------------------------------------------
//
// function : 		Py_re_throw_dev_failed
// 
// description : 	This function unpack data in the exception
//			value object which are supposed to be generated by a
//			Tango DevFailed exception generated from Python and
//			throw a Tango::DevFailed from these info.
//			This function also release the Python global lock
//			before the exception is thrown
//
// argin : - exec_ptr : The python exception type pbject
//	   - value_ptr : The ipython exception value object
//	   - tb_ptr ; The python exception traceback object
//	   - lock : The thread Python lock. If NULL, means that the python lock
//		   management must not be done here
//
//--------------------------------------------------------------------------

void Py_re_throw_dev_failed(PyObject *exec_ptr,PyObject *value_ptr,PyObject *tb_ptr,Tango::PyLock *lock)
{
	if (value_ptr == NULL)
	{
		Py_DECREF(exec_ptr);
		Py_XDECREF(tb_ptr);

		if (lock != NULL)
			lock->Release();
				
		Tango::Except::throw_exception((const char *)"PyDs_BadDevFailedException",
					(const char *)"A badly formed exception has been received",
					(const char *)"Py_re_throw_dev_failed");
	}

//
// Translate the Python DevFailed into a Tango DevFailed
//
	
	Tango::DevErrorList dev_err;
	if (PyTuple_Check(value_ptr) == 0)
	{
		PyObject *args = PyObject_GetAttrString(value_ptr,"args");
		if (PyTuple_Check(args) == 0)
		{
			dev_err.length(1);

			dev_err[0].desc = CORBA::string_dup("A badly formed exception has been received");
			dev_err[0].reason = CORBA::string_dup("PyDs_badDevFailedException");
			dev_err[0].origin = CORBA::string_dup("Py_re_throw_exception");
			dev_err[0].severity = Tango::ERR;
		}
		else
		{
			PyDF_2_TgDF(args,dev_err);
			Py_DECREF(args);
		}
	}
	else
		PyDF_2_TgDF(value_ptr,dev_err);
	
	Py_DECREF(exec_ptr);
	Py_DECREF(value_ptr);
	Py_DECREF(tb_ptr);	

	if (lock != NULL)
		lock->Release();
		
	throw Tango::DevFailed(dev_err);
}


//+-------------------------------------------------------------------------
//
// function : 		Py_throw_dev_failed
// 
// description : 	This function throw a DevFailed exception created
//			from a Python exception
//			This function also release the Python global lock
//			before the exception is thrown
//
// argin : - exec_ptr : The python exception type pbject
//	   - value_ptr : The ipython exception value object
//	   - tb_ptr ; The python exception traceback object
//	   - lock : The Python thread lock. If NULL, means that Python lock
//		   management is done elesewhere
//
//--------------------------------------------------------------------------


void Py_throw_dev_failed(PyObject *exec_ptr,PyObject *value_ptr,PyObject *tb_ptr,Tango::PyLock *lock)
{

//
// Send a default exception in case Python does not send us infornation
//

	if (value_ptr == NULL)
	{
		Py_XDECREF(exec_ptr);
		Py_XDECREF(value_ptr);
		Py_XDECREF(tb_ptr);

 		if (lock != NULL)
			lock->Release();
			
		Tango::Except::throw_exception((const char *)"PyDs_BadPythonException",
					(const char *)"A badly formed exception has been received",
					(const char *)"Py_throw_dev_failed");
	}
	
	Tango::DevErrorList dev_err;
	dev_err.length(1);

//
// Populate a one level DevFailed exception
//

	PyObject *tracebackModule = PyImport_ImportModule("traceback");
    	if (tracebackModule != NULL)
    	{
        	PyObject *tbList, *emptyString, *strRetval;

//
// Foormat the traceback part of the Python exception
// and store it in the origin part of the Tango exception
//

        	tbList = PyObject_CallMethod(
            		tracebackModule, 
            		"format_tb", 
            		"O",
            		tb_ptr == NULL ? Py_None : tb_ptr);
			
        	emptyString = PyString_FromString("");
        	strRetval = PyObject_CallMethod(emptyString, "join", "O", tbList);

        	dev_err[0].origin = CORBA::string_dup(PyString_AsString(strRetval));

        	Py_DECREF(tbList);
        	Py_DECREF(emptyString);
        	Py_DECREF(strRetval);

//
// Format the exec and value part of the Python exception
// and store it in the desc part of the Tango exception
//
		
        	tbList = PyObject_CallMethod(
            		tracebackModule, 
            		"format_exception_only", 
            		"OO",
			exec_ptr,
            		value_ptr == NULL ? Py_None : value_ptr);
			
        	emptyString = PyString_FromString("");
        	strRetval = PyObject_CallMethod(emptyString, "join", "O", tbList);

        	dev_err[0].desc = CORBA::string_dup(PyString_AsString(strRetval));

        	Py_DECREF(tbList);
        	Py_DECREF(emptyString);
        	Py_DECREF(strRetval);
        	Py_DECREF(tracebackModule);
		
		dev_err[0].reason = CORBA::string_dup("PyDs_PythonError");
		dev_err[0].severity = Tango::ERR;
    	}
	else
	{
	
//
// Send a default exception because we can't format the
// different parts of the Python's one !
//

        	dev_err[0].origin = CORBA::string_dup("Py_throw_dev_failed");
        	dev_err[0].desc = CORBA::string_dup("Can't import Python traceback module. Can't extract info from Python exception");
		dev_err[0].reason = CORBA::string_dup("PyDs_PythonError");
		dev_err[0].severity = Tango::ERR;
	}
	    
	Py_XDECREF(exec_ptr);
	Py_XDECREF(value_ptr);
	Py_XDECREF(tb_ptr);	
	
	if (lock != NULL)
		lock->Release();
		
	throw Tango::DevFailed(dev_err);			
}	

//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::delete_dev
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method is
//			called to ask Python to delete a device by decrementing
//			the Python ref count
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::delete_dev()
{

//
// Call here the delete_device method. It is defined in Device_3ImplWrap class
// which is already destroyed when the Tango kernel call the delete_device
// method
//

	try
	{
		delete_device();
	}
	catch (Tango::DevFailed &e)
	{
		Tango::Except::print_exception(e);
	}

//
// Decrement Python reference count
//

	try
	{
		Py_DECREF(m_self);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
	}

}


//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::init_instance
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method is
//			called by the Device_3ImplWra ctor.
//			Its rule is to check if the basic Tango device
//			methods are defined.
//
//--------------------------------------------------------------------------


void Device_3ImplWrap::init_instance()
{

//
// Set the PY device flag and inc its reference count
//

	set_py_device(true);
	
	Py_INCREF(m_self);
	
//
// Change it's extension instance in order to get the delete_dev method
//

	Tango::Device_3ImplExt *tmp_ptr = ext_3;
	Py_Device_3ImplExt *new_ext = new Py_Device_3ImplExt(this);
	ext_3 = new_ext;
	delete tmp_ptr;

//
// Check for a always_executed_hook method defined in our class
//
	
	PyObject *meth = PyObject_GetAttrString(m_self,"always_executed_hook");
	if (meth != NULL)
	{
		int mod = PyObject_HasAttrString(meth,"__module__");
		if (mod == 1)
		{
			if (PyMethod_Check(meth) == false)
				always_defined = false;
			else
				always_defined = true;
		}
		else
			always_defined = false;
		Py_DECREF(meth);
	}
	else
	{
		PyErr_Clear();
		always_defined = false;
	}	
	
//
// Check for a read_attr_hardware method defined in our class
//

	meth = PyObject_GetAttrString(m_self,"read_attr_hardware");
	if (meth != NULL)
	{
		int mod = PyObject_HasAttrString(meth,"__module__");
		if (mod == 1)
		{
			if (PyMethod_Check(meth) == false)
				read_attr_defined = false;
			else
				read_attr_defined = true;
		}
		else
			read_attr_defined = false;
		Py_DECREF(meth);
	}
	else
	{
		PyErr_Clear();
		read_attr_defined = false;
	}	

//
// Check for a delete_device method defined in our class
//
	
	meth = PyObject_GetAttrString(m_self,"delete_device");
	if (meth != NULL)
	{
		int mod = PyObject_HasAttrString(meth,"__module__");
		if (mod == 1)
		{
			if (PyMethod_Check(meth) == false)
				delete_dev_defined = false;
			else
				delete_dev_defined = true;
		}
		else
			delete_dev_defined = false;
		Py_DECREF(meth);
	}
	else
	{
		PyErr_Clear();
		delete_dev_defined = false;
	}	
	
//
// Check for a signal_handler method defined in our class
//

	meth = PyObject_GetAttrString(m_self,"signal_handler");
	if (meth != NULL)
	{
		int mod = PyObject_HasAttrString(meth,"__module__");
		if (mod == 1)
		{	
			if (PyMethod_Check(meth) == false)
				sig_handler_defined = false;
			else
				sig_handler_defined = true;
		}
		else
			sig_handler_defined = false;
		Py_DECREF(meth);
	}
	else
	{
		PyErr_Clear();
		sig_handler_defined = false;
	}
		
//
// Check for a State method defined in our class
//

	int State_met = PyObject_HasAttrString(m_self,"State");
	if (State_met == 1)
	{
		meth = PyObject_GetAttrString(m_self,"State");
		if (meth != NULL)
		{
			int mod = PyObject_HasAttrString(meth,"__module__");
			if (mod == 1)
			{	
				if (PyMethod_Check(meth) == false)
					State_defined = false;
				else
					State_defined = true;
			}
			else
				State_defined = false;
			Py_DECREF(meth);
		}
		else
		{
			PyErr_Clear();
			State_defined = false;
		}
	}
	else
	{
		PyErr_Clear();
		State_defined = false;
	}
		
//
// Check for a Status method defined in our class
//

	int Status_met = PyObject_HasAttrString(m_self,"Status");
	if (Status_met == 1)
	{
		meth = PyObject_GetAttrString(m_self,"Status");
		if (meth != NULL)
		{
			int mod = PyObject_HasAttrString(meth,"__module__");
			if (mod == 1)
			{	
				if (PyMethod_Check(meth) == false)
					Status_defined = false;
				else
					Status_defined = true;
			}
			else
				Status_defined = false;
			Py_DECREF(meth);
		}
		else
		{
			PyErr_Clear();
			Status_defined = false;
		}
	}
	else
	{
		PyErr_Clear();
		Status_defined = false;
	}
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::Py_tg_call_void_void
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the always_executed_hook to the Python method.
//
//			I have not used the classical "get_override" boost
//			stuff because I don't want to have the class
//			inheriting from wrapper<> which completely hides
//			the PyObject ptr (and I need it). Using boost
//			"has_back_reference" stuff does not help because
//			using both has_back_reference and wrapper<> in a
//			class will make the pointer in the wrapper<> class
//			not to be initialized.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::PyTg_call_void_void(const char *meth_name)
{
	
	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();
	
	try
	{
		call_method<void>(m_self,meth_name);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}

//+-------------------------------------------------------------------------
//
// function : 		Device_3ImpleWrap::Py_tg_call_read_hard
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the read_attr_hardware to the Python method.
//
//			I have not used the classical "get_override" boost
//			stuff because I don't want to have the class
//			inheriting from wrapper<> which completely hides
//			the PyObject ptr (and I need it). Using boost
//			"has_back_reference" stuff does not help because
//			using both has_back_reference and wrapper<> in a
//			class will make the pointer in the wrapper<> class
//			not to be initialized.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::PyTg_call_read_hard(vector<long> &attr_list)
{		

//
// In case of memorized attribute, this method could be called by the main thread
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		call_method<void>(m_self,"read_attr_hardware",attr_list);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::PyTg_call_delete_device
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the delete_device to the Python method.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::PyTg_call_delete_device()
{
	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();
	
	try
	{
		call_method<void>(m_self,"delete_device");
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}
	
	lock_ptr->Release();	
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::init_device
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the init_device to the Python method.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::init_device()
{
	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();
	
	try
	{
		call_method<void>(m_self,"init_device");
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}
	
	lock_ptr->Release();	
}



//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::PyTg_call_sig_handler
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the signal_handler to the Python method.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::PyTg_call_sig_handler(long signo)
{		

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		call_method<void>(m_self,"signal_handler",signo);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::PyTg_call_State
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the State to the Python method.
//			This method also manages special cases to forbid
//			recusive call which could happens when the State
//			method is re-defined in user code
//
//--------------------------------------------------------------------------

Tango::DevState Device_3ImplWrap::PyTg_call_State()
{		

//
// In case of memorized attribute, this method could be called by the main thread
//

	bool *state_flag_ptr;
	
	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	state_flag_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->rec_state);
	lock_ptr->Get();

	Tango::DevState sta = Tango::UNKNOWN;

	try
	{
		if (*state_flag_ptr == true)
			sta = Tango::DeviceImpl::dev_state();
		else
		{
			*state_flag_ptr = true;
			sta = call_method<Tango::DevState>(m_self,"State");
		}
	}
	catch(error_already_set)
	{
		*state_flag_ptr = false;
			
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}
	catch(Tango::DevFailed &e)
	{
		*state_flag_ptr = false;
	}

	lock_ptr->Release();
	*state_flag_ptr = false;
	
	return sta;
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::PyTg_call_Status
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the Status to the Python method.
//			This method also manages special cases to forbid
//			recusive call which could happens when the Status
//			method is re-defined in user code
//
//--------------------------------------------------------------------------

Tango::ConstDevString Device_3ImplWrap::PyTg_call_Status()
{		

//
// In case of memorized attribute, this method could be called by the main thread
//

	bool *status_flag_ptr;

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	status_flag_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->rec_status);
	lock_ptr->Get();

	Tango::ConstDevString stat = NULL;
	try
	{
		if (*status_flag_ptr == true)
			stat = Tango::DeviceImpl::dev_status();
		else
		{
			*status_flag_ptr = true;
			stat = call_method<Tango::ConstDevString>(m_self,"Status");
		}
	}
	catch(error_already_set)
	{
		*status_flag_ptr = false;
			
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}
	catch(Tango::DevFailed &e)
	{
		*status_flag_ptr = false;
	}

	lock_ptr->Release();
	*status_flag_ptr = false;
			
	return stat;
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::met_exist_callable
// 
// description : 	Check that a method exist and is callable
//
// argin : - met_name : The method name
//	   - attr_name : The attribute name
//	   - tg_ptr : Ptr to the PyObject for the Tango object class
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::met_exist_callable(string &met_name,string &attr_name,PyObject *tg_ptr)
{
		
	PyObject *meth = PyObject_GetAttrString(tg_ptr,const_cast<char *>(met_name.c_str()));
	if (meth == NULL)
	{
		PyErr_Clear();
		TangoSys_OMemStream o;
		o << "Wrong definition of attribute " << attr_name;
		o << "\nThe attribute method " << met_name << " does not exist in your class!" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       			o.str(),
			       			(const char *)"Device_3ImplWrap::met_exist_callable()");
	}
	else
	{
		if (PyMethod_Check(meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of attribute " << attr_name;
			o << "\nThe object " << met_name << " exist in your class but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"Device_3ImplWrap::not_exist_callable()");
		}
		else
			Py_DECREF(meth);
	}
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3ImpleWrap::add_attribute
// 
// description : 	Device_3ImpleWrap is the class used to represent
//			a Python Tango device. This method forward the call
//			to the Status to the Python method.
//
//--------------------------------------------------------------------------

void Device_3ImplWrap::cpp_add_attribute(Tango::Attr *new_attr)
{
	string attr_name = new_attr->get_name();
	
//
// Before creating the attribute, check that the method to read and/or to write
// the attributes exist and that they are callable
//

	string read_name_met("read_");
	read_name_met += attr_name;

	string write_name_met("write_");
	write_name_met += attr_name;
		
	switch (new_attr->get_writable())
	{
	case Tango::READ:
		met_exist_callable(read_name_met,attr_name,m_self);
		break;
		
	case Tango::WRITE:
		met_exist_callable(write_name_met,attr_name,m_self);
		break;
		
	case Tango::READ_WRITE:
		met_exist_callable(read_name_met,attr_name,m_self);
		met_exist_callable(write_name_met,attr_name,m_self);
		break;
		
	default:
	
//
// Do we support READ_WITH_WRITE ?
//

		break;
	}

//
// Create the attribute objet according to attribute format
//

	PyScaAttr *sca_attr_ptr = NULL;
	PySpecAttr *spec_attr_ptr = NULL;
	PyImaAttr *ima_attr_ptr= NULL;

	Tango::AttrWriteType attr_write = new_attr->get_writable();
	long attr_type = new_attr->get_type();
	long x,y;
	vector<Tango::AttrProperty> &def_prop = new_attr->get_user_default_properties();

	switch (new_attr->get_format())
	{
	case Tango::SCALAR:
		sca_attr_ptr = new PyScaAttr(attr_name.c_str(),attr_type,attr_write,def_prop);
		sca_attr_ptr->set_read_name(read_name_met);
		sca_attr_ptr->set_write_name(write_name_met);
		break;
		
	case Tango::SPECTRUM:
		x = (static_cast<Tango::SpectrumAttr *>(new_attr))->get_max_x();
		spec_attr_ptr = new PySpecAttr(attr_name.c_str(),attr_type,attr_write,x,def_prop);
		spec_attr_ptr->set_read_name(read_name_met);
		spec_attr_ptr->set_write_name(write_name_met);
		break;
		
	case Tango::IMAGE:
		x = (static_cast<Tango::SpectrumAttr *>(new_attr))->get_max_x();
		y = (static_cast<Tango::ImageAttr *>(new_attr))->get_max_y();
		ima_attr_ptr = new PyImaAttr(attr_name.c_str(),attr_type,attr_write,x,y,def_prop);
		ima_attr_ptr->set_read_name(read_name_met);
		ima_attr_ptr->set_write_name(write_name_met);
		break;
	}

//
// Check if there is a is_allowed method for this attribute
// 

	string is_allo("is_");
	is_allo += attr_name;
	is_allo += "_allowed";

	PyObject *allo_meth = PyObject_GetAttrString(m_self,const_cast<char *>(is_allo.c_str()));
	if (allo_meth == NULL)
	{
		PyErr_Clear();
	}
	else
	{
		if (PyMethod_Check(allo_meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of attribute " << attr_name;
			o << "\nThe object " << is_allo << " exist in your class but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"Device_3ImplWrap::cpp_add_attribute()");
		}
		else
		{
			switch (new_attr->get_format())
			{
			case Tango::SCALAR:
				sca_attr_ptr ->set_allowed(is_allo);
				break;

			case Tango::SPECTRUM:
				spec_attr_ptr->set_allowed(is_allo);
				break;

			case Tango::IMAGE:
				spec_attr_ptr->set_allowed(is_allo);
				break;
			}
			Py_DECREF(allo_meth);
		}
	}

	
//
// Install attribute in Tango. 
//

	switch (new_attr->get_format())
	{
	case Tango::SCALAR:
		Tango::DeviceImpl::add_attribute(sca_attr_ptr);
		break;
		
	case Tango::SPECTRUM:
		Tango::DeviceImpl::add_attribute(spec_attr_ptr);
		break;
		
	case Tango::IMAGE:
		Tango::DeviceImpl::add_attribute(ima_attr_ptr);
		break;
	}
		
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClassWrap::init_class
// 
// description : 	CppDeviceClassWrap is the class used to represent
//			a Python Tango class. This method is called from the
//			CppDeviceClassWrap ctor
//
//--------------------------------------------------------------------------

void CppDeviceClassWrap::init_class()
{
	set_py_class(true);
	
//
// Check for a signal_handler method defined in our class
//

	PyObject *meth = PyObject_GetAttrString(m_self,"signal_handler");
	if (meth != NULL)
	{
		int mod = PyObject_HasAttrString(meth,"__module__");
		if (mod == 1)
		{
			if (PyMethod_Check(meth) == false)
				class_sig_handler_defined = false;
			else
				class_sig_handler_defined = true;
		}
		else
			class_sig_handler_defined = false;
		Py_DECREF(meth);
	}
	else
	{
		PyErr_Clear();
		class_sig_handler_defined = false;
	}	
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClassWrap::PyTg_call_dev_factory
// 
// description : 	CppDeviceClassWrap is the class used to represent
//			a Python Tango class. This method forward a C++
//			call to the dev_factory method to the Python
//			method
//
//--------------------------------------------------------------------------


void CppDeviceClassWrap::PyTg_call_dev_factory(const Tango::DevVarStringArray *ptr)
{
	try
	{
		call_method<void>(m_self,"device_factory",ptr);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
	}

}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClassWrap::PyTg_call_class_sig_handler
// 
// description : 	CppDeviceClassWrap is the class used to represent
//			a Python Tango class. This method forward a C++
//			call to the sig_handler method to the Python
//			method
//
//--------------------------------------------------------------------------

void CppDeviceClassWrap::PyTg_call_class_sig_handler(long signo)
{		
//
// In case of memorized attribute, this method could be called by the main thread
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();

	try
	{
		call_method<void>(m_self,"signal_handler",signo);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,lock_ptr);
		}
	}

	lock_ptr->Release();
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClassWrap::delete_class
// 
// description : 	CppDeviceClassWrap is the class used to represent
//			a Python Tango class. This method is
//			called to ask Python to delete a class by decrementing
//			the Python ref count
//
//--------------------------------------------------------------------------

void CppDeviceClassWrap::delete_class()
{
	try
	{
//
// Get the Python PyTango module
//

		handle<> mod(borrowed(PyImport_AddModule("PyTango")));
		object mod_obj(mod);

//
// Call the delete_class_list function in order to clear the global
// constructed class Python list. It is MANDATORY to destroy these objects
// from Python. Otherwise, there are "seg fault" when Python exit.
// It tooks me quite a long time to find this...
//

		PyObject *del_cl_list = PyObject_GetAttrString(mod_obj.ptr(),"delete_class_list");
		call<void>(del_cl_list);
		Py_DECREF(del_cl_list);
	}
	catch(error_already_set)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		if (PyErr_ExceptionMatches(__tango_dev_failed.ptr()))
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_re_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
		else
		{
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			Py_throw_dev_failed(ex_exec,ex_value,ex_tb,NULL);
		}
	}
}



//+-------------------------------------------------------------------------
//
// methods : 		RealPyLock::ctor, dtor, Get and Release
// 
// description : 	Manage the Python lock.
//			this method has to be re-entrant in case of a command
//			or an attribute calling another command or attribute
//			of another class in the same process. Unfortunately,
//			this is not managed by Python
//
//--------------------------------------------------------------------------


RealPyLock::RealPyLock():tstate(0),locking_ctr(0)
{
	th_id = omni_thread::self()->id();
	Tango::Util *tg = Tango::Util::instance();
	py_dbg = tg->is_py_dbg();
	poll_th_id = tg->get_polling_thread_id();

	PyInterpreterState *py_ptr = (PyInterpreterState *)tg->get_py_interp();
	if (py_dbg == true)
	{
		tstate = PyThreadState_Get();
	}
	else
	{
		PyEval_AcquireLock();
		if (th_id == 0)
			tstate = PyThreadState_Get();
		else
			tstate = PyThreadState_New(py_ptr);
		PyEval_ReleaseLock();
	}
}
	

RealPyLock::~RealPyLock()
{
	if (py_dbg == true)
		return;
		
	if (th_id != 0)
	{
		PyEval_AcquireLock();
		PyThreadState_Swap(NULL);
		PyThreadState_Clear(tstate);
		PyThreadState_Delete(tstate);
		PyEval_ReleaseLock();
	}
}

		
void RealPyLock::Get()
{
	if (py_dbg == true)
		return;
			
	if (locking_ctr == 0)
	{	
		PyEval_AcquireLock();
		PyThreadState_Swap(tstate);
	}
	locking_ctr++;
}


void RealPyLock::Release()
{
	if (py_dbg == true)
		return;
		
	if (locking_ctr == 0)
		return;

	locking_ctr--;
	if (locking_ctr == 0)
	{		
		PyThreadState_Swap(NULL);
		PyEval_ReleaseLock();
	}
}

//+-------------------------------------------------------------------------
//
// method : 		CreateRealPyLock::create
// 
// description : 	Create a real Python lock management class and
//			manage debugging info if the process has been
//			started under debugger control
//
//--------------------------------------------------------------------------

Tango::PyLock *CreateRealPyLock::create()
{
	Tango::PyLock *p_lock = new RealPyLock();

	int th_id = static_cast<RealPyLock *>(p_lock)->get_th_id();
	if (th_id != 0)
	{
		if (static_cast<RealPyLock *>(p_lock)->get_py_dbg() == true)
		{
			p_lock->Get();
			object mod_obj((handle<>(borrowed(PyImport_AddModule("PyTango")))));
			PyObject *dbg_func = PyObject_GetAttrString(mod_obj.ptr(),"set_dbg_fct");

			if (dbg_func != NULL)
			{
				call<void>(dbg_func);
				Py_DECREF(dbg_func);

			}
			else
				PyErr_Clear();
			p_lock->Release();
		}
	}
	
	return p_lock;
}




//+-------------------------------------------------------------------------
//
// function : 		init_module
// 
// description : Initialize the Python module. This means
//		  - Installing to python converters
//		  - Installing also from python converters
//		  - Installing DevFailed exception converter
//
//--------------------------------------------------------------------------
	
void init_module()
{

//
// Register in Boost the to python converters
//

	
	to_python_converter<Tango::DevVarCharArray,CORBASequence_to_python<Tango::DevVarCharArray> >();
	to_python_converter<Tango::DevVarShortArray,CORBASequence_to_python<Tango::DevVarShortArray> >();
	to_python_converter<Tango::DevVarLongArray,CORBASequence_to_python<Tango::DevVarLongArray> >();
	to_python_converter<Tango::DevVarFloatArray,CORBASequence_to_python<Tango::DevVarFloatArray> >();
	to_python_converter<Tango::DevVarDoubleArray,CORBASequence_to_python<Tango::DevVarDoubleArray> >();
	to_python_converter<Tango::DevVarUShortArray,CORBASequence_to_python<Tango::DevVarUShortArray> >();
	to_python_converter<Tango::DevVarULongArray,CORBASequence_to_python<Tango::DevVarULongArray> >();
	to_python_converter<Tango::DevVarStringArray,CORBASequence_to_python<Tango::DevVarStringArray> >();
	to_python_converter<Tango::DevVarLongStringArray,CORBA_LongString_to_python>();
	to_python_converter<Tango::DevVarDoubleStringArray,CORBA_DoubleString_to_python>();
	to_python_converter<Tango::DevVarLong64Array,CORBASequence_to_python<Tango::DevVarLong64Array> >();
	to_python_converter<Tango::DevVarULong64Array,CORBASequence_to_python<Tango::DevVarULong64Array> >();

	to_python_converter<_CORBA_String_element,CORBA_StringElt_to_python>();
	to_python_converter<vector<long>,vector_long_to_python>();
	to_python_converter<vector<Tango::DeviceImpl *>,vector_deviceimpl_to_python>();
	
//
// Register in Boost the from python converters
//

	DevVarCharArray_from_python();
	DevVarShortArray_from_python();
	DevVarLongArray_from_python();
	DevVarFloatArray_from_python();
	DevVarDoubleArray_from_python();
	DevVarUShortArray_from_python();
	DevVarULongArray_from_python();
	DevVarStringArray_from_python();
	DevVarLongStringArray_from_python();
	DevVarDoubleStringArray_from_python();
	DevVarLong64Array_from_python();
	DevVarULong64Array_from_python();
	
	std_string_from_python();
	std_vec_db_from_python();
	std_vec_string_from_python();
	DevErrorList_from_python();
	const_char_ptr_from_python();
	Ch_ptr_from_python();
	DevFailed_from_python();
	
}


//---------------------------------------------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------------------------------------------



void python_init()
{
	Tango::Util *tg = Tango::Util::instance();

//
// Init Python threading and save Interpreter
// If the python threading is already initialised, I assume
// that I am running in a debugger.
// Well debugger could be better written...
// Eclipse/Pydev debugger is in this aspect not perfect....
//

	PyThreadState *tstate;
	if (PyEval_ThreadsInitialized() != 0)
	{
		tstate = PyThreadState_Get();
		tg->set_py_interp((void *)tstate->interp);
		
//
// Check if we are really in a debugger (at least Pydev)
// Some Python module (ctypes for instance) also initialize 
// the Python threading
//

		object mod_obj((handle<>(borrowed(PyImport_AddModule("PyTango")))));
		PyObject *dbg_trace_func = PyObject_GetAttrString(mod_obj.ptr(),"pydev_trace_func");

		if (dbg_trace_func != NULL)
		{
			if (dbg_trace_func != Py_None)
			{
				Py_DECREF(dbg_trace_func);
				tg->set_py_dbg();
			}
			else
			{
				Py_DECREF(dbg_trace_func);
				PyEval_ReleaseLock();
			}
		}
		else
			PyErr_Clear();				
	}
	else
	{
		PyEval_InitThreads();
		tstate = PyThreadState_Get();
		tg->set_py_interp((void *)tstate->interp);
		PyEval_ReleaseLock();
	}

//
// We are in a python DS
//

	tg->set_py_ds();

//
// Create a real Python lock creator and replace the empty one
// in the Util class by this new one
//

	CreateRealPyLock *cr_ptr = new CreateRealPyLock();
	Tango::CreatePyLock *lib_cr = tg->get_py_lock_creator();
	tg->set_py_lock_creator(cr_ptr);
	delete lib_cr;

//
// Create Python data for this thread (main thread (0))
// Also get the python lock to create device class
//

	omni_thread *th = omni_thread::self();
	th->set_value(key_py_data,new Tango::PyData());

		
	omni_thread::value_t *tmp_py_data = th->get_value(key_py_data);
	Tango::PyLock *lock_ptr = (static_cast<Tango::PyData *>(tmp_py_data))->PerTh_py_lock;
	lock_ptr->Get();
}


//---------------------------------------------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------------------------------------------



void (Tango::DeviceClass::*set_ty)(const char *)			= &Tango::DeviceClass::set_type;
void (Tango::DeviceClass::*d_destroy)(const char *)			= &Tango::DeviceClass::device_destroyer;

void (*set_att_1)(Tango::Attribute *,boost::python::list) 		= &set_attribute_value;
void (*set_att_2)(Tango::Attribute *,Tango::DevLong64) 				= &set_attribute_value;
void (*set_att_3)(Tango::Attribute *,double) 				= &set_attribute_value;
void (*set_att_4)(Tango::Attribute *,boost::python::str)		= &set_attribute_value;
void (*set_att_5)(Tango::Attribute *,boost::python::list,long)		= &set_attribute_value;
void (*set_att_6)(Tango::Attribute *,boost::python::list,long,long)	= &set_attribute_value;
void (*set_att_7)(Tango::Attribute *,Tango::DevLong64,long)				= &set_attribute_value;
void (*set_att_8)(Tango::Attribute *,Tango::DevLong64,long,long)			= &set_attribute_value;
void (*set_att_9)(Tango::Attribute *,boost::python::str,long)		= &set_attribute_value;
void (*set_att_10)(Tango::Attribute *,boost::python::str,long,long)	= &set_attribute_value;
void (*set_att_11)(Tango::Attribute *,double,long) 			= &set_attribute_value;
void (*set_att_12)(Tango::Attribute *,double,long,long) 		= &set_attribute_value;

void (*set_att_dq_2)(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality) 			= &set_attribute_value_dq;
void (*set_att_dq_3)(Tango::Attribute *,double,double,Tango::AttrQuality) 			= &set_attribute_value_dq;
void (*set_att_dq_4)(Tango::Attribute *,str,double,Tango::AttrQuality) 				= &set_attribute_value_dq;
void (*set_att_dq_1)(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality) 		= &set_attribute_value_dq;
void (*set_att_dq_5)(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality,long) 	= &set_attribute_value_dq;
void (*set_att_dq_6)(Tango::Attribute *,boost::python::list,double,Tango::AttrQuality,long,long)= &set_attribute_value_dq;
void (*set_att_dq_7)(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality,long)			= &set_attribute_value_dq;
void (*set_att_dq_8)(Tango::Attribute *,Tango::DevLong64,double,Tango::AttrQuality,long,long)		= &set_attribute_value_dq;
void (*set_att_dq_9)(Tango::Attribute *,boost::python::str,double,Tango::AttrQuality,long)	= &set_attribute_value_dq;
void (*set_att_dq_10)(Tango::Attribute *,boost::python::str,double,Tango::AttrQuality,long,long)= &set_attribute_value_dq;
void (*set_att_dq_11)(Tango::Attribute *,double,double,Tango::AttrQuality,long) 		= &set_attribute_value_dq;
void (*set_att_dq_12)(Tango::Attribute *,double,double,Tango::AttrQuality,long,long) 		= &set_attribute_value_dq;

void (*WAtt_set_val_1)(Tango::WAttribute *,Tango::DevLong64)                = set_write_value;
void (*WAtt_set_val_2)(Tango::WAttribute *,double)                          = set_write_value;
void (*WAtt_set_val_3)(Tango::WAttribute *,str)                             = set_write_value;
void (*WAtt_set_val_4)(Tango::WAttribute *,boost::python::list)             = set_write_value;
void (*WAtt_set_val_5)(Tango::WAttribute *,boost::python::list, long)       = set_write_value;
void (*WAtt_set_val_6)(Tango::WAttribute *,boost::python::list, long, long) = set_write_value;

void (*WAtt_get_min_val_1)(Tango::WAttribute *,Tango::DevLong64 &)	= get_min_value;
void (*WAtt_get_min_val_2)(Tango::WAttribute *,double &)			= get_min_value;
void (*WAtt_set_min_val_1)(Tango::WAttribute *,Tango::DevLong64)	= set_min_value;
void (*WAtt_set_min_val_2)(Tango::WAttribute *,double)				= set_min_value;
bool (*WAtt_is_min_val)(Tango::WAttribute *)						= is_min_value;

void (*WAtt_get_max_val_1)(Tango::WAttribute *,Tango::DevLong64 &)	= get_max_value;
void (*WAtt_get_max_val_2)(Tango::WAttribute *,double &)			= get_max_value;
void (*WAtt_set_max_val_1)(Tango::WAttribute *,Tango::DevLong64)	= set_max_value;
void (*WAtt_set_max_val_2)(Tango::WAttribute *,double)				= set_max_value;
bool (*WAtt_is_max_val)(Tango::WAttribute *)						= is_max_value;

void (*push_ev_2)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64)			= &push_att_event;
void (*push_ev_7)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,long)		= &push_att_event;
void (*push_ev_8)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,long,long)	= &push_att_event;
void (*push_ev_3)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double)				= &push_att_event;
void (*push_ev_11)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,long)			= &push_att_event;
void (*push_ev_12)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,long,long)			= &push_att_event;
void (*push_ev_4)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str)			= &push_att_event;
void (*push_ev_9)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,long)		= &push_att_event;
void (*push_ev_10)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,long,long)	= &push_att_event;
void (*push_ev_1)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list)			= &push_att_event;
void (*push_ev_5)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,long)		= &push_att_event;
void (*push_ev_6)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,long,long)	= &push_att_event;
void (*push_ev_ex)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevFailed *)			= &push_att_event;
void (*push_ev_st)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &)					= &push_att_event;

void (*push_ev_dq_2)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality)				= &push_att_event;
void (*push_ev_dq_7)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality,long)			= &push_att_event;
void (*push_ev_dq_8)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,Tango::DevLong64,double,Tango::AttrQuality,long,long)			= &push_att_event;
void (*push_ev_dq_3)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality)				= &push_att_event;
void (*push_ev_dq_11)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality,long)			= &push_att_event;
void (*push_ev_dq_12)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,double,double,Tango::AttrQuality,long,long)		= &push_att_event;
void (*push_ev_dq_4)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality)		= &push_att_event;
void (*push_ev_dq_9)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality,long)	= &push_att_event;
void (*push_ev_dq_10)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::str,double,Tango::AttrQuality,long,long)	= &push_att_event;
void (*push_ev_dq_1)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality)		= &push_att_event;
void (*push_ev_dq_5)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality,long)	= &push_att_event;
void (*push_ev_dq_6)(Tango::DeviceImpl *,boost::python::str,vector<string> &,vector<double> &,boost::python::list,double,Tango::AttrQuality,long,long)	= &push_att_event;

void (*push_ch_ev_2)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64)				= &push_att_ch_event;
void (*push_ch_ev_7)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long)				= &push_att_ch_event;
void (*push_ch_ev_8)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long,long)			= &push_att_ch_event;
void (*push_ch_ev_3)(Tango::DeviceImpl *,boost::python::str,double)				= &push_att_ch_event;
void (*push_ch_ev_11)(Tango::DeviceImpl *,boost::python::str,double,long)			= &push_att_ch_event;
void (*push_ch_ev_12)(Tango::DeviceImpl *,boost::python::str,double,long,long)			= &push_att_ch_event;
void (*push_ch_ev_4)(Tango::DeviceImpl *,boost::python::str,boost::python::str)			= &push_att_ch_event;
void (*push_ch_ev_9)(Tango::DeviceImpl *,boost::python::str,boost::python::str,long)		= &push_att_ch_event;
void (*push_ch_ev_10)(Tango::DeviceImpl *,boost::python::str,boost::python::str,long,long)	= &push_att_ch_event;
void (*push_ch_ev_1)(Tango::DeviceImpl *,boost::python::str,boost::python::list)		= &push_att_ch_event;
void (*push_ch_ev_5)(Tango::DeviceImpl *,boost::python::str,boost::python::list,long)		= &push_att_ch_event;
void (*push_ch_ev_6)(Tango::DeviceImpl *,boost::python::str,boost::python::list,long,long)	= &push_att_ch_event;
void (*push_ch_ev_ex)(Tango::DeviceImpl *,boost::python::str,Tango::DevFailed *)		= &push_att_ch_event;
void (*push_ch_ev_st)(Tango::DeviceImpl *,boost::python::str)					= &push_att_ch_event;

void (*push_ch_ev_dq_2)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality)				= &push_att_ch_event;
void (*push_ch_ev_dq_7)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long)			= &push_att_ch_event;
void (*push_ch_ev_dq_8)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long,long) 		= &push_att_ch_event;
void (*push_ch_ev_dq_3)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality) 			= &push_att_ch_event;
void (*push_ch_ev_dq_11)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long)  		= &push_att_ch_event;
void (*push_ch_ev_dq_12)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long,long)		= &push_att_ch_event;
void (*push_ch_ev_dq_4)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality)		= &push_att_ch_event;
void (*push_ch_ev_dq_9)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long)	= &push_att_ch_event;
void (*push_ch_ev_dq_10)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long,long)	= &push_att_ch_event;
void (*push_ch_ev_dq_1)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality)		= &push_att_ch_event;
void (*push_ch_ev_dq_5)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long)	= &push_att_ch_event;
void (*push_ch_ev_dq_6)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long,long)	= &push_att_ch_event;


void (*push_ar_ev_2)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64)				= &push_att_ar_event;
void (*push_ar_ev_7)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long)				= &push_att_ar_event;
void (*push_ar_ev_8)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,long,long)			= &push_att_ar_event;
void (*push_ar_ev_3)(Tango::DeviceImpl *,boost::python::str,double)				= &push_att_ar_event;
void (*push_ar_ev_11)(Tango::DeviceImpl *,boost::python::str,double,long)			= &push_att_ar_event;
void (*push_ar_ev_12)(Tango::DeviceImpl *,boost::python::str,double,long,long)			= &push_att_ar_event;
void (*push_ar_ev_4)(Tango::DeviceImpl *,boost::python::str,boost::python::str)			= &push_att_ar_event;
void (*push_ar_ev_9)(Tango::DeviceImpl *,boost::python::str,boost::python::str,long)		= &push_att_ar_event;
void (*push_ar_ev_10)(Tango::DeviceImpl *,boost::python::str,boost::python::str,long,long)	= &push_att_ar_event;
void (*push_ar_ev_1)(Tango::DeviceImpl *,boost::python::str,boost::python::list)		= &push_att_ar_event;
void (*push_ar_ev_5)(Tango::DeviceImpl *,boost::python::str,boost::python::list,long)		= &push_att_ar_event;
void (*push_ar_ev_6)(Tango::DeviceImpl *,boost::python::str,boost::python::list,long,long)	= &push_att_ar_event;
void (*push_ar_ev_ex)(Tango::DeviceImpl *,boost::python::str,Tango::DevFailed *)		= &push_att_ar_event;
void (*push_ar_ev_st)(Tango::DeviceImpl *,boost::python::str)					= &push_att_ar_event;

void (*push_ar_ev_dq_2)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality)				= &push_att_ar_event;
void (*push_ar_ev_dq_7)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long)			= &push_att_ar_event;
void (*push_ar_ev_dq_8)(Tango::DeviceImpl *,boost::python::str,Tango::DevLong64,double,Tango::AttrQuality,long,long) 	= &push_att_ar_event;
void (*push_ar_ev_dq_3)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality) 			= &push_att_ar_event;
void (*push_ar_ev_dq_11)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long)  		= &push_att_ar_event;
void (*push_ar_ev_dq_12)(Tango::DeviceImpl *,boost::python::str,double,double,Tango::AttrQuality,long,long)		= &push_att_ar_event;
void (*push_ar_ev_dq_4)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality)		= &push_att_ar_event;
void (*push_ar_ev_dq_9)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long)	= &push_att_ar_event;
void (*push_ar_ev_dq_10)(Tango::DeviceImpl *,boost::python::str,boost::python::str,double,Tango::AttrQuality,long,long)	= &push_att_ar_event;
void (*push_ar_ev_dq_1)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality)		= &push_att_ar_event;
void (*push_ar_ev_dq_5)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long)	= &push_att_ar_event;
void (*push_ar_ev_dq_6)(Tango::DeviceImpl *,boost::python::str,boost::python::list,double,Tango::AttrQuality,long,long)	= &push_att_ar_event;


Tango::Util *util_instance1() {return Tango::Util::instance();}
Tango::Util *util_instance2(bool b) {return Tango::Util::instance(b);}
#ifdef WIN32
Tango::Util *util_init_win32(int arg1,char *arg2[]) {return Tango::Util::init(arg1,arg2);}
#endif

void Py_Except(const char *a,const char *b,const char *c) {Tango::Except::throw_exception(a,b,c);}
void Py_Except_sever(const char *a,const char *b,const char *c,Tango::ErrSeverity d) {Tango::Except::throw_exception(a,b,c,d);}
void Py_Re_Except(Tango::DevFailed df,const char *a,const char *b,const char *c) {Tango::Except::re_throw_exception(df,a,b,c);}
void Py_Re_Except_sever(Tango::DevFailed df,const char *a,const char *b,const char *c,Tango::ErrSeverity d) {Tango::Except::re_throw_exception(df,a,b,c,d);}
void Print_Except(Tango::DevFailed df) {Tango::Except::print_exception(df);}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Util_server_init_overload,Tango::Util::server_init,0,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Att_set_quality_over,Tango::Attribute::set_quality,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (DevClass_ed_over,CppDeviceClass::export_device,1,2)
#ifndef sun
#ifndef WIN32
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Dev_rs_over,Tango::DeviceImpl::register_signal,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Class_rs_over,Tango::DeviceClass::register_signal,1,2)
#endif
#endif
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (Class_urs_over,Tango::DeviceClass::unregister_signal,1,1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (set_ch_event_over,Tango::DeviceImpl::set_change_event,2,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (set_ar_event_over,Tango::DeviceImpl::set_archive_event,2,3)


void (Tango::DeviceClass::*Cl_dev_1)(string &,string &)			= &Tango::DeviceClass::add_wiz_dev_prop;
void (Tango::DeviceClass::*Cl_dev_2)(string &,string &,string &)	= &Tango::DeviceClass::add_wiz_dev_prop;
void (Tango::DeviceClass::*Cl_cla_1)(string &,string &)			= &Tango::DeviceClass::add_wiz_class_prop;
void (Tango::DeviceClass::*Cl_cla_2)(string &,string &,string &)	= &Tango::DeviceClass::add_wiz_class_prop;


void declare_tangogroup_base();

void py_ds()
{			
     
	enum_<Tango::SerialModel>("SerialModel")
	.value("BY_DEVICE",Tango::BY_DEVICE)
	.value("BY_CLASS",Tango::BY_CLASS)
	.value("BY_PROCESS",Tango::BY_PROCESS)
	.value("NO_SYNC",Tango::NO_SYNC)
	;


    	enum_<Tango::AttReqType>("AttReqType")
     	.value("READ_REQ",Tango::READ_REQ)
     	.value("WRITE_REQ",Tango::WRITE_REQ)
     	;
	
	enum_<Tango::ErrSeverity>("ErrSeverity")
	.value("WARN",Tango::WARN)
   	.value("ERR",Tango::ERR)
   	.value("PANIC",Tango::PANIC)
	; 
	
        class_<Tango::Except,boost::noncopyable>("Except",no_init)
	.def("throw_exception",Py_Except)
	.def("throw_exception",Py_Except_sever)
	.staticmethod("throw_exception")
	.def("re_throw_exception",Py_Re_Except)
	.def("re_throw_exception",Py_Re_Except_sever)
	.staticmethod("re_throw_exception")
	.def("print_exception",Print_Except)
	.staticmethod("print_exception")
	;
	
        class_<Tango::Util,boost::noncopyable>("Util",no_init)
#ifdef WIN32
	.def("init",util_init_win32,return_value_policy<reference_existing_object>())
#else
	.def("init",&Tango::Util::init,return_value_policy<reference_existing_object>())
#endif
	.staticmethod("init")
	.def("instance",util_instance1, return_value_policy<reference_existing_object>())
	.def("instance",util_instance2, return_value_policy<reference_existing_object>())
	.staticmethod("instance")
	.def("server_init",&Tango::Util::server_init,Util_server_init_overload())
	.def("set_serial_model",&Tango::Util::set_serial_model)
	.def("server_run",&Tango::Util::server_run)
	.def("python_init",python_init)
	.staticmethod("python_init")
	.def("get_dserver_device",&Tango::Util::get_dserver_device,return_value_policy<reference_existing_object>())
	.def("trigger_cmd_polling",&Tango::Util::trigger_cmd_polling)
	.def("trigger_attr_polling",&Tango::Util::trigger_attr_polling)
	
	.def("get_ds_inst_name", &Tango::Util::get_ds_inst_name, 
			return_value_policy<copy_non_const_reference>(),
			"Get the device server instance name.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : the device server instance name ")

	.def("get_ds_exec_name", &Tango::Util::get_ds_exec_name, 
			return_value_policy<copy_non_const_reference>(),
			"Get the device server executable name.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : the device server executable name")

	.def("get_ds_name", &Tango::Util::get_ds_name, 
			return_value_policy<copy_non_const_reference>(),
			"Get the device server name.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : the device server name")

	.def("get_host_name", &Tango::Util::get_host_name, 
			return_value_policy<copy_non_const_reference>(),
			"Get the host name where the device server process is running.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : the host name")

	.def("get_pid_str", &Tango::Util::get_pid_str, 
			return_value_policy<copy_non_const_reference>(),
			"Get the device server process identifier as a string.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : the device server process identifier as a string")

	.def("get_tango_lib_release", &Tango::Util::get_tango_lib_release,
			"Get the TANGO C++ library version number.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : The Tango library release number coded in 3 digits "
			"(for instance 550,551,552,600,....)")

	.def("get_version_str", &Tango::Util::get_version_str, 
			return_value_policy<copy_non_const_reference>(),
			"Get the IDL TANGO version.\n"
			"\nNew in PyTango 3.0.4\n\n"
			"Return     : The device server version")

	.def("get_ds_name",&Tango::Util::get_ds_name,return_value_policy<copy_non_const_reference>())
	
	.def_readonly("_UseDb",&Tango::Util::_UseDb)
	.def_readonly("_FileDb",&Tango::Util::_FileDb)
	;
	
      	class_<CppDeviceClass,CppDeviceClassWrap,boost::noncopyable>("CppDeviceClass",init<std::string &>())
     	.def("device_factory",&CppDeviceClassWrap::device_factory)
	.def("export_device",&CppDeviceClass::export_device,DevClass_ed_over())
	.def("cpp_add_device",&CppDeviceClass::add_device)
#if ((defined sun) || (defined WIN32))
	.def("register_signal",&Tango::DeviceClass::register_signal)
#else
	.def("register_signal",&Tango::DeviceClass::register_signal,Class_rs_over())
#endif
	.def("unregister_signal",&Tango::DeviceClass::unregister_signal,Class_urs_over())
     	.def("signal_handler",&Tango::DeviceClass::signal_handler,&CppDeviceClassWrap::default_signal_handler)
	.def("get_name",&Tango::DeviceClass::get_name,return_value_policy<copy_non_const_reference>())
	.def("set_type",set_ty)
	.def("get_cvs_tag",&Tango::DeviceClass::get_cvs_tag,return_value_policy<copy_non_const_reference>())
	.def("get_cvs_location",&Tango::DeviceClass::get_cvs_location,return_value_policy<copy_non_const_reference>())
	.def("add_wiz_dev_prop",Cl_dev_1)
	.def("add_wiz_dev_prop",Cl_dev_2)
	.def("add_wiz_class_prop",Cl_cla_1)
	.def("add_wiz_class_prop",Cl_cla_2)
	.def("cpp_device_destroyer",d_destroy)
	;


	class_<Tango::DeviceImpl,DeviceImplWrap,boost::noncopyable>("DeviceImpl",init<CppDeviceClass *,std::string &>())
	.def(init<CppDeviceClass *,const char *,optional<const char *,Tango::DevState, const char *> >())
     	.def("init_device",pure_virtual(&Tango::DeviceImpl::init_device))
	.def("set_state",&Tango::DeviceImpl::set_state)
	.def("get_state",&Tango::DeviceImpl::get_state,return_value_policy<copy_non_const_reference>())
	.def("get_name",&Tango::DeviceImpl::get_name,return_value_policy<copy_non_const_reference>())
	.def("get_device_attr",&Tango::DeviceImpl::get_device_attr,return_value_policy<reference_existing_object>())
#if ((defined sun) || (defined WIN32))
	.def("register_signal",&Tango::DeviceImpl::register_signal)
#else
	.def("register_signal",&Tango::DeviceImpl::register_signal,Dev_rs_over())
#endif
	.def("unregister_signal",&Tango::DeviceImpl::unregister_signal)
	.def("get_status",&Tango::DeviceImpl::get_status,return_value_policy<copy_non_const_reference>())
	.def("set_status",&Tango::DeviceImpl::set_status)
	.def("dev_state",&Tango::DeviceImpl::dev_state)
	.def("dev_status",&Tango::DeviceImpl::dev_status)
	.def("set_change_event",&Tango::DeviceImpl::set_change_event,set_ch_event_over())
	.def("set_archive_event",&Tango::DeviceImpl::set_archive_event,set_ar_event_over())
	.def("adm_name",&DeviceImplWrap::PyTg_call_adm_name)
	;
	

	class_<Tango::Device_2Impl,Device_2ImplWrap,bases<Tango::DeviceImpl>,boost::noncopyable>("Device_2Impl",init<CppDeviceClass *,std::string &>())
	.def(init<CppDeviceClass *,const char *,optional<const char *,Tango::DevState, const char *> >())
	;
	
	class_<Tango::Device_3Impl,Device_3ImplWrap,bases<Tango::Device_2Impl>,boost::noncopyable>("Device_3Impl",init<CppDeviceClass *,std::string &>())
	.def(init<CppDeviceClass *,const char *,optional<const char *,Tango::DevState, const char *> >())
     	.def("always_executed_hook",&Tango::Device_3Impl::always_executed_hook,&Device_3ImplWrap::default_always_executed_hook)
     	.def("delete_device",&Tango::Device_3Impl::delete_device,&Device_3ImplWrap::default_delete_device)
     	.def("signal_handler",&Tango::Device_3Impl::signal_handler,&Device_3ImplWrap::default_signal_handler)
     	.def("read_attr_hardware",&Tango::Device_3Impl::read_attr_hardware,&Device_3ImplWrap::default_read_attr_hardware)
	.def("cpp_add_attribute",&Device_3ImplWrap::cpp_add_attribute)
	
	.def("cpp_remove_attribute",&Device_3ImplWrap::cpp_remove_attribute,
			"Remove one attribute from the device attribute list.\n"
			"\nNew in PyTango 3.0.4")
	;

	class_<Tango::DServer,bases<Tango::Device_3Impl>,boost::noncopyable>("DServer",no_init)
	;
	
	def("set_attribute_value",set_att_1);
	def("set_attribute_value",set_att_3);
	def("set_attribute_value",set_att_2);
	def("set_attribute_value",set_att_4);
	def("set_attribute_value",set_att_5);
	def("set_attribute_value",set_att_6);
	def("set_attribute_value",set_att_11);
	def("set_attribute_value",set_att_12);
	def("set_attribute_value",set_att_7);
	def("set_attribute_value",set_att_8);
	def("set_attribute_value",set_att_9);
	def("set_attribute_value",set_att_10);
	
	def("set_attribute_value_date_quality",set_att_dq_1);
	def("set_attribute_value_date_quality",set_att_dq_3);
	def("set_attribute_value_date_quality",set_att_dq_2);
	def("set_attribute_value_date_quality",set_att_dq_4);
	def("set_attribute_value_date_quality",set_att_dq_5);
	def("set_attribute_value_date_quality",set_att_dq_6);
	def("set_attribute_value_date_quality",set_att_dq_11);
	def("set_attribute_value_date_quality",set_att_dq_12);
	def("set_attribute_value_date_quality",set_att_dq_7);
	def("set_attribute_value_date_quality",set_att_dq_8);
	def("set_attribute_value_date_quality",set_att_dq_9);
	def("set_attribute_value_date_quality",set_att_dq_10);
	

	def("set_write_value",WAtt_set_val_1,
		"Set the writable scalar attribute value. This method is applicable for"
		"the Tango scalar types: Dev Char, Short, Long, Long64, State and their"
		"unsigned siblings (when existing).\n"
		"Parameters : \n"
		"        - data : the integer scalar value to be set as write value\n"
		"Return     : None");	
	
	def("set_write_value",WAtt_set_val_2,
		"Set the writable scalar attribute value. This method is applicable for"
		"the Tango scalar types: DevFloat and DevDouble.\n"
		"Parameters : \n"
		"        - data : the integer scalar value to be set as write value\n"
		"Return     : None");	

	def("set_write_value",WAtt_set_val_3,
		"Set the writable scalar attribute value. This method is applicable for"
		"the Tango scalar type DevString\n"
		"Parameters : \n"
		"        - data : the integer scalar value to be set as write value\n"
		"Return     : None");	
	
	def("set_write_value",WAtt_set_val_4,
		"Set the writable spectrum or image attribute value. Applicable for all"
		"spectrum or image Tango types.\n"
		"Attention: The dim_x defaults to 1 and the dim_y defaults to 0!\n"
		"\nNew in PyTango 3.0.4\n\n"
		"Parameters : \n"
		"        - data : a list containning the value to be set as write value\n"
		"Return     : None");
	
	def("set_write_value",WAtt_set_val_5,
		"Set the writable spectrum or image attribute value. Applicable for all"
		"spectrum or image Tango types.\n"
		"Attention: The dim_y defaults to 0!\n"
		"\nNew in PyTango 3.0.4\n\n"
		"Parameters : \n"
		"        - data : a list containning the value to be set as write value\n"
		"        - x : the attribute set value x length.n"
		"Return     : None");
	
	def("set_write_value", WAtt_set_val_6,
		"Set the writable spectrum or image attribute value. Applicable for all"
		"spectrum or image Tango types.\n"
		"\nNew in PyTango 3.0.4\n\n"
		"Parameters : \n"
		"        - data : a list containning the value to be set as write value\n"
		"        - x : the attribute set value x length\n"
		"        - y : the attribute set value y length\n"
		"Return     : None");
	
	def("get_write_value",get_write_value);
	
	def("set_min_value",WAtt_set_min_val_2);
	def("set_min_value",WAtt_set_min_val_1);
	def("get_min_value",WAtt_get_min_val_2);
	def("get_min_value",WAtt_get_min_val_1);
	def("is_min_value",WAtt_is_min_val);

	def("set_max_value",WAtt_set_max_val_2);
	def("set_max_value",WAtt_set_max_val_1);
	def("get_max_value",WAtt_get_max_val_2);
	def("get_max_value",WAtt_get_max_val_1);
	def("is_max_value",WAtt_is_max_val);
	
	def("push_att_event",push_ev_1);	
	def("push_att_event",push_ev_3);
	def("push_att_event",push_ev_2);
	def("push_att_event",push_ev_4);
	def("push_att_event",push_ev_5);
	def("push_att_event",push_ev_6);
	def("push_att_event",push_ev_11);
	def("push_att_event",push_ev_12);
	def("push_att_event",push_ev_7);
	def("push_att_event",push_ev_8);
	def("push_att_event",push_ev_9);
	def("push_att_event",push_ev_10);
	def("push_att_event",push_ev_dq_1);	
	def("push_att_event",push_ev_dq_3);
	def("push_att_event",push_ev_dq_2);
	def("push_att_event",push_ev_dq_4);
	def("push_att_event",push_ev_dq_5);
	def("push_att_event",push_ev_dq_6);
	def("push_att_event",push_ev_dq_11);
	def("push_att_event",push_ev_dq_12);
	def("push_att_event",push_ev_dq_7);
	def("push_att_event",push_ev_dq_8);
	def("push_att_event",push_ev_dq_9);
	def("push_att_event",push_ev_dq_10);
	def("push_att_event",push_ev_ex);
	def("push_att_event",push_ev_st);
	
	def("push_att_ch_event",push_ch_ev_1);	
	def("push_att_ch_event",push_ch_ev_3);
	def("push_att_ch_event",push_ch_ev_2);
	def("push_att_ch_event",push_ch_ev_4);
	def("push_att_ch_event",push_ch_ev_5);
	def("push_att_ch_event",push_ch_ev_6);
	def("push_att_ch_event",push_ch_ev_11);
	def("push_att_ch_event",push_ch_ev_12);
	def("push_att_ch_event",push_ch_ev_7);
	def("push_att_ch_event",push_ch_ev_8);
	def("push_att_ch_event",push_ch_ev_9);
	def("push_att_ch_event",push_ch_ev_10);
	def("push_att_ch_event",push_ch_ev_dq_1);	
	def("push_att_ch_event",push_ch_ev_dq_3);
	def("push_att_ch_event",push_ch_ev_dq_2);
	def("push_att_ch_event",push_ch_ev_dq_4);
	def("push_att_ch_event",push_ch_ev_dq_5);
	def("push_att_ch_event",push_ch_ev_dq_6);
	def("push_att_ch_event",push_ch_ev_dq_11);
	def("push_att_ch_event",push_ch_ev_dq_12);
	def("push_att_ch_event",push_ch_ev_dq_7);
	def("push_att_ch_event",push_ch_ev_dq_8);
	def("push_att_ch_event",push_ch_ev_dq_9);
	def("push_att_ch_event",push_ch_ev_dq_10);
	def("push_att_ch_event",push_ch_ev_ex);
	def("push_att_ch_event",push_ch_ev_st);
	
	def("push_att_ar_event",push_ar_ev_1);	
	def("push_att_ar_event",push_ar_ev_3);
	def("push_att_ar_event",push_ar_ev_2);
	def("push_att_ar_event",push_ar_ev_4);
	def("push_att_ar_event",push_ar_ev_5);
	def("push_att_ar_event",push_ar_ev_6);
	def("push_att_ar_event",push_ar_ev_11);
	def("push_att_ar_event",push_ar_ev_12);
	def("push_att_ar_event",push_ar_ev_7);
	def("push_att_ar_event",push_ar_ev_8);
	def("push_att_ar_event",push_ar_ev_9);
	def("push_att_ar_event",push_ar_ev_10);
	def("push_att_ar_event",push_ar_ev_dq_1);	
	def("push_att_ar_event",push_ar_ev_dq_3);
	def("push_att_ar_event",push_ar_ev_dq_2);
	def("push_att_ar_event",push_ar_ev_dq_4);
	def("push_att_ar_event",push_ar_ev_dq_5);
	def("push_att_ar_event",push_ar_ev_dq_6);
	def("push_att_ar_event",push_ar_ev_dq_11);
	def("push_att_ar_event",push_ar_ev_dq_12);
	def("push_att_ar_event",push_ar_ev_dq_7);
	def("push_att_ar_event",push_ar_ev_dq_8);
	def("push_att_ar_event",push_ar_ev_dq_9);
	def("push_att_ar_event",push_ar_ev_dq_10);
	def("push_att_ar_event",push_ar_ev_ex);
	def("push_att_ar_event",push_ar_ev_st);
	
	class_<Tango::Attribute>("Attribute",no_init)
	.def("get_name",&Tango::Attribute::get_name,return_value_policy<copy_non_const_reference>())
	.def("set_quality",&Tango::Attribute::set_quality,Att_set_quality_over())
	.def("check_alarm",&Tango::Attribute::check_alarm)
	;
	
	class_<Tango::WAttribute,bases<Tango::Attribute> >("WAttribute",no_init)
	.def("get_write_value_length",&Tango::WAttribute::get_write_value_length)
	;

	class_<Tango::MultiAttribute,boost::noncopyable>("MultiAttribute",no_init)
	.def("get_w_attr_by_name",&Tango::MultiAttribute::get_w_attr_by_name,return_value_policy<reference_existing_object>())
	.def("get_w_attr_by_ind",&Tango::MultiAttribute::get_w_attr_by_ind,return_value_policy<reference_existing_object>())
	.def("get_attr_by_name",&Tango::MultiAttribute::get_attr_by_name,return_value_policy<reference_existing_object>())
	.def("get_attr_by_ind",&Tango::MultiAttribute::get_attr_by_ind,return_value_policy<reference_existing_object>())
	;
	
	class_<Tango::UserDefaultAttrProp,boost::noncopyable>("UserDefaultAttrProp")
	.def("set_label",&Tango::UserDefaultAttrProp::set_label)
	.def("set_description",&Tango::UserDefaultAttrProp::set_description)
	.def("set_format",&Tango::UserDefaultAttrProp::set_format)
	.def("set_unit",&Tango::UserDefaultAttrProp::set_unit)
	.def("set_standard_unit",&Tango::UserDefaultAttrProp::set_standard_unit)
	.def("set_display_unit",&Tango::UserDefaultAttrProp::set_display_unit)
	.def("set_min_value",&Tango::UserDefaultAttrProp::set_min_value)
	.def("set_max_value",&Tango::UserDefaultAttrProp::set_max_value)
	.def("set_min_alarm",&Tango::UserDefaultAttrProp::set_min_alarm)
	.def("set_max_alarm",&Tango::UserDefaultAttrProp::set_max_alarm)
	.def("set_min_warning",&Tango::UserDefaultAttrProp::set_min_warning)
	.def("set_max_warning",&Tango::UserDefaultAttrProp::set_max_warning)
	.def("set_delta_t",&Tango::UserDefaultAttrProp::set_delta_t)
	.def("set_delta_val",&Tango::UserDefaultAttrProp::set_delta_val)
	.def("set_abs_change",&Tango::UserDefaultAttrProp::set_abs_change)
	.def("set_rel_change",&Tango::UserDefaultAttrProp::set_rel_change)
	.def("set_period",&Tango::UserDefaultAttrProp::set_period)
	.def("set_archive_abs_change",&Tango::UserDefaultAttrProp::set_archive_abs_change)
	.def("set_archive_rel_change",&Tango::UserDefaultAttrProp::set_archive_rel_change)
	.def("set_archive_period",&Tango::UserDefaultAttrProp::set_archive_period)
	;

	class_<Tango::Attr,boost::noncopyable>("Attr",init<const char *,long,optional<Tango::AttrWriteType,const char *> >())
	.def("set_default_properties",&Tango::Attr::set_default_properties)
	.def("get_name",&Tango::Attr::get_name,return_value_policy<copy_non_const_reference>())
	;
	
	class_<Tango::SpectrumAttr,bases<Tango::Attr>,boost::noncopyable>("SpectrumAttr",init<const char *,long,Tango::AttrWriteType,long>())
	;

	class_<Tango::ImageAttr,bases<Tango::SpectrumAttr>,boost::noncopyable>("ImageAttr",init<const char *,long,Tango::AttrWriteType,long,long>())
	;
	
	declare_tangogroup_base();
}
