################################################################################
##
## This file is part of PyTango, a python binding for Tango
## 
## http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html
##
## Copyright 2011 CELLS / ALBA Synchrotron, Bellaterra, Spain
## 
## PyTango is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## PyTango is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public License
## along with PyTango.  If not, see <http://www.gnu.org/licenses/>.
##
################################################################################

"""
This is an internal PyTango module.
"""

__all__ = []

__docformat__ = "restructuredtext"

from _PyTango import *

from device_server import AttributeAlarm, EventProperties
from device_server import ChangeEventProp, PeriodicEventProp, ArchiveEventProp
from device_server import AttributeConfig, AttributeConfig_2, AttributeConfig_3

def __inc_param(obj, name):
    ret  = not name.startswith('_')
    ret &= not name in ('except_flags',)
    ret &= not callable(getattr(obj,name))
    return ret

def __single_param(obj, param_name, f=repr, fmt='%s = %s'):
    param_value = getattr(obj, param_name)
    if param_name is 'data_type':
        param_value = CmdArgType.values.get(param_value, param_value)
    return fmt % (param_name, f(param_value))

def __struct_params_s(obj, separator=', ', f=repr, fmt='%s = %s'):
    """method wrapper for printing all elements of a struct"""
    s = separator.join([__single_param(obj, n, f, fmt) for n in dir(obj) if __inc_param(obj,n)])
    return s

def __struct_params_repr(obj):
    """method wrapper for representing all elements of a struct"""
    return __struct_params_s(obj)

def __struct_params_str(obj, fmt, f=repr):
    """method wrapper for printing all elements of a struct."""
    return __struct_params_s(obj, '\n', f=f, fmt=fmt)

def __repr__Struct(self):
    """repr method for struct"""
    return '%s(%s)' % (self.__class__.__name__, __struct_params_repr(self))

def __str__Struct_Helper(self, f=repr):
    """str method for struct"""
    attrs = [ n for n in dir(self) if __inc_param(self, n)]
    fmt = attrs and '%%%ds = %%s' % max( map(len, attrs) ) or "%s = %s"
    return '%s[\n%s]\n' % (self.__class__.__name__, __struct_params_str(self, fmt, f))

def __str__Struct(self):
    return __str__Struct_Helper(self, f=repr)

def __str__Struct_extra(self):
    return __str__Struct_Helper(self, f=str)

def __registerSeqStr():
    """helper function to make internal sequences printable"""
    _SeqStr = lambda self: (self and "[%s]" % (", ".join(map(repr,self)))) or "[]"
    _SeqRepr = lambda self: (self and "[%s]" % (", ".join(map(repr,self)))) or "[]"

    seqs = (StdStringVector, StdLongVector, CommandInfoList,
            AttributeInfoList, AttributeInfoListEx,
            DeviceDataHistoryList,
            GroupReplyList, GroupAttrReplyList, GroupCmdReplyList,
            DbData, DbDevInfos, DbDevExportInfos, DbDevImportInfos, DbHistoryList)

    for seq in seqs:
        seq.__str__ = _SeqStr
        seq.__repr__ = _SeqRepr

import operator

def __str__DevFailed(self):
    if operator.isSequenceType(self.args):
        return 'DevFailed[\n%s]' % '\n'.join(map(str,self.args))
    return 'DevFailed[%s]' % (self.args)
    
def __repr__DevFailed(self):
    return 'DevFailed(args = %s)' % repr(self.args)

def __str__DevError(self):
    desc = self.desc.replace("\n","\n           ")
    s = """DevError[
    desc = %s
  origin = %s
  reason = %s
severity = %s]\n""" % (desc, self.origin, self.reason, self.severity)
    return s

def __registerStructStr():
    """helper method to register str and repr methods for structures"""
    structs = (LockerInfo, DevCommandInfo, AttributeDimension, CommandInfo,
        DeviceInfo, DeviceAttributeConfig, AttributeInfo, AttributeAlarmInfo,
        ChangeEventInfo, PeriodicEventInfo, ArchiveEventInfo,
        AttributeEventInfo, AttributeInfoEx,
        DeviceAttribute, DeviceAttributeHistory, DeviceData, DeviceDataHistory,
        DbDatum, DbDevInfo, DbDevImportInfo, DbDevExportInfo, DbServerInfo,
        GroupElement, GroupReply, GroupAttrReply, GroupCmdReply,
        DevError, EventData, AttrConfEventData, DataReadyEventData,
        AttributeConfig, AttributeConfig_2, AttributeConfig_3,
        ChangeEventProp, PeriodicEventProp, ArchiveEventProp,
        AttributeAlarm, EventProperties)

    for struct in structs:
        struct.__str__ = __str__Struct
        struct.__repr__ = __repr__Struct

    # special case for TimeVal: it already has a str representation itself
    TimeVal.__repr__ = __repr__Struct

    # special case for DevFailed: we want a better pretty print
    # also, because it is an Exception it has the message attribute which
    # generates a Deprecation warning in python 2.6
    DevFailed.__str__ = __str__DevFailed
    DevFailed.__repr__ = __repr__DevFailed

    DevError.__str__ = __str__DevError

def init(doc=True):
    __registerSeqStr()
    __registerStructStr()
