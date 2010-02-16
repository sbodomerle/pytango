
#include <tango.h>
#include "pytgutils.h"

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/copy_non_const_reference.hpp>

#include "callback.h"



namespace PyConnection
{

    using namespace boost::python;

    static
    Tango::DeviceData command_inout(Tango::Connection& self, const string & cmd_name, const Tango::DeviceData &argin)
    {
        AutoPythonAllowThreads guard;
        return self.command_inout(const_cast<string&>(cmd_name), const_cast<Tango::DeviceData&>(argin));
    }


    static
    long command_inout_asynch_id(Tango::Connection& self, const string &cmd_name, const Tango::DeviceData &argin, bool forget)
    {
        AutoPythonAllowThreads guard;
        return self.command_inout_asynch(const_cast<string&>(cmd_name), const_cast<Tango::DeviceData&>(argin), forget);
    }


    static
    Tango::DeviceData command_inout_reply(Tango::Connection& self, long id)
    {
        AutoPythonAllowThreads guard;
        return self.command_inout_reply(id);
    }

    static
    Tango::DeviceData command_inout_reply(Tango::Connection& self, long id, long timeout)
    {
        AutoPythonAllowThreads guard;
        return self.command_inout_reply(id, timeout);
    }

    static
    void command_inout_asynch_cb(object py_self, const string & cmd_name, const Tango::DeviceData &argin, object py_cb)
    {
        Tango::Connection* self = extract<Tango::Connection*>(py_self);
        PyCallBackAutoDie* cb = extract<PyCallBackAutoDie*>(py_cb);
        cb->set_autokill_references(py_cb, py_self);

        try {
            AutoPythonAllowThreads guard;
            self->command_inout_asynch(const_cast<string&>(cmd_name), const_cast<Tango::DeviceData&>(argin), *cb);
        } catch (...) {
            cb->unset_autokill_references();
            throw;
        }
    }

    static
    void get_asynch_replies(Tango::Connection& self)
    {
        AutoPythonAllowThreads guard;
        self.get_asynch_replies();
    }

    static
    void get_asynch_replies(Tango::Connection& self, long call_timeout)
    {
        AutoPythonAllowThreads guard;
        self.get_asynch_replies(call_timeout);
    }
}

void export_connection()
{
    using namespace boost::python;

    class_<Tango::Connection, boost::noncopyable> Connection("Connection", no_init)
    ;

    Connection
        .def("dev_name", pure_virtual(&Tango::Connection::dev_name))

        .def("get_db_host", &Tango::Connection::get_db_host,
            ( arg_("self") ),
            return_value_policy<copy_non_const_reference>())

        .def("get_db_port", &Tango::Connection::get_db_port,
            ( arg_("self") ),
            return_value_policy<copy_non_const_reference>())

        .def("get_db_port_num", &Tango::Connection::get_db_port_num,
            ( arg_("self") ))

        .def("get_from_env_var", &Tango::Connection::get_from_env_var,
            ( arg_("self") ))

        .def("connect", &Tango::Connection::connect,
            ( arg_("self"), arg_("corba_name") ))

        .def("reconnect", &Tango::Connection::reconnect,
            ( arg_("self"), arg_("db_used") ))

        .def("get_idl_version", &Tango::Connection::get_idl_version,
            ( arg_("self") ))

        .def("set_timeout_millis", &Tango::Connection::set_timeout_millis,
            ( arg_("self"), arg_("timeout") ))

        .def("get_timeout_millis", &Tango::Connection::get_timeout_millis,
            ( arg_("self") ))

        .def("get_source", &Tango::Connection::get_source,
            ( arg_("self") ))

        .def("set_source", &Tango::Connection::set_source,
            ( arg_("self"), arg_("source") ))

        .def("get_transparency_reconnection",
            &Tango::Connection::get_transparency_reconnection,
            ( arg_("self") ))

        .def("set_transparency_reconnection",
            &Tango::Connection::set_transparency_reconnection,
            ( arg_("self"), arg_("yesno") ))

        .def("__command_inout", &PyConnection::command_inout)
        .def("__command_inout_asynch_id", &PyConnection::command_inout_asynch_id)
        .def("__command_inout_asynch_cb", &PyConnection::command_inout_asynch_cb)

        .def("command_inout_reply_raw",
            (Tango::DeviceData (*)(Tango::Connection&, long))
            &PyConnection::command_inout_reply,
            ( arg_("self"), arg_("id") ))

        .def("command_inout_reply_raw",
            (Tango::DeviceData (*)(Tango::Connection&, long, long))
            &PyConnection::command_inout_reply,
            ( arg_("self"), arg_("id"), arg_("timeout") ))


        //
        // Asynchronous methods
        //

        .def("get_asynch_replies",
            (void (*) (Tango::Connection&))
            &PyConnection::get_asynch_replies,
            ( arg_("self") ))

        .def("get_asynch_replies",
            (void (*) (Tango::Connection&,long))
            &PyConnection::get_asynch_replies,
            ( arg_("self"), arg_("call_timeout") ))

        .def("cancel_asynch_request",
            &Tango::Connection::cancel_asynch_request,
            ( arg_("self"), arg_("id") ))

        .def("cancel_all_polling_asynch_request",
            &Tango::Connection::cancel_all_polling_asynch_request,
            ( arg_("self") ))

        //
        // Control access related methods
        //
        .def("get_access_control", &Tango::Connection::get_access_control,
            ( arg_("self") ))

        .def("set_access_control", &Tango::Connection::set_access_control,
            ( arg_("self"), arg_("acc") ))
    ;
}
