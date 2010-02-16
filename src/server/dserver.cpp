#include <boost/python.hpp>
#include <tango.h>

using namespace boost::python;

void export_dserver()
{
    class_<Tango::DServer,
        bases<Tango::Device_4Impl>, boost::noncopyable>
        ("DServer", no_init)
        .def("get_process_name", &Tango::DServer::get_process_name,
            return_value_policy<copy_non_const_reference>())
        .def("get_personal_name", &Tango::DServer::get_personal_name,
            return_value_policy<copy_non_const_reference>())
        .def("get_instance_name", &Tango::DServer::get_instance_name,
            return_value_policy<copy_non_const_reference>())
        .def("get_full_name", &Tango::DServer::get_full_name,
            return_value_policy<copy_non_const_reference>())
    ;
}
