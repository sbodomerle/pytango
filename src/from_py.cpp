#include "from_py.h"

using namespace boost::python;

void convert2array(const object &py_value, StdStringVector & result)
{
    PyObject *py_value_ptr = py_value.ptr();
    if(PySequence_Check(py_value_ptr) == 0)
    {
        raise_(PyExc_TypeError, param_must_be_seq);
    }
    
    if (PyString_Check(py_value_ptr))
    {
        result.push_back(PyString_AsString(py_value_ptr));
    }
    else
    {
        size_t size = boost::python::len(py_value);
        result.reserve(size);
        
        for (size_t i=0; i < size; ++i) {
            char *vi = boost::python::extract<char*>(py_value[i]);
            result.push_back(vi);
        }
    }
}

void convert2array(const object &py_value, Tango::DevVarStringArray & result)
{
    PyObject *py_value_ptr = py_value.ptr();
    if(PySequence_Check(py_value_ptr) == 0)
    {
        raise_(PyExc_TypeError, param_must_be_seq);
    }
    
    if (PyString_Check(py_value_ptr))
    {
        result.length(1);
        result[0] = CORBA::string_dup(PyString_AsString(py_value_ptr));
    }
    else
    {
        size_t size = boost::python::len(py_value);
        result.length(size);
        for (size_t i=0; i < size; ++i) {
            result[i] = CORBA::string_dup(boost::python::extract<char*>(py_value[i]));
        }
    }
}

void from_py_object(object &py_obj, Tango::AttributeAlarm &attr_alarm)
{
    attr_alarm.min_alarm = extract<const char *>(py_obj.attr("min_alarm"));
    attr_alarm.max_alarm = extract<const char *>(py_obj.attr("max_alarm"));
    attr_alarm.min_warning = extract<const char *>(py_obj.attr("min_warning"));
    attr_alarm.max_warning = extract<const char *>(py_obj.attr("max_warning"));
    attr_alarm.delta_t = extract<const char *>(py_obj.attr("delta_t"));
    attr_alarm.delta_val = extract<const char *>(py_obj.attr("delta_val"));
    convert2array(py_obj.attr("extensions"), attr_alarm.extensions);
}

void from_py_object(object &py_obj, Tango::ChangeEventProp &change_evt_prop)
{
    change_evt_prop.rel_change = extract<const char *>(py_obj.attr("rel_change"));
    change_evt_prop.abs_change = extract<const char *>(py_obj.attr("abs_change"));
    convert2array(py_obj.attr("extensions"), change_evt_prop.extensions);
}

void from_py_object(object &py_obj, Tango::PeriodicEventProp &periodic_evt_prop)
{
    periodic_evt_prop.period = extract<const char *>(py_obj.attr("period"));
    convert2array(py_obj.attr("extensions"), periodic_evt_prop.extensions);
}

void from_py_object(object &py_obj, Tango::ArchiveEventProp &archive_evt_prop)
{
    archive_evt_prop.rel_change = extract<const char *>(py_obj.attr("rel_change"));
    archive_evt_prop.abs_change = extract<const char *>(py_obj.attr("abs_change"));
    archive_evt_prop.period = extract<const char *>(py_obj.attr("period"));
    convert2array(py_obj.attr("extensions"), archive_evt_prop.extensions);
}

void from_py_object(object &py_obj, Tango::EventProperties &evt_props)
{
    object py_ch_event = py_obj.attr("ch_event");
    object py_per_event = py_obj.attr("per_event");
    object py_arch_event = py_obj.attr("arch_event");
        
    from_py_object(py_ch_event, evt_props.ch_event);
    from_py_object(py_per_event, evt_props.per_event);
    from_py_object(py_arch_event, evt_props.arch_event);
}


void from_py_object(object &py_obj, Tango::AttributeConfig &attr_conf)
{
    attr_conf.name = extract<const char *>(py_obj.attr("name"));
    attr_conf.writable = extract<Tango::AttrWriteType>(py_obj.attr("writable"));
    attr_conf.data_format = extract<Tango::AttrDataFormat>(py_obj.attr("data_format"));
    attr_conf.data_type = extract<CORBA::Long>(py_obj.attr("data_type"));
    attr_conf.max_dim_x = extract<CORBA::Long>(py_obj.attr("max_dim_x"));
    attr_conf.max_dim_y = extract<CORBA::Long>(py_obj.attr("max_dim_y"));
    attr_conf.description = extract<const char *>(py_obj.attr("description"));
    attr_conf.label = extract<const char *>(py_obj.attr("label"));
    attr_conf.unit = extract<const char *>(py_obj.attr("unit"));
    attr_conf.standard_unit = extract<const char *>(py_obj.attr("standard_unit"));
    attr_conf.display_unit = extract<const char *>(py_obj.attr("display_unit"));
    attr_conf.format = extract<const char *>(py_obj.attr("format"));
    attr_conf.min_value = extract<const char *>(py_obj.attr("min_value"));
    attr_conf.max_value = extract<const char *>(py_obj.attr("max_value"));
    attr_conf.min_alarm = extract<const char *>(py_obj.attr("min_alarm"));
    attr_conf.max_alarm = extract<const char *>(py_obj.attr("max_alarm"));
    attr_conf.writable_attr_name = extract<const char *>(py_obj.attr("writable_attr_name"));
    convert2array(py_obj.attr("extensions"), attr_conf.extensions);
}

void from_py_object(object &py_obj, Tango::AttributeConfig_2 &attr_conf)
{
    attr_conf.name = extract<const char *>(py_obj.attr("name"));
    attr_conf.writable = extract<Tango::AttrWriteType>(py_obj.attr("writable"));
    attr_conf.data_format = extract<Tango::AttrDataFormat>(py_obj.attr("data_format"));
    attr_conf.data_type = extract<CORBA::Long>(py_obj.attr("data_type"));
    attr_conf.max_dim_x = extract<CORBA::Long>(py_obj.attr("max_dim_x"));
    attr_conf.max_dim_y = extract<CORBA::Long>(py_obj.attr("max_dim_y"));
    attr_conf.description = extract<const char *>(py_obj.attr("description"));
    attr_conf.label = extract<const char *>(py_obj.attr("label"));
    attr_conf.unit = extract<const char *>(py_obj.attr("unit"));
    attr_conf.standard_unit = extract<const char *>(py_obj.attr("standard_unit"));
    attr_conf.display_unit = extract<const char *>(py_obj.attr("display_unit"));
    attr_conf.format = extract<const char *>(py_obj.attr("format"));
    attr_conf.min_value = extract<const char *>(py_obj.attr("min_value"));
    attr_conf.max_value = extract<const char *>(py_obj.attr("max_value"));
    attr_conf.min_alarm = extract<const char *>(py_obj.attr("min_alarm"));
    attr_conf.max_alarm = extract<const char *>(py_obj.attr("max_alarm"));
    attr_conf.writable_attr_name = extract<const char *>(py_obj.attr("writable_attr_name"));
    attr_conf.level = extract<Tango::DispLevel>(py_obj.attr("level"));
    convert2array(py_obj.attr("extensions"), attr_conf.extensions);
}

void from_py_object(object &py_obj, Tango::AttributeConfig_3 &attr_conf)
{
    attr_conf.name = extract<const char *>(py_obj.attr("name"));
    attr_conf.writable = extract<Tango::AttrWriteType>(py_obj.attr("writable"));
    attr_conf.data_format = extract<Tango::AttrDataFormat>(py_obj.attr("data_format"));
    attr_conf.data_type = extract<CORBA::Long>(py_obj.attr("data_type"));
    attr_conf.max_dim_x = extract<CORBA::Long>(py_obj.attr("max_dim_x"));
    attr_conf.max_dim_y = extract<CORBA::Long>(py_obj.attr("max_dim_y"));
    attr_conf.description = extract<const char *>(py_obj.attr("description"));
    attr_conf.label = extract<const char *>(py_obj.attr("label"));
    attr_conf.unit = extract<const char *>(py_obj.attr("unit"));
    attr_conf.standard_unit = extract<const char *>(py_obj.attr("standard_unit"));
    attr_conf.display_unit = extract<const char *>(py_obj.attr("display_unit"));
    attr_conf.format = extract<const char *>(py_obj.attr("format"));
    attr_conf.min_value = extract<const char *>(py_obj.attr("min_value"));
    attr_conf.max_value = extract<const char *>(py_obj.attr("max_value"));
    attr_conf.writable_attr_name = extract<const char *>(py_obj.attr("writable_attr_name"));
    attr_conf.level = extract<Tango::DispLevel>(py_obj.attr("level"));
    
    object py_att_alarm = py_obj.attr("att_alarm");
    object py_event_prop = py_obj.attr("event_prop");
        
    from_py_object(py_att_alarm, attr_conf.att_alarm);
    from_py_object(py_event_prop, attr_conf.event_prop);
    convert2array(py_obj.attr("extensions"), attr_conf.extensions);
    convert2array(py_obj.attr("sys_extensions"), attr_conf.sys_extensions);
}


void from_py_object(object &py_obj, Tango::AttributeConfigList &attr_conf_list)
{
    PyObject* py_obj_ptr = py_obj.ptr();
    
    if (!PySequence_Check(py_obj_ptr))
    {
        attr_conf_list.length(1);
        from_py_object(py_obj, attr_conf_list[0]);
        return;
    }
    
    size_t size = boost::python::len(py_obj);
    attr_conf_list.length(size);
    for (size_t i=0; i < size; ++i) {
        object tmp = py_obj[i];
        from_py_object(tmp, attr_conf_list[i]);
    }
}

void from_py_object(object &py_obj, Tango::AttributeConfigList_2 &attr_conf_list)
{
    PyObject* py_obj_ptr = py_obj.ptr();
    
    if (!PySequence_Check(py_obj_ptr))
    {
        attr_conf_list.length(1);
        from_py_object(py_obj, attr_conf_list[0]);
        return;
    }
    
    size_t size = boost::python::len(py_obj);
    attr_conf_list.length(size);
    for (size_t i=0; i < size; ++i) {
        object tmp = py_obj[i];
        from_py_object(tmp, attr_conf_list[i]);
    }
}

void from_py_object(object &py_obj, Tango::AttributeConfigList_3 &attr_conf_list)
{
    PyObject* py_obj_ptr = py_obj.ptr();
    
    if (!PySequence_Check(py_obj_ptr))
    {
        attr_conf_list.length(1);
        from_py_object(py_obj, attr_conf_list[0]);
        return;
    }
    
    size_t size = boost::python::len(py_obj);
    attr_conf_list.length(size);
    for (size_t i=0; i < size; ++i) {
        object tmp = py_obj[i];
        from_py_object(tmp, attr_conf_list[i]);
    }
}
