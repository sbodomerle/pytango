#############################################################################
##
## This file is part of PyTango, a python binding for Tango
##
## http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html
##
## (copyleft) CELLS / ALBA Synchrotron, Bellaterra, Spain
##
## This is free software; you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##
## This software is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this program; if not, see <http://www.gnu.org/licenses/>.
###########################################################################

"""
This is an internal PyTango module.
"""

__all__ = []

__docformat__ = "restructuredtext"

import copy

from utils import document_method as __document_method
from utils import document_enum as __document_enum
from _PyTango import DeviceAttribute, ExtractAs

def __DeviceAttribute__get_data(self):
    return self.get_data_raw().extract()

def __DeviceAttribute__init(self, da=None):
    if da is None:
        DeviceAttribute.__init_orig(self)
    else:
        DeviceAttribute.__init_orig(self, da)
        try: self.value = copy.deepcopy(da.value)
        except: pass
        try: self.w_value = copy.deepcopy(da.w_value)
        except: pass
        try: self.scalar_w_value = da.scalar_w_value
        except: pass
        self.type = da.type
        self.is_empty = da.is_empty
        self.has_failed = da.has_failed

def __doc_DeviceAttribute():
    def document_method(method_name, desc, append=True):
        return __document_method(DeviceAttribute, method_name, desc, append)

    DeviceAttribute.__doc__ = """
        This is the fundamental type for RECEIVING data from device attributes.

        It contains several fields. The most important ones depend on the
        ExtractAs method used to get the value. Normally they are:
        
            - value   : Normal scalar value or numpy array of values.
            - w_value : The write part of the attribute.
            
        See other ExtractAs for different possibilities. There are some more
        fields, these really fixed:
        
            - name        : (str)
            - data_format : (AttrDataFormat) Attribute format
            - quality     : (AttrQuality)
            - time        : (TimeVal)
            - dim_x       : (int) attribute dimension x
            - dim_y       : (int) attribute dimension y
            - w_dim_x     : (int) attribute written dimension x
            - w_dim_y     : (int) attribute written dimension y
            - r_rimension : (tuple<int,int>) Attribute read dimensions.
            - w_dimension : (tuple<int,int>) Attribute written dimensions.
            - nb_read     : (int) attribute read total length
            - nb_written  : (int) attribute written total length
            

        And two methods:
            - get_date
            - get_err_stack
    """

    document_method("get_date", """
    get_date(self) -> TimeVal

            Get the time at which the attribute was read by the server.
            
            Note: It's the same as reading the "time" attribute.
        
        Parameters : None
        Return     : (TimeVal) The attribute read timestamp.
    """ )

    document_method("get_err_stack", """
    get_err_stack(self) -> sequence<DevError>

            Returns the error stack reported by the server when the
            attribute was read.

        Parameters : None
        Return     : (sequence<DevError>)
    """ )

def __init_DeviceAttribute():
    DeviceAttribute.__init_orig = DeviceAttribute.__init__
    DeviceAttribute.__init__ = __DeviceAttribute__init
    DeviceAttribute.ExtractAs = ExtractAs

def init(doc=True):
    __init_DeviceAttribute()
    if doc:
        __doc_DeviceAttribute()
