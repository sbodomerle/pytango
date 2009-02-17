//+============================================================================
//
// file :               from_py.h
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.2 $
//
// $Log: from_py.h,v $
// Revision 1.2  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.1  2006/01/18 14:47:33  taurel
// - Added files when adding device server features in the Python binding
//
// Revision 1.3  2006/01/10 11:35:28  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.2  2006/01/03 10:16:15  etaurel
// - Commit after many changes and beautifullying code
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


#ifndef _FROM_PY_H
#define _FROM_PY_H

#include <boost/python.hpp>
#include <tango.h>
#include <py_ds.h>

using namespace boost::python;


struct DevVarCharArray_from_python
{
	DevVarCharArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarCharArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarShortArray_from_python
{
	DevVarShortArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarShortArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarLongArray_from_python
{
	DevVarLongArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarLongArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarFloatArray_from_python
{
	DevVarFloatArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarFloatArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarDoubleArray_from_python
{
	DevVarDoubleArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarDoubleArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarUShortArray_from_python
{
	DevVarUShortArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarUShortArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarULongArray_from_python
{
	DevVarULongArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarULongArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarStringArray_from_python
{
	DevVarStringArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarStringArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarLongStringArray_from_python
{
	DevVarLongStringArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarLongStringArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarDoubleStringArray_from_python
{
	DevVarDoubleStringArray_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarDoubleStringArray>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarLong64Array_from_python
{
	DevVarLong64Array_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarLong64Array>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct DevVarULong64Array_from_python
{
	DevVarULong64Array_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevVarULong64Array>());
	}		
	
	static void *convert_from_python_lvalue(PyObject *);
};


struct std_string_from_python
{
	std_string_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<std::string>());
	}

	static void *convert_from_python_lvalue(PyObject *);
};


struct std_vec_string_from_python
{
	std_vec_string_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<std::vector<string> >());
	}

	static void *convert_from_python_lvalue(PyObject *);
};

struct std_vec_db_from_python
{
	std_vec_db_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<std::vector<double> >());
	}

	static void *convert_from_python_lvalue(PyObject *);
};

struct DevErrorList_from_python
{
	DevErrorList_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<Tango::DevErrorList>());

	}

	static void *convert_from_python_lvalue(PyObject *);
};

struct const_char_ptr_from_python
{
	const_char_ptr_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<const char *>());

	}

	static void *convert_from_python_lvalue(PyObject *);
};

struct Ch_ptr_from_python
{
	Ch_ptr_from_python()
	{
		converter::registry::insert(convert_from_python_lvalue,type_id<char *>());
	}

	static void *convert_from_python_lvalue(PyObject *);
};


#endif // FROM_PY 
