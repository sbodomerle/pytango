#include <string>
#include <vector>
#include <boost/python.hpp>
#include <istream>

#include <iostream>
#include <boost/python/type_id.hpp>

using namespace std;

template<class T>
void get_scalar_value(T &val,std::string typ,boost::python::object pyData) 
{
	boost::python::extract<T> s_val(pyData);
	if (s_val.check()) 
		val = s_val();
	else 
	{
		std::string type = boost::python::extract<std::string>(pyData.attr("__class__").attr("__name__"));
		if ((type == "int") || (type == "long") || (type == "float") || (type == "str"))
		{
			std::string temp = boost::python::extract<std::string>(pyData.attr("__str__")());
			std::istringstream str(temp);
			str >> val;
		}
		else
		{
			std::ostringstream ostream;
			std::string stpy = boost::python::extract<std::string>(pyData.attr("__str__")());
			ostream << "TypeError : expected a " << typ <<" instead of " << type << ends;
			PyErr_SetString(PyExc_TypeError, ostream.str().data());
			boost::python::throw_error_already_set();
		}
	}
}

template<class T>
void get_array_value(std::vector<T> &val_array,std::string typ,boost::python::object pyData)
{
	int arr_len = boost::python::extract<int>(pyData.attr("__len__")()) ;
	T i_val;
	for (int i = 0 ; i < arr_len; i++ )
	{
		boost::python::extract<T> s_val(pyData[i]);
		if (s_val.check()) 
			i_val = s_val();
		else
		{
			std::string type = boost::python::extract<std::string>(pyData[i].attr("__class__").attr("__name__"));
			if ((type == "int") || (type == "long") || (type == "float") || (type == "str"))
			{
				std::string temp = boost::python::extract<std::string>(pyData[i].attr("__str__")());
				std::istringstream str(temp);
				str >> i_val;
			}
			else
			{
				std::ostringstream ostream;
				std::string stpy = boost::python::extract<std::string>(pyData[i].attr("__str__")());
				ostream << "TypeError : expected a " << typ <<" instead of " << type << ends;
				PyErr_SetString(PyExc_TypeError, ostream.str().data());
				boost::python::throw_error_already_set();
			}
		}
		val_array.push_back(i_val);
	}
}


template<class T>
boost::python::list get_list_from_vector(std::vector<T> &val_array) 
{
    	boost::python::list py_list;
    	typename std::vector<T>::iterator i;
    	T temp;
    	for (i = val_array.begin() ; i < val_array.end() ; i++ )
    	{
       		temp = *i;
       		py_list.append(temp);
    	}
    	return py_list;
}


template<class T>
boost::python::list get_list_from_sequence(T *seq) 
{
    	boost::python::list py_list;
	if (seq != NULL)
	{
    		unsigned long nb_data = seq->length();
    		for (unsigned long i = 0 ; i < nb_data ; i++ )
    		{
       			py_list.append((*seq)[i]);
		}
    	}
    	return py_list;
}

template<class T>
boost::python::list get_list_from_const_sequence(const T *seq) 
{
    	boost::python::list py_list;
    	unsigned long nb_data = seq->length();
    	for (unsigned long i = 0 ; i < nb_data ; i++ )
    	{
       		py_list.append((*seq)[i]);
    	}
    	return py_list;
}

template<class T>
void get_vector_from_list(std::vector<T> &val_array, boost::python::list &pylist) 
{
	int arr_len = boost::python::extract<int>(pylist.attr("__len__")());
//	std::vector<T> val_array;
	T temp;
	for (int i = 0 ; i < arr_len; i++)
	{
		boost::python::extract<T> s_val(pylist[i]);
		if (s_val.check()) 
			temp = s_val();
		val_array.push_back(temp);
	}
}
