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

from utils import document_method as __document_method
from _PyTango import GroupReply
from _PyTango import GroupCmdReply
from _PyTango import GroupAttrReply
from _PyTango import ExtractAs

def __GroupCmdReply__get_data(self):
    return self.get_data_raw().extract()

def __GroupAttrReply__get_data(self, extract_as=ExtractAs.Numpy):
    # GroupAttrReply.__get_data() extracts the data from the object, so
    # two successive calls to get_data() result in the second one returning
    # an empty value, which is an unexpected behaviour.
    # That's why we cache the result of the first call.
    try:
        data, orig_extract_as = self.__dataCache
    except AttributeError:
        data = self.__get_data(extract_as)
        self.__dataCache = data, extract_as
        return data
    
    if extract_as != orig_extract_as:
        raise Exception("Successive calls to get_data() must receive the same"
                        " parameters as the first one.")
    return data

def __init_GroupReply():
    GroupCmdReply.get_data = __GroupCmdReply__get_data
    GroupAttrReply.get_data = __GroupAttrReply__get_data

def __doc_GroupReply():
    def document_method(method_name, desc, append=True):
        return __document_method(GroupReply, method_name, desc, append)

    GroupReply.__doc__ = """
        This is the base class for the result of an operation on a
        PyTangoGroup, being it a write attribute, read attribute, or
        command inout operation.

        It has some trivial common operations:

            - has_failed(self) -> bool
            - group_element_enabled(self) ->bool
            - dev_name(self) -> str
            - obj_name(self) -> str
            - get_err_stack(self) -> DevErrorList
    """

    __document_method(GroupCmdReply, "get_data", """
    get_data(self) -> any

            Get the actual value stored in the GroupCmdRply, the command
            output value.
            It's the same as self.get_data_raw().extract()

        Parameters : None
        Return     : (any) Whatever is stored there, or None.
    """ )

    __document_method(GroupCmdReply, "get_data_raw", """
    get_data_raw(self) -> any

            Get the DeviceData containing the output parameter
            of the command.

        Parameters : None
        Return     : (DeviceData) Whatever is stored there, or None.
    """ )

    __document_method(GroupAttrReply, "get_data", """
    get_data(self, extract_as=ExtractAs.Numpy) -> DeviceAttribute

            Get the DeviceAttribute.

        Parameters :
            - extract_as : (ExtractAs)

        Return     : (DeviceAttribute) Whatever is stored there, or None.
    """ )

def init(doc=True):
    __init_GroupReply()
    if doc:
        __doc_GroupReply()
