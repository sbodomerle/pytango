# ------------------------------------------------------------------------------
# This file is part of PyTango (http://www.tinyurl.com/PyTango)
#
# Copyright 2006-2012 CELLS / ALBA Synchrotron, Bellaterra, Spain
# Copyright 2013-2014 European Synchrotron Radiation Facility, Grenoble, France
#
# Distributed under the terms of the GNU Lesser General Public License,
# either version 3 of the License, or (at your option) any later version.
# See LICENSE.txt for more info.
# ------------------------------------------------------------------------------

"""
This is the main PyTango package file.
Documentation for this package can be found online:

http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html
"""

from __future__ import print_function

__all__ = [ 'AccessControlType', 'ApiUtil', 'ArchiveEventInfo',
'ArchiveEventProp', 'ArgType', 'AsynCall', 'AsynReplyNotArrived', 'AttReqType',
'Attr', 'AttrConfEventData', 'AttrData', 'AttrDataFormat', 'AttrList',
'AttrProperty', 'AttrQuality', 'AttrReadEvent', 'AttrSerialModel',
'AttrWriteType', 'AttrWrittenEvent', 'Attribute', 'AttributeAlarm',
'AttributeAlarmInfo', 'AttributeConfig', 'AttributeConfig_2',
'AttributeConfig_3', 'AttributeDimension', 'AttributeEventInfo',
'AttributeInfo', 'AttributeInfoEx', 'AttributeInfoList', 'AttributeInfoListEx',
'AttributeList', 'AttributeProxy', 'ChangeEventInfo', 'ChangeEventProp',
'CmdArgType', 'CmdDoneEvent', 'CommandInfo', 'CommandInfoList',
'CommunicationFailed', 'Connection', 'ConnectionFailed',
'ConstDevString', 'DServer', 'DataReadyEventData', 'Database', 'DbData',
'DbDatum', 'DbDevExportInfo', 'DbDevExportInfos', 'DbDevImportInfo',
'DbDevImportInfos', 'DbDevFullInfo', 'DbDevInfo', 'DbDevInfos', 'DbHistory',
'DbHistoryList', 'DbServerInfo', 'DbServerData', 'DebugIt', 'DevBoolean', 'DevCommandInfo',
'DevDouble', 'DevEncoded', 'DevError', 'DevFailed', 'DevFloat', 'DevInt',
'DevLong', 'DevLong64', 'DevShort', 'DevSource', 'DevState', 'DevString',
'DevUChar', 'DevULong', 'DevULong64', 'DevUShort', 'DevVarBooleanArray',
'DevVarCharArray', 'DevVarDoubleArray', 'DevVarDoubleStringArray',
'DevVarFloatArray', 'DevVarLong64Array', 'DevVarLongArray',
'DevVarLongStringArray', 'DevVarShortArray', 'DevVarStringArray',
'DevVarULong64Array', 'DevVarULongArray', 'DevVarUShortArray',
'DevVoid', 'DeviceAttribute', 'DeviceAttributeConfig',
'DeviceAttributeHistory', 'DeviceClass', 'DeviceData', 'DeviceDataList',
'DeviceDataHistory', 'DeviceDataHistoryList',
'DeviceImpl', 'DeviceInfo', 'DeviceProxy',
'DeviceUnlocked', 'Device_2Impl', 'Device_3Impl', 'Device_4Impl',
'DispLevel', 'EncodedAttribute', 'ErrSeverity', 'ErrorIt',
'EventData', 'EventProperties', 'EventSystemFailed', 'EventType',
'Except', 'ExtractAs', 'FMT_UNKNOWN', 'FatalIt', 'Group', 'GroupAttrReply',
'GroupAttrReplyList', 'GroupCmdReply', 'GroupCmdReplyList',
'GroupReply', 'GroupReplyList', 'IMAGE', 'ImageAttr', 'InfoIt',
'KeepAliveCmdCode', 'Level', 'LockCmdCode', 'LockerInfo', 'LockerLanguage',
'LogIt', 'LogLevel', 'LogTarget', 'Logger', 'Logging', 'MessBoxType',
'MultiAttribute', 'MultiAttrProp', 'MultiClassAttribute', 'NamedDevFailed',
'NamedDevFailedList', 'NonDbDevice', 'NonSupportedFeature',
'NotAllowed', 'NumpyType', 'PeriodicEventInfo', 'PeriodicEventProp',
'PollCmdCode', 'PollDevice',
'PollObjType', 'READ', 'READ_WITH_WRITE', 'READ_WRITE', 'Release', 'SCALAR',
'SPECTRUM', 'SerialModel', 'SpectrumAttr', 'StdDoubleVector',
'StdGroupAttrReplyVector', 'StdGroupCmdReplyVector', 'StdGroupReplyVector',
'StdLongVector', 'StdNamedDevFailedVector', 'StdStringVector', 'SubDevDiag',
'TangoStream', 'TimeVal', 'UserDefaultAttrProp', 'Util', 'WAttribute',
'WRITE', 'WarnIt', 'WrongData', 'WrongNameSyntax', '__version__',
'__version_description__', '__version_info__', '__version_long__',
'__version_number__', 'alarm_flags', 'asyn_req_type', 'cb_sub_model',
'class_factory', 'class_list', 'constants', 'constructed_class',
'cpp_class_list', 'delete_class_list', 'get_class', 'get_classes',
'get_constructed_class', 'get_constructed_classes', 'get_cpp_class',
'get_cpp_classes', 'is_array_type', 'is_float_type',
'is_int_type', 'is_numerical_type', 'is_scalar_type', 'numpy_image',
'numpy_spectrum', 'numpy_type', 'obj_2_str', 'raise_asynch_exception',
'seqStr_2_obj']

__docformat__ = "restructuredtext"

import os
import sys

def __prepare_nt():
    PATH = os.environ['PATH']
    tango_root = os.environ.get("TANGO_ROOT")
    if tango_root is None:
        tango_root = os.path.join(os.environ["ProgramFiles"], "tango")
    if sys.hexversion < 0x03030000:
        vc = "vc9_dll"
    else:
        vc = "vc10_dll"
    tango_dll_path = os.path.join(tango_root,"win32","lib",vc)
    if tango_dll_path.lower() not in PATH.lower():
        os.environ['PATH'] += ";" + tango_dll_path

try:
    from ._PyTango import DeviceProxy
except ImportError as ie:
    if ie.args[0].count("_PyTango"):
        print(80*"-")
        print(ie)
        print(80*"-")
        print("Probably your current directory is the PyTango's source installation directory.")
        print("You must leave this directory first before using PyTango, otherwise the")
        print("source distribution will conflict with the installed PyTango")
        print(80*"-")
        raise
    
    # in windows try to find the location for tango
    if os.name == 'nt':
        __prepare_nt()
        from ._PyTango import DeviceProxy

from ._PyTango import (AccessControlType, ApiUtil, ArchiveEventInfo,
    AsynCall, AsynReplyNotArrived, AttReqType, Attr, AttrConfEventData,
    AttrDataFormat, AttrList, AttrProperty, AttrQuality, AttrReadEvent,
    AttrSerialModel, AttrWriteType, AttrWrittenEvent, Attribute,
    AttributeAlarmInfo, AttributeDimension, AttributeEventInfo, AttributeInfo,
    AttributeInfoEx, AttributeInfoList, AttributeInfoListEx, AttributeList,
    ChangeEventInfo, CmdArgType,
    CmdDoneEvent, CommandInfo, CommandInfoList, CommunicationFailed,
    Connection, ConnectionFailed, ConstDevString, DServer, DataReadyEventData,
    Database, DbData, DbDatum, DbDevExportInfo, DbDevExportInfos,
    DbDevImportInfo, DbDevImportInfos, DbDevFullInfo, DbDevInfo, DbDevInfos, DbHistory,
    DbHistoryList, DbServerInfo, DbServerData, DevBoolean, DevCommandInfo, DevDouble,
    DevEncoded, DevError, DevFailed, DevFloat, DevInt, DevLong, DevLong64,
    DevShort, DevSource, DevState, DevString, DevUChar, DevULong, DevULong64,
    DevUShort, DevVarBooleanArray, DevVarCharArray, DevVarDoubleArray,
    DevVarDoubleStringArray, DevVarFloatArray, DevVarLong64Array,
    DevVarLongArray, DevVarLongStringArray, DevVarShortArray, DevVarStringArray,
    DevVarULong64Array, DevVarULongArray, DevVarUShortArray, DevVoid,
    DeviceAttribute, DeviceAttributeConfig, DeviceAttributeHistory,
    DeviceData, DeviceDataList, DeviceDataHistory, DeviceDataHistoryList,
    DeviceImpl, DeviceInfo, DeviceUnlocked, Device_2Impl,
    Device_3Impl, Device_4Impl, DispLevel, EncodedAttribute, ErrSeverity,
    EventData, EventSystemFailed, EventType,
    Except, ExtractAs, GreenMode, FMT_UNKNOWN, GroupAttrReply, GroupAttrReplyList,
    GroupCmdReply, GroupCmdReplyList, GroupReply, GroupReplyList,
    IMAGE, ImageAttr, KeepAliveCmdCode, Level, LockCmdCode, LockerInfo,
    LockerLanguage, LogLevel, LogTarget, Logger, Logging, MessBoxType,
    MultiAttribute, MultiClassAttribute, NamedDevFailed, NamedDevFailedList,
    NonDbDevice, NonSupportedFeature, NotAllowed, PeriodicEventInfo,
    PollCmdCode, PollDevice, PollObjType, READ,
    READ_WITH_WRITE, READ_WRITE, SCALAR, SPECTRUM, SerialModel,
    SpectrumAttr, StdDoubleVector, StdGroupAttrReplyVector,
    StdGroupCmdReplyVector, StdGroupReplyVector, StdLongVector,
    StdNamedDevFailedVector, StdStringVector, SubDevDiag, TimeVal,
    UserDefaultAttrProp, WAttribute, WRITE, WrongData, WrongNameSyntax,
    alarm_flags, asyn_req_type, cb_sub_model, constants,
    raise_asynch_exception, Interceptors)

ArgType = CmdArgType

from .release import Release

__author__ = Release.author_lines
__version_info__ = Release.version_info
__version__ = Release.version
__version_long__ = Release.version_long
__version_number__ = Release.version_number
__version_description__ = Release.version_description
__doc__ = Release.long_description

from .attr_data import AttrData
from .log4tango import TangoStream, LogIt, DebugIt, InfoIt, WarnIt, \
    ErrorIt, FatalIt
from .device_server import ChangeEventProp, PeriodicEventProp, \
    ArchiveEventProp, AttributeAlarm, EventProperties, AttributeConfig, \
    AttributeConfig_2, AttributeConfig_3, MultiAttrProp
from .attribute_proxy import AttributeProxy, get_attribute_proxy
from .group import Group
from .pyutil import Util
from .device_class import DeviceClass
from .globals import get_class, get_classes, get_cpp_class, get_cpp_classes, \
    get_constructed_class, get_constructed_classes, class_factory, \
    delete_class_list, class_list, cpp_class_list, constructed_class
from .utils import is_scalar_type, is_array_type, is_numerical_type, \
    is_int_type, is_float_type, obj_2_str, seqStr_2_obj
from .green import set_green_mode, get_green_mode
from .device_proxy import get_device_proxy
from .tango_numpy import NumpyType, numpy_type, numpy_spectrum, numpy_image

from .pytango_init import init as __init
__init()

