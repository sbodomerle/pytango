#include <iostream>
#include <string>
#include <boost/python.hpp>
#include <tango.h>

using namespace boost::python;

void export_time_val()
{
    class_<Tango::TimeVal>("TimeVal")
        .def_readonly("tv_sec", &Tango::TimeVal::tv_sec)
        .def_readonly("tv_usec", &Tango::TimeVal::tv_usec)
        .def_readonly("tv_nsec", &Tango::TimeVal::tv_nsec)
    ;
}
