#include <string>
#include <vector>
#include <boost/python.hpp>

template<class T>
void get_scalar_value(T &, std::string , boost::python::object );

template<class T>
void get_array_value(std::vector<T> &, std::string , boost::python::object );

template<class T>
boost::python::list get_list_from_vector(std::vector<T> &);

template<class T>
boost::python::list get_list_from_sequence(T *);

template<class T>
boost::python::list get_list_from_const_sequence(const T *) ;

template<class T>
void get_vector_from_list(std::vector<T> &, boost::python::list &, int );

template<class T>
void get_vector_from_list(std::vector<T> &, boost::python::list &);
