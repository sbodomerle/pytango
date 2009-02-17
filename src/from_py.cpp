static const char *RcsId = "$Id: from_py.cpp,v 1.5 2007/03/09 14:54:16 tiagocoutinho Exp $\n$Name:  $";

//+============================================================================
//
// file :               from_py.cpp
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.5 $
//
// $Log: from_py.cpp,v $
// Revision 1.5  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.4  2006/07/26 07:06:43  taurel
// - Add wrapping of all the new calls added in Tango V5.5 to manually firing events
//
// Revision 1.3  2006/05/17 15:56:36  taurel
// - Changes due to the Windows port using XP and VC8 (Visual Studio 2005)
//
// Revision 1.2  2006/01/27 12:52:51  taurel
// - Remove C++ vector used on client side between CORBA sequences and Python list
// - Replace list by tuple on server side for DevVarLongStringArray and DevVarDoubleStringArray
// - Add defualt command case in Py DS
//
// Revision 1.1  2006/01/18 14:47:33  taurel
// - Added files when adding device server features in the Python binding
//
// Revision 1.4  2006/01/10 11:35:28  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.3  2006/01/04 12:59:53  etaurel
// - Other changes to add the add_attribute feature
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


#include <from_py.h>


//
// The per thread data storage key
//

#ifdef WIN32
__declspec(dllimport) omni_thread::key_t key_py_data;
#else
extern omni_thread::key_t key_py_data;
#endif


//-------------------------------------------------------------------------------------------
//
//			BOOST data type converters
//
//-------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
//
//		For DevVarCharArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarCharArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarCharArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarCharArray *dvca_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvca);
		
	dvca_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<unsigned char> s_val(the_list[l]);
	  	(*dvca_ptr)[l] = s_val();
	}			

	return (void *)(dvca_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarShortArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarShortArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarShortArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarShortArray *dvsha_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvsha);
		
	dvsha_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<short> s_val(the_list[l]);
	  	(*dvsha_ptr)[l] = s_val();
	}			

	return (void *)(dvsha_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarLongArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarLongArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarLongArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarLongArray *dvla_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvla);
		
	dvla_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<long> s_val(the_list[l]);
	  	(*dvla_ptr)[l] = s_val();
	}			

	return (void *)(dvla_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarFloatArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarFloatArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarFloatArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarFloatArray *dvfa_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvfa);
		
	dvfa_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<float> s_val(the_list[l]);
	  	(*dvfa_ptr)[l] = s_val();
	}			

	return (void *)(dvfa_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarDoubleArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarDoubleArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarDoubleArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarDoubleArray *dvda_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvda);
		
	dvda_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<double> s_val(the_list[l]);
	  	(*dvda_ptr)[l] = s_val();
	}			

	return (void *)(dvda_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarUShortArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarUShortArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarUShortArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarUShortArray *dvusa_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvusa);
		
	dvusa_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<unsigned short> s_val(the_list[l]);
	  	(*dvusa_ptr)[l] = s_val();
	}			

	return (void *)(dvusa_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarULongArray
//
//--------------------------------------------------------------------------------------
	
void *DevVarULongArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarULongArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarULongArray *dvula_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvula);
		
	dvula_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<unsigned long> s_val(the_list[l]);
	  	(*dvula_ptr)[l] = s_val();
	}			

	return (void *)(dvula_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarStringArray
//
//--------------------------------------------------------------------------------------

	
void *DevVarStringArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevvarStringArray from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarStringArray *dvsa_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvsa);
		
	dvsa_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<std::string> s_val(the_list[l]);
	  	(*dvsa_ptr)[l] = s_val().c_str();
	}			

	return (void *)(dvsa_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarLong64Array
//
//--------------------------------------------------------------------------------------
	
void *DevVarLong64Array_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarLong64Array from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarLong64Array *dvl64a_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvl64a);
		
	dvl64a_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<long long> s_val(the_list[l]);
	  	(*dvl64a_ptr)[l] = s_val();
	}			

	return (void *)(dvl64a_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarULong64Array
//
//--------------------------------------------------------------------------------------
	
void *DevVarULong64Array_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarULong64Array from_python lvalue converter" << endl;

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
		
//	cout << "I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarULong64Array *dvul64a_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvul64a);
		
	dvul64a_ptr->length(list_len);
	for (int l = 0;l < list_len;l++)
	{
		extract<unsigned long long> s_val(the_list[l]);
	  	(*dvul64a_ptr)[l] = s_val();
	}			

	return (void *)(dvul64a_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For DevVarLongStringArray
//
//--------------------------------------------------------------------------------------

	
void *DevVarLongStringArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevvarStringArray from_python lvalue converter" << endl;

//
// Check that I have received a list with two elements whihc are also list
//

	if (PyTuple_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a tuple..." << std::endl;
		return 0;
	}

	int first_list_len = PyTuple_Size(py_ptr);
	if (first_list_len != 2)
	{
		return 0;
	}

	PyObject *ptr_lo_list = PyTuple_GetItem(py_ptr,0);
	if (PyList_Check(ptr_lo_list) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}
		
	PyObject *ptr_str_list = PyTuple_GetItem(py_ptr,1);
	if (PyList_Check(ptr_str_list) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}
		
	boost::python::list the_long_list = extract<boost::python::list>(ptr_lo_list);
	boost::python::list the_string_list = extract<boost::python::list>(ptr_str_list);
	int long_list_len = PyList_Size(ptr_lo_list);
	int string_list_len = PyList_Size(ptr_str_list);

			
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarLongStringArray *dvlsa_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvlsa);
		
	dvlsa_ptr->lvalue.length(long_list_len);
	for (int l = 0;l < long_list_len;l++)
	{
		extract<long> s_val(the_long_list[l]);
	  	(*dvlsa_ptr).lvalue[l] = s_val();
	}
				
	dvlsa_ptr->svalue.length(string_list_len);
	for (int l = 0;l < string_list_len;l++)
	{
		extract<std::string> s_val(the_string_list[l]);
	  	(*dvlsa_ptr).svalue[l] = s_val().c_str();
	}
	
	return (void *)(dvlsa_ptr);
}



//--------------------------------------------------------------------------------------
//
//		For DevVarDoubleStringArray
//
//--------------------------------------------------------------------------------------

	
void *DevVarDoubleStringArray_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//	cout << "In the DevVarDoubleStringArray from_python lvalue converter" << endl;

//
// Check that I have received a list with two elements whihc are also list
//

	if (PyTuple_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int first_list_len = PyTuple_Size(py_ptr);
	if (first_list_len != 2)
	{
		return 0;
	}

	PyObject *ptr_db_list = PyTuple_GetItem(py_ptr,0);
	if (PyList_Check(ptr_db_list) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}
		
	PyObject *ptr_str_list = PyTuple_GetItem(py_ptr,1);
	if (PyList_Check(ptr_str_list) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}
		
	boost::python::list the_double_list = extract<boost::python::list>(ptr_db_list);
	boost::python::list the_string_list = extract<boost::python::list>(ptr_str_list);
	int double_list_len = PyList_Size(ptr_db_list);
	int string_list_len = PyList_Size(ptr_str_list);

			
//
// TAKE CARE, the following data is per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevVarDoubleStringArray *dvdsa_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_dvdsa);
		
	dvdsa_ptr->dvalue.length(double_list_len);
	for (int l = 0;l < double_list_len;l++)
	{
		extract<double> s_val(the_double_list[l]);
	  	(*dvdsa_ptr).dvalue[l] = s_val();
	}
				
	dvdsa_ptr->svalue.length(string_list_len);
	for (int l = 0;l < string_list_len;l++)
	{
		extract<std::string> s_val(the_string_list[l]);
	  	(*dvdsa_ptr).svalue[l] = s_val().c_str();
	}
	
	return (void *)(dvdsa_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For C++ string
//
//--------------------------------------------------------------------------------------
	
void *std_string_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//
// Check that I have received a Python string
//

	if (PyString_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a python string..." << std::endl;
		return 0;
	}
		
				
//
// TAKE CARE, the following static data MUST be a per-thread data
//

//	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
//	string *str_ptr = &((static_cast<PyData *>(tmp_py_data))->PerTh_string);
			
	static std::string str;
	static std::string str2;
	static std::string str3;
	static std::string str4;

	static long nb_str = 1;
	void *returned_ptr = NULL;

	char *tmp_ptr = PyString_AsString(py_ptr);
//	(*str_ptr) = tmp_ptr;
	switch (nb_str)
	{
	case 1:
		str = tmp_ptr;
		nb_str = 2;
		returned_ptr = (void *)(&str);
		break;
		
	case 2:
		str2 = tmp_ptr;
		nb_str = 3;
		returned_ptr = (void *)(&str2);
		break;
		
	case 3:
		str3 = tmp_ptr;
		nb_str = 4;
		returned_ptr = (void *)(&str3);
		break;
		
	case 4:
		str4 = tmp_ptr;
		nb_str = 1;
		returned_ptr = (void *)(&str4);
		break;
	}
	
	return returned_ptr;

}

//--------------------------------------------------------------------------------------
//
//		For C++ strings vector
//
//--------------------------------------------------------------------------------------

void *std_vec_string_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
//	cout << "From_Python list to vector<string>: I have received a list with " << list_len << " element(s)" << endl;
		
//
// TAKE CARE, the following static data MUST be a per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	vector<string> *vec_str = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_vec_str);		
		
	vec_str->resize(list_len);
	int l;		
	for (l = 0;l < list_len;l++)
	{
		extract<std::string> s_val(the_list[l]);
		if (s_val.check() == false)
		{
//			std::cout << "From_Python list to vector<string>: Gasp, the PyObject I received is not a string..." << std::endl;
			return 0;
		}
		else
			vec_str->push_back(s_val);
	}
	
	return (void *)(vec_str);
}


//--------------------------------------------------------------------------------------
//
//		For C++ double vector
//
//--------------------------------------------------------------------------------------

void *std_vec_db_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{

//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	int list_len = PyList_Size(py_ptr);
	boost::python::list the_list = extract<boost::python::list>(py_ptr);
//	cout << "From_Python list to vector<double>: I have received a list with " << list_len << " element(s)" << endl;
		
				
//
// TAKE CARE, the following static data MUST be a per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	vector<double> *vec_db = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_vec_db);		
		
	vec_db->resize(list_len);
	int l;		
	for (l = 0;l < list_len;l++)
	{
		extract<double> s_val(the_list[l]);
		if (s_val.check() == false)
		{
//			std::cout << "From_Python list to vector<double>: Gasp, the PyObject I received is not a number..." << std::endl;
			return 0;
		}
		else
			vec_db->push_back(s_val);
	}
	
	return (void *)(vec_db);
}


//--------------------------------------------------------------------------------------
//
//		For DevErrorList
//
//--------------------------------------------------------------------------------------

void *DevErrorList_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{

//
// Check that I have received a tuple
//

	if (PyTuple_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a tuple..." << std::endl;
		return 0;
	}

//	int tuple_len = PyTuple_Size(py_ptr);
	boost::python::tuple the_tuple = extract<boost::python::tuple>(py_ptr);
//	cout << "From_Python tuple to DevErrorList: I have received a tuple with " << tuple_len << " element(s)" << endl;
		
				
//
// TAKE CARE, the following static data MUST be a per-thread data
//

	omni_thread::value_t *tmp_py_data = omni_thread::self()->get_value(key_py_data);
	Tango::DevErrorList *del_ptr = &((static_cast<Tango::PyData *>(tmp_py_data))->PerTh_del);		

	PyDF_2_TgDF(py_ptr,*del_ptr);
		
	return (void *)(del_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For C const char *
//
//--------------------------------------------------------------------------------------

	
void *const_char_ptr_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{

//
// Check that I have received a Python string
//

	if (PyString_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a python string..." << std::endl;
		return 0;
	}
		
				
//
// TAKE CARE, the following static data MUST be a per-thread data
//

	static const char *_ptr = PyString_AsString(py_ptr);
		
	return (void *)(&_ptr);
}


//--------------------------------------------------------------------------------------
//
//		For array of C char *
//
//--------------------------------------------------------------------------------------
	
void *Ch_ptr_from_python::convert_from_python_lvalue(PyObject *py_ptr)
{
//
// Check that I have received a list
//

	if (PyList_Check(py_ptr) == false)
	{
//		std::cout << "Gasp, the PyObject I received is not a list..." << std::endl;
		return 0;
	}

	boost::python::list the_list = extract<boost::python::list>(py_ptr);
	short list_len = extract<short>(the_list.attr("__len__")());
		
//	std::cout << "I have received a list with " << list_len << " element(s)" << std::endl;
	
//
// TAKE CARE, the following static data MUST be a per-thread data
//

	static char **msa;
	msa = new char *[list_len + 1];

	int l;		
	for (l = 0;l < list_len;l++)
	{
		extract<std::string> s_val(the_list[l]);
		msa[l] = new char[128];
		strcpy(msa[l],s_val().c_str());
	}
	msa[l] = NULL;			

	return (void *)(msa);
}

