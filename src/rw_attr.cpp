static const char *RcsId = "$Id: rw_attr.cpp,v 1.7 2007/06/13 07:22:38 taurel Exp $\n$Name:  $";

//+============================================================================
//
// file :               rw_attr.cpp
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.7 $
//
// $Log: rw_attr.cpp,v $
// Revision 1.7  2007/06/13 07:22:38  taurel
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
// Revision 1.6  2007/03/15 16:35:59  tiagocoutinho
// - refactoring of AttributeInfoEx. New API on WAttribute supports set(get)_min(max)_value
//
// Revision 1.5  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.4  2006/02/07 13:51:35  taurel
// - Miscellaneous changes to even simplify the way to write Tango DS in python
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
// Revision 1.3  2006/01/10 11:35:29  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.2  2005/12/14 15:44:07  etaurel
// - Now the AttrWrite test sequence is running well
//
// Revision 1.1  2005/12/14 10:24:15  etaurel
// - Now, the AttrMisc test file pass correctly
//
//
// copyleft :           CELLS - ALBA
//			Campus Universitari de Bellaterra. Universitat 
//			Aut�noma de Barcelona
//			08193 Bellaterra, Barcelona
//		  	SPAIN			
//
//-============================================================================

#include <math.h>
#include <templates.cpp>
#include <rw_attr.h>

//+-------------------------------------------------------------------------
//
// function : 		set_attribute_value
// 
// description : Set attribute value. This is a family of function with
//		 overloading used depending on attribute type.	
//
// argin(s) :   - att : The attribute
//		- the_list : The attribute data received as a Python list
//
//--------------------------------------------------------------------------

//
// For scalar attribute with default value given
//

void set_attribute_value(Tango::Attribute *att,Tango::DevLong64 val,long x)
{
	set_attribute_value(att,val);
}

void set_attribute_value(Tango::Attribute *att,Tango::DevLong64 val,long x,long y)
{
	set_attribute_value(att,val);
}

void set_attribute_value(Tango::Attribute *att,boost::python::str val,long x)
{
	set_attribute_value(att,val);
}

void set_attribute_value(Tango::Attribute *att,boost::python::str val,long x,long y)
{
	set_attribute_value(att,val);
}

void set_attribute_value(Tango::Attribute *att,double val,long x)
{
	set_attribute_value(att,val);
}

void set_attribute_value(Tango::Attribute *att,double val,long x,long y)
{
	set_attribute_value(att,val);
}

//
// For spectrum attribute
//

void set_attribute_value(Tango::Attribute *att,boost::python::list the_list)
{
	long list_len = PyList_Size(the_list.ptr());
	__set_attribute_value(att,the_list,list_len,0);
}

void set_attribute_value(Tango::Attribute *att,boost::python::list the_list,long x)
{
	__set_attribute_value(att,the_list,x,0);
}

//
// For image attribute
//

void set_attribute_value(Tango::Attribute *att,boost::python::list the_list,long x,long y)
{
	__set_attribute_value(att,the_list,x,y);
}


//
// A more generic one for spectrum and image attributes
//

void __set_attribute_value(Tango::Attribute *att,boost::python::list the_list,long x_dim,long y_dim)
{

	if (att->get_data_format() == Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
	long list_len = PyList_Size(the_list.ptr());

//
// Limit the max number of element to send to C++
//

	if (y_dim == 0)
	{
		if (x_dim < list_len)
			list_len = x_dim;
	}
	else
	{
		long max_elt = x_dim * y_dim;
		if (max_elt < list_len)
			list_len = max_elt;
	}
	
	int l;
	Tango::DevLong  *ptr_dl;
	Tango::DevULong *ptr_ulo;
	Tango::DevLong64 *ptr_dl64;
	Tango::DevULong64 *ptr_ulo64;
	Tango::DevFloat *ptr_fl;
	Tango::DevDouble *ptr_db;
	Tango::DevShort *ptr_sh;
	Tango::DevBoolean *ptr_bo;
	Tango::DevUShort *ptr_ush;
	Tango::DevUChar *ptr_uch;
	Tango::DevState *ptr_sta;

	char **ptr_str;
		
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		ptr_sh = Tango::DevVarShortArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_sh;
				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_sh[l] = (short)PyInt_AsLong(elt_ptr);
		}
		att->set_value(ptr_sh,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_LONG:
		ptr_dl = Tango::DevVarLongArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_dl;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_dl[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value(ptr_dl,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_ULONG:
		ptr_ulo = Tango::DevVarULongArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_ulo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ulo[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value(ptr_ulo,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_FLOAT:
		ptr_fl = Tango::DevVarFloatArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyFloat_Check(elt_ptr) == false)
			{
				delete  [] ptr_fl;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			ptr_fl[l] = (float)PyFloat_AsDouble(elt_ptr);
		}
		att->set_value(ptr_fl,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_DOUBLE:
		ptr_db = Tango::DevVarDoubleArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyFloat_Check(elt_ptr) == false)
			{
				delete  [] ptr_db;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			ptr_db[l] = PyFloat_AsDouble(elt_ptr);
		}
		att->set_value(ptr_db,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_BOOLEAN:
		ptr_bo = Tango::DevVarBooleanArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_bo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			long tmp = PyInt_AsLong(elt_ptr);
			if ((tmp == 0) || (tmp == 1))
				ptr_bo[l] = tmp;
			else
			{
				delete  [] ptr_bo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list is not a boolean" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
		}
		att->set_value(ptr_bo,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_USHORT:
		ptr_ush = Tango::DevVarUShortArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_ush;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ush[l] = (unsigned short)PyInt_AsLong(elt_ptr);
		}
		att->set_value(ptr_ush,list_len,y_dim,true);
		break;
		
	case Tango::DEV_UCHAR:
		ptr_uch = Tango::DevVarUCharArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_uch;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_uch[l] = (unsigned char)PyInt_AsLong(elt_ptr);
		}
		att->set_value(ptr_uch,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_STRING:
		ptr_str = new char *[list_len];

		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyString_Check(elt_ptr) == false)
			{
				for (int ll = 0;ll < l;ll++)
					delete [] ptr_str[ll];
				delete  [] ptr_str;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			char *ptr = PyString_AsString(elt_ptr);
			ptr_str[l] = new char[strlen(ptr) + 1];
			::strcpy(ptr_str[l],ptr);
		}
		att->set_value(ptr_str,x_dim,y_dim,true);
		break;

	case Tango::DEV_LONG64:
		ptr_dl64 = Tango::DevVarLong64Array::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if ((PyInt_Check(elt_ptr)) == false && (PyLong_Check(elt_ptr) == false))
			{
				delete  [] ptr_dl64;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_dl64[l] = PyLong_AsLongLong(elt_ptr);
		}
		att->set_value(ptr_dl64,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_ULONG64:
		ptr_ulo64 = Tango::DevVarULong64Array::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if ((PyInt_Check(elt_ptr)) == false && (PyLong_Check(elt_ptr) == false))
			{
				delete  [] ptr_ulo64;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ulo64[l] = PyLong_AsLongLong(elt_ptr);
		}
		att->set_value(ptr_ulo64,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_STATE:
		ptr_sta = Tango::DevVarStateArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if ((PyInt_Check(elt_ptr)) == false && (PyLong_Check(elt_ptr) == false))
			{
				delete  [] ptr_sta;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_sta[l] = (Tango::DevState)PyLong_AsLong(elt_ptr);
		}
		att->set_value(ptr_sta,x_dim,y_dim,true);
		break;
	}
}

//
// For scalar attribute
//


void set_attribute_value(Tango::Attribute *att,Tango::DevLong64 the_value)
{
	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}

	Tango::DevShort *ptr_sh;
	Tango::DevLong *ptr_lo;
	Tango::DevLong64 *ptr_lo64;
	Tango::DevUShort *ptr_us;
	Tango::DevUChar *ptr_uc;
	Tango::DevBoolean *ptr_bo;
	Tango::DevULong *ptr_ulo;
	Tango::DevULong64 *ptr_ulo64;
	Tango::DevState *ptr_sta;
	
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		ptr_sh = Tango::DevVarShortArray::allocbuf(1);
		*ptr_sh = static_cast<short>(the_value);
		att->set_value(ptr_sh,1,0,true);
		break;
		
	case Tango::DEV_LONG:
		ptr_lo = Tango::DevVarLongArray::allocbuf(1);
		*ptr_lo = static_cast<long>(the_value);
		att->set_value(ptr_lo,1,0,true);
		break;

	case Tango::DEV_USHORT:
		ptr_us = Tango::DevVarUShortArray::allocbuf(1);
		*ptr_us = static_cast<unsigned short>(the_value);
		att->set_value(ptr_us,1,0,true);
		break;
		
	case Tango::DEV_UCHAR:
		ptr_uc = Tango::DevVarUCharArray::allocbuf(1);
		*ptr_uc = static_cast<unsigned char>(the_value);
		att->set_value(ptr_uc,1,0,true);
		break;
				
	case Tango::DEV_BOOLEAN:
		ptr_bo = Tango::DevVarBooleanArray::allocbuf(1);
		if (the_value == 0)
			*ptr_bo = false;
		else if (the_value == 1)
			*ptr_bo = true;
		else
		{
			delete ptr_bo;
			
			TangoSys_OMemStream o;
			o << "Wrong Python type for attribute " << att->get_name();
			o << "\nMust be a boolean" << ends;

			Tango::Except::throw_exception((const char *)"API_AttrOptProp",
				       o.str(),
				       (const char *)"set_attribute_value()");
		}
		
		att->set_value(ptr_bo,1,0,true);
		break;
		
	case Tango::DEV_ULONG:
		ptr_ulo = Tango::DevVarULongArray::allocbuf(1);
		*ptr_ulo = static_cast<Tango::DevULong>(the_value);
		att->set_value(ptr_ulo,1,0,true);
		break;
	
	case Tango::DEV_LONG64:
		ptr_lo64 = Tango::DevVarLong64Array::allocbuf(1);
		*ptr_lo64 = static_cast<Tango::DevLong64>(the_value);
		att->set_value(ptr_lo64,1,0,true);
		break;
		
	case Tango::DEV_ULONG64:
		ptr_ulo64 = Tango::DevVarULong64Array::allocbuf(1);
		*ptr_ulo64 = static_cast<Tango::DevULong64>(the_value);
		att->set_value(ptr_ulo64,1,0,true);
		break;
		
	case Tango::DEV_STATE:
		ptr_sta = Tango::DevVarStateArray::allocbuf(1);
		*ptr_sta = static_cast<Tango::DevState>(the_value);
		att->set_value(ptr_sta,1,0,true);
		break;
					
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Float" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
		break;
	}
	
}

//
// For scalar string attribute
//

void set_attribute_value(Tango::Attribute *att,boost::python::str the_value)
{
	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
	if (att->get_data_type() != Tango::DEV_STRING)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a boolean" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}

	char *ptr = PyString_AsString(the_value.ptr());
	char **ptr_str;
	ptr_str = new char *[1];
	*ptr_str = new char[strlen(ptr) + 1];
	::strcpy(*ptr_str,ptr);
	
	att->set_value(ptr_str,1,0,true);	
}

//
// For scalar floating point attribute
//

void set_attribute_value(Tango::Attribute *att,double the_value)
{
	float *ptr_fl;
	double *ptr_db;

	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		ptr_fl = Tango::DevVarFloatArray::allocbuf(1);
		*ptr_fl = (float)the_value;
		att->set_value(ptr_fl,1,0,true);
		break;
		
	case Tango::DEV_DOUBLE:
		ptr_db = Tango::DevVarDoubleArray::allocbuf(1);
		*ptr_db = the_value;
		att->set_value(ptr_db,1,0,true);
		break;
		
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Float" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
		break;
	}
}



//+-------------------------------------------------------------------------
//
// function : 		set_attribute_value_date_quality
// 
// description : Set attribute value. This is a family of function with
//		 overloading used depending on attribute type.	
//
// argin(s) :   - att : The attribute
//		- the_list : The attribute data received as a Python list
//		- the_time : The date to be set in the attribute
//			     as retruned by a time.clock() python call
//		- qual : The attribute quality factor
//
//--------------------------------------------------------------------------


void set_attribute_value_dq(Tango::Attribute *att,Tango::DevLong64 the_value,double the_time,Tango::AttrQuality qual)
{

	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}

	Tango::DevShort *ptr_sh;
	Tango::DevLong *ptr_lo;
	Tango::DevLong64 *ptr_lo64;
	Tango::DevUShort *ptr_us;
	Tango::DevUChar *ptr_uc;
	Tango::DevBoolean *ptr_bo;
	Tango::DevULong *ptr_ulo;
	Tango::DevULong64 *ptr_ulo64;
	
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		ptr_sh = Tango::DevVarShortArray::allocbuf(1);
		*ptr_sh = static_cast<Tango::DevShort>(the_value);
		att->set_value_date_quality(ptr_sh,(time_t)the_time,qual,1,0,true);
		break;
		
	case Tango::DEV_LONG:
		ptr_lo = Tango::DevVarLongArray::allocbuf(1);
		*ptr_lo = static_cast<Tango::DevLong>(the_value);
		att->set_value_date_quality(ptr_lo,(time_t)the_time,qual,1,0,true);
		break;

	case Tango::DEV_USHORT:
		ptr_us = Tango::DevVarUShortArray::allocbuf(1);
		*ptr_us = static_cast<Tango::DevUShort>(the_value);
		att->set_value_date_quality(ptr_us,(time_t)the_time,qual,1,0,true);
		break;
		
	case Tango::DEV_UCHAR:
		ptr_uc = Tango::DevVarUCharArray::allocbuf(1);
		*ptr_uc = static_cast<Tango::DevUChar>(the_value);
		att->set_value_date_quality(ptr_uc,(time_t)the_time,qual,1,0,true);
		break;
				
	case Tango::DEV_BOOLEAN:
		ptr_bo = Tango::DevVarBooleanArray::allocbuf(1);
		if (the_value == 0)
			*ptr_bo = false;
		else if (the_value == 1)
			*ptr_bo = true;
		else
		{
			delete ptr_bo;
			
			TangoSys_OMemStream o;
			o << "Wrong Python type for attribute " << att->get_name();
			o << "\nMust be a boolean" << ends;

			Tango::Except::throw_exception((const char *)"API_AttrOptProp",
				       o.str(),
				       (const char *)"set_attribute_value()");
		}
		
		att->set_value_date_quality(ptr_bo,(time_t)the_time,qual,1,0,true);
		break;

	case Tango::DEV_ULONG:
		ptr_ulo = Tango::DevVarULongArray::allocbuf(1);
		*ptr_ulo = static_cast<Tango::DevULong>(the_value);
		att->set_value_date_quality(ptr_ulo,(time_t)the_time,qual,1,0,true);
		break;	
		
	case Tango::DEV_ULONG64:
		ptr_ulo64 = Tango::DevVarULong64Array::allocbuf(1);
		*ptr_ulo64 = static_cast<Tango::DevULong64>(the_value);
		att->set_value_date_quality(ptr_ulo64,(time_t)the_time,qual,1,0,true);
		break;	
			
	case Tango::DEV_LONG64:
		ptr_lo64 = Tango::DevVarLong64Array::allocbuf(1);
		*ptr_lo64 = static_cast<Tango::DevLong64>(the_value);
		att->set_value_date_quality(ptr_lo64,(time_t)the_time,qual,1,0,true);
		break;	
	
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Float" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
		break;
	}
	
}

void set_attribute_value_dq(Tango::Attribute *att,double the_value,double the_time,Tango::AttrQuality qual)
{
	float *ptr_fl;
	double *ptr_db;

	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
		
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		ptr_fl = Tango::DevVarFloatArray::allocbuf(1);
		*ptr_fl = (float)the_value;
		att->set_value_date_quality(ptr_fl,(time_t)the_time,qual,1,0,true);
		break;
		
	case Tango::DEV_DOUBLE:
		ptr_db = Tango::DevVarDoubleArray::allocbuf(1);
		*ptr_db = the_value;
		att->set_value_date_quality(ptr_db,(time_t)the_time,qual,1,0,true);
		break;
		
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Float" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
		break;
	}
}


void set_attribute_value_dq(Tango::Attribute *att,boost::python::str the_value,double the_time,Tango::AttrQuality qual)
{
	if (att->get_data_format() != Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
	if (att->get_data_type() != Tango::DEV_STRING)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a boolean" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}

	char *ptr = PyString_AsString(the_value.ptr());
	char **ptr_str;
	ptr_str = new char *[1];
	*ptr_str = new char[strlen(ptr) + 1];
	::strcpy(*ptr_str,ptr);
	
	att->set_value_date_quality(ptr_str,(time_t)the_time,qual,1,0,true);	
}


void set_attribute_value_dq(Tango::Attribute *att,boost::python::list the_list,double the_time,Tango::AttrQuality qual)
{
	long list_len = PyList_Size(the_list.ptr());
	__set_attribute_value_dq(att,the_list,the_time,qual,list_len,0);
}

void set_attribute_value_dq(Tango::Attribute *att,boost::python::list the_list,double the_time,Tango::AttrQuality qual,long x)
{
	__set_attribute_value_dq(att,the_list,the_time,qual,x,0);
}

//
// For image attribute
//

void set_attribute_value_dq(Tango::Attribute *att,boost::python::list the_list,double the_time,Tango::AttrQuality qual,long x,long y)
{
	__set_attribute_value_dq(att,the_list,the_time,qual,x,y);
}

void __set_attribute_value_dq(Tango::Attribute *att,boost::python::list the_list,double the_time,Tango::AttrQuality qual,long x_dim,long y_dim)
{
	if (att->get_data_format() == Tango::SCALAR)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
			       o.str(),
			       (const char *)"set_attribute_value()");
	}
	
	long list_len = PyList_Size(the_list.ptr());

//
// Limit the max number of element to send to C++
//

	if (y_dim == 0)
	{
		if (x_dim < list_len)
			list_len = x_dim;
	}
	else
	{
		long max_elt = x_dim * y_dim;
		if (max_elt < list_len)
			list_len = max_elt;
	}
	
	int l;
	Tango::DevLong *ptr_dl;
	Tango::DevLong64 *ptr_dl64;
	Tango::DevFloat *ptr_fl;
	Tango::DevDouble *ptr_db;
	Tango::DevShort *ptr_sh;
	Tango::DevBoolean *ptr_bo;
	Tango::DevUShort *ptr_ush;
	Tango::DevUChar *ptr_uch;
	Tango::DevULong *ptr_ulo;
	Tango::DevULong64 *ptr_ulo64;

	char **ptr_str;
		
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		ptr_sh = Tango::DevVarShortArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_sh;
				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_sh[l] = (short)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_sh,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_LONG:
		ptr_dl = Tango::DevVarLongArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_dl;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_dl[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_dl,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_ULONG:
		ptr_ulo = Tango::DevVarULongArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_ulo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ulo[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_ulo,(time_t)the_time,qual,x_dim,y_dim,true);
		break;

	case Tango::DEV_FLOAT:
		ptr_fl = Tango::DevVarFloatArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyFloat_Check(elt_ptr) == false)
			{
				delete  [] ptr_fl;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			ptr_fl[l] = (float)PyFloat_AsDouble(elt_ptr);
		}
		att->set_value_date_quality(ptr_fl,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_DOUBLE:
		ptr_db = Tango::DevVarDoubleArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyFloat_Check(elt_ptr) == false)
			{
				delete  [] ptr_db;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			ptr_db[l] = PyFloat_AsDouble(elt_ptr);
		}
		att->set_value_date_quality(ptr_db,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_BOOLEAN:
		ptr_bo = Tango::DevVarBooleanArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_bo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			long tmp = PyInt_AsLong(elt_ptr);
			if ((tmp == 0) || (tmp == 1))
				ptr_bo[l] = tmp;
			else
			{
				delete  [] ptr_bo;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list is not a boolean" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
		}
		att->set_value_date_quality(ptr_bo,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_USHORT:
		ptr_ush = Tango::DevVarUShortArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_ush;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ush[l] = (unsigned short)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_ush,(time_t)the_time,qual,list_len,y_dim,true);
		break;
		
	case Tango::DEV_UCHAR:
		ptr_uch = Tango::DevVarUCharArray::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_uch;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_uch[l] = (unsigned char)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_uch,(time_t)the_time,qual,x_dim,y_dim,true);
		break;
		
	case Tango::DEV_STRING:
		ptr_str = new char *[list_len];

		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyString_Check(elt_ptr) == false)
			{
				for (int ll = 0;ll < l;ll++)
					delete [] ptr_str[ll];
				delete  [] ptr_str;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
			char *ptr = PyString_AsString(elt_ptr);
			ptr_str[l] = new char[strlen(ptr) + 1];
			::strcpy(ptr_str[l],ptr);
		}
		att->set_value_date_quality(ptr_str,(time_t)the_time,qual,x_dim,y_dim,true);
		break;

	case Tango::DEV_LONG64:
		ptr_dl64 = Tango::DevVarLong64Array::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_dl64;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_dl64[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_dl64,(time_t)the_time,qual,x_dim,y_dim,true);
		break;	
		
	case Tango::DEV_ULONG64:
		ptr_ulo64 = Tango::DevVarULong64Array::allocbuf(list_len);
		for (l = 0;l < list_len;l++)
		{
			PyObject *elt_ptr = PyList_GetItem(the_list.ptr(),l);
			if (PyInt_Check(elt_ptr) == false)
			{
				delete  [] ptr_ulo64;

				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nElement number " << l << " in list does not have a correct type" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongPythonDataTypeForAttribute",
					       o.str(),
					       (const char *)"set_attribute_value()");
			}
        		ptr_ulo64[l] = (int)PyInt_AsLong(elt_ptr);
		}
		att->set_value_date_quality(ptr_ulo64,(time_t)the_time,qual,x_dim,y_dim,true);
		break;		
	}
}

//
// For scalar attribute with default value given
//

void set_attribute_value_dq(Tango::Attribute *att,Tango::DevLong64 val,double the_time,Tango::AttrQuality qual,long x)
{
	set_attribute_value_dq(att,val,the_time,qual);
}

void set_attribute_value_dq(Tango::Attribute *att,Tango::DevLong64 val,double the_time,Tango::AttrQuality qual,long x,long y)
{
	set_attribute_value_dq(att,val,the_time,qual);
}

void set_attribute_value_dq(Tango::Attribute *att,boost::python::str val,double the_time,Tango::AttrQuality qual,long x)
{
	set_attribute_value_dq(att,val,the_time,qual);
}

void set_attribute_value_dq(Tango::Attribute *att,boost::python::str val,double the_time,Tango::AttrQuality qual,long x,long y)
{
	set_attribute_value_dq(att,val,the_time,qual);
}

void set_attribute_value_dq(Tango::Attribute *att,double val,double the_time,Tango::AttrQuality qual,long x)
{
	set_attribute_value_dq(att,val,the_time,qual);
}

void set_attribute_value_dq(Tango::Attribute *att,double val,double the_time,Tango::AttrQuality qual,long x,long y)
{
	set_attribute_value_dq(att,val,the_time,qual);
}


		
//+-------------------------------------------------------------------------
//
// function : 		get_write_value
// 
// description : Get attribute value. This is called for writable attribute
//		 to get which value has been written by the client	
//
// argin(s) :   - att : The attribute
//		- the_list : The Python list which will be updated with the
//			     new data
//
//--------------------------------------------------------------------------

void get_write_value(Tango::WAttribute *att,boost::python::list the_data)
{

	long data_length = att->get_write_value_length();
		
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		init_list<Tango::DevShort>(data_length,att,the_data);
		break;	
		
	case Tango::DEV_LONG:
		init_list<Tango::DevLong>(data_length,att,the_data);
		break;	
		
	case Tango::DEV_ULONG:
		init_list<Tango::DevULong>(data_length,att,the_data);
		break;	
		
	case Tango::DEV_FLOAT:
		init_list<Tango::DevFloat>(data_length,att,the_data);
		break;
		
	case Tango::DEV_DOUBLE:
		init_list<Tango::DevDouble>(data_length,att,the_data);
		break;
		
	case Tango::DEV_BOOLEAN:
		init_list<Tango::DevBoolean>(data_length,att,the_data);
		break;
		
	case Tango::DEV_USHORT:
		init_list<Tango::DevUShort>(data_length,att,the_data);
		break;
		
	case Tango::DEV_UCHAR:
		init_list<Tango::DevUChar>(data_length,att,the_data);
		break;
		
	case Tango::DEV_STRING:
		init_list<Tango::ConstDevString>(data_length,att,the_data);
		break;
		
	case Tango::DEV_LONG64:
		init_list<Tango::DevLong64>(data_length,att,the_data);
		break;	
		
	case Tango::DEV_ULONG64:
		init_list<Tango::DevULong64>(data_length,att,the_data);
		break;	

	}
}


//+-------------------------------------------------------------------------
//
// function : 		set_write_value
// 
// description : Set writable attribute value.	
//
// argin(s) :   - att : The attribute
//		- the_data : The Python data
//
//--------------------------------------------------------------------------

void set_write_value(Tango::WAttribute *att,Tango::DevLong64 the_value)
{
	switch (att->get_data_type())
	{
		case Tango::DEV_SHORT:
		{
			att->set_write_value(static_cast<Tango::DevShort>(the_value));
			break;
		}
		
		case Tango::DEV_LONG:
		{
			att->set_write_value(static_cast<Tango::DevLong>(the_value));
			break;
		}
		
		case Tango::DEV_ULONG:
		{
			att->set_write_value(static_cast<Tango::DevULong>(the_value));
			break;
		}

		case Tango::DEV_USHORT:
		{
			att->set_write_value(static_cast<Tango::DevUShort>(the_value));
			break;
		}

		case Tango::DEV_UCHAR:
		{
			att->set_write_value(static_cast<Tango::DevUChar>(the_value));
			break;
		}
				
		case Tango::DEV_BOOLEAN:
		{
			if (the_value < 0 || the_value > 1)
			{			
				TangoSys_OMemStream o;
				o << "Wrong Python type for attribute " << att->get_name();
				o << "\nMust be a boolean" << ends;
	
				Tango::Except::throw_exception((const char *)"API_AttrOptProp",
					       o.str(),
					       (const char *)"set_write_value()");
			}
			
			att->set_write_value(the_value == 0);
			break;
		}
		
		case Tango::DEV_LONG64:
		{
			att->set_write_value(static_cast<Tango::DevLong64>(the_value));
			break;
		}		
		
		case Tango::DEV_ULONG64:
		{
			att->set_write_value(static_cast<Tango::DevULong64>(the_value));
			break;
		}		

		case Tango::DEV_STATE:
		{
			att->set_write_value(static_cast<Tango::DevState>(the_value));
			break;
		}		
		
		default:
		{
			TangoSys_OMemStream o;
			o << "Wrong Python type for attribute " << att->get_name();
			o << "\nMust not be a Float" << ends;
	
			Tango::Except::throw_exception((const char *)"API_AttrOptProp",
				       o.str(),
				       (const char *)"set_write_value()");
			break;
		}
	}
}

void set_write_value(Tango::WAttribute *att,boost::python::str the_value)
{

	if (att->get_data_type() != Tango::DEV_STRING)
	{
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name();
		o << "\nMust not be a boolean" << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_write_value()");
	}

	char *ptr = PyString_AsString(the_value.ptr());
	att->set_write_value(ptr);	
}


void set_write_value(Tango::WAttribute *att,double the_value)
{

	float fl;
	double db;

		
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		fl = (float)the_value;
		att->set_write_value(fl);
		break;
		
	case Tango::DEV_DOUBLE:
		db = the_value;
		att->set_write_value(db);
		break;
		
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for attribute " << att->get_name() << ends;

		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_write_value()");
		break;
	}
}

void set_write_value(Tango::WAttribute *att, boost::python::list the_value)
{
	set_write_value(att, the_value, 1);
}

void set_write_value(Tango::WAttribute *att, boost::python::list the_value,
                     long dim_x)
{
	set_write_value(att, the_value, dim_x, 0);
}

void set_write_value(Tango::WAttribute *att, boost::python::list the_value,
                     long dim_x, long dim_y)
{
	switch (att->get_data_type())
	{
		case Tango::DEV_SHORT:
		{
			vector<Tango::DevShort> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
		
		case Tango::DEV_LONG:
		{
			vector<Tango::DevLong> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
		
		case Tango::DEV_ULONG:
		{
			vector<Tango::DevULong> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}

		case Tango::DEV_USHORT:
		{
			vector<Tango::DevUShort> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}

		case Tango::DEV_UCHAR:
		{
			vector<Tango::DevUChar> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
				
		case Tango::DEV_BOOLEAN:
		{
			vector<Tango::DevBoolean> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
		
		case Tango::DEV_LONG64:
		{
			vector<Tango::DevLong64> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}		
		
		case Tango::DEV_ULONG64:
		{
			vector<Tango::DevULong64> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}		

		case Tango::DEV_STATE:
		{
			vector<Tango::DevState> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}		

		case Tango::DEV_FLOAT:
		{
			vector<Tango::DevFloat> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
		
		case Tango::DEV_DOUBLE:
		{
			vector<Tango::DevDouble> vec;
			get_vector_from_list(vec, the_value);
			att->set_write_value(vec, dim_x, dim_y);
			break;
		}
			
		default:
		{
			TangoSys_OMemStream o;
			o << "Wrong Python type for attribute " << att->get_name() << ends;

			Tango::Except::throw_exception((const char *)"API_AttrOptProp",
				       o.str(),
				       (const char *)"set_write_value()");
			break;
		}
	}	
}

void set_min_value(Tango::WAttribute *att,Tango::DevLong64 the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		Tango::DevShort sh;
		sh = static_cast<Tango::DevShort>(the_value);
		att->set_min_value(sh);
		break;
		
	case Tango::DEV_LONG:
		Tango::DevLong lo;
		lo = static_cast<Tango::DevLong>(the_value);
		att->set_min_value(lo);
		break;
		
	case Tango::DEV_ULONG:
		Tango::DevULong ulo;
		ulo = static_cast<Tango::DevULong>(the_value);
		att->set_min_value(ulo);
		break;

	case Tango::DEV_USHORT:
		Tango::DevUShort us;
		us = static_cast<Tango::DevUShort>(the_value);
		att->set_min_value(us);
		break;
		
	case Tango::DEV_UCHAR:
		Tango::DevUChar uc;
		uc = static_cast<Tango::DevUChar>(the_value);
		att->set_min_value(uc);
		break;
				
	case Tango::DEV_LONG64:
		att->set_write_value(the_value);
		break;
		
	case Tango::DEV_ULONG64:
		Tango::DevULong64 ulo64;
		ulo64 = static_cast<Tango::DevULong64>(the_value);
		att->set_min_value(us);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for min value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_min_value()");
		break;
	}
}

void set_min_value(Tango::WAttribute *att,double the_value)
{
	float fl;
	double db;
		
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		fl = (float)the_value;
		att->set_min_value(fl);
		break;
		
	case Tango::DEV_DOUBLE:
		db = the_value;
		att->set_min_value(db);
		break;
		
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for min value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_min_value()");
		break;
	}
}

void get_min_value(Tango::WAttribute *att,Tango::DevLong64 &the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		Tango::DevShort sh;
		att->get_min_value(sh);
		the_value = static_cast<Tango::DevLong64>(sh);
		break;
		
	case Tango::DEV_LONG:
		Tango::DevLong lo;
		att->get_min_value(lo);
		the_value = static_cast<Tango::DevLong64>(lo);
		break;

	case Tango::DEV_ULONG:
		Tango::DevULong ulo;
		att->get_min_value(ulo);
		the_value = static_cast<Tango::DevLong64>(ulo);
		break;

	case Tango::DEV_USHORT:
		Tango::DevUShort us;
		att->get_min_value(us);
		the_value = static_cast<Tango::DevLong64>(us);
		break;
		
	case Tango::DEV_UCHAR:
		Tango::DevUChar uc;
		att->get_min_value(uc);
		the_value = static_cast<Tango::DevLong64>(uc);
		break;
		
	case Tango::DEV_ULONG64:
		Tango::DevULong64 ulo64;
		att->get_min_value(ulo64);
		the_value = static_cast<Tango::DevLong64>(ulo64);
		break;
				
	case Tango::DEV_LONG64:
		att->get_min_value(the_value);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong type for min value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"get_min_value()");
		break;
	}
}

void get_min_value(Tango::WAttribute *att,double &the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		Tango::DevFloat fl;
		att->get_min_value(fl);
		the_value = static_cast<double>(fl);
		break;
		
	case Tango::DEV_DOUBLE:
		att->get_min_value(the_value);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong type for min value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"get_min_value()");
		break;
	}
}
 
void set_max_value(Tango::WAttribute *att,Tango::DevLong64 the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		Tango::DevShort sh;
		sh = static_cast<Tango::DevShort>(the_value);
		att->set_max_value(sh);
		break;
		
	case Tango::DEV_LONG:
		Tango::DevLong lo;
		lo = static_cast<Tango::DevLong>(the_value);
		att->set_max_value(lo);
		break;

	case Tango::DEV_USHORT:
		Tango::DevUShort us;
		us = static_cast<Tango::DevUShort>(the_value);
		att->set_max_value(us);
		break;
		
	case Tango::DEV_UCHAR:
		Tango::DevUChar uc;
		uc = static_cast<Tango::DevUChar>(the_value);
		att->set_max_value(uc);
		break;
				
	case Tango::DEV_LONG64:
		att->set_write_value(the_value);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for max value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_max_value()");
		break;
	}
}

void set_max_value(Tango::WAttribute *att,double the_value)
{
	float fl;
	double db;
		
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		fl = (float)the_value;
		att->set_max_value(fl);
		break;
		
	case Tango::DEV_DOUBLE:
		db = the_value;
		att->set_max_value(db);
		break;
		
	default:
		TangoSys_OMemStream o;
		o << "Wrong Python type for max value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"set_max_value()");
		break;
	}
}

void get_max_value(Tango::WAttribute *att,Tango::DevLong64 &the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_SHORT:
		Tango::DevShort sh;
		att->get_max_value(sh);
		the_value = static_cast<Tango::DevLong64>(sh);
		break;
		
	case Tango::DEV_LONG:
		Tango::DevLong lo;
		att->get_max_value(lo);
		the_value = static_cast<Tango::DevLong64>(lo);
		break;

	case Tango::DEV_USHORT:
		Tango::DevUShort us;
		att->get_max_value(us);
		the_value = static_cast<Tango::DevLong64>(us);
		break;
		
	case Tango::DEV_UCHAR:
		Tango::DevUChar uc;
		att->get_max_value(uc);
		the_value = static_cast<Tango::DevLong64>(uc);
		break;
				
	case Tango::DEV_LONG64:
		att->get_max_value(the_value);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong type for max value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"get_max_value()");
		break;
	}
}

void get_max_value(Tango::WAttribute *att,double &the_value)
{
	switch (att->get_data_type())
	{
	case Tango::DEV_FLOAT:
		Tango::DevFloat fl;
		att->get_max_value(fl);
		the_value = static_cast<double>(fl);
		break;
		
	case Tango::DEV_DOUBLE:
		att->get_max_value(the_value);
		break;
				
	default:
		TangoSys_OMemStream o;
		o << "Wrong type for max value of attribute " << att->get_name() << ends;
		Tango::Except::throw_exception((const char *)"API_AttrOptProp",
			       o.str(),
			       (const char *)"get_max_value()");
		break;
	}
}

bool is_min_value(Tango::WAttribute *att)
{
	return att->is_min_value();
}

bool is_max_value(Tango::WAttribute *att)
{
	return att->is_max_value();
}
