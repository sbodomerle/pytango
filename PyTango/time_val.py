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

import time
import datetime
import operator

from _PyTango import TimeVal

def __TimeVal__init(self, a=None, b=None, c=None):
    TimeVal.__init_original(self)
    if a is None: 
        return

    if isinstance(a, datetime.datetime):
        assert(b is None and c is None)
        a = time.mktime(a.timetuple()) + a.microsecond*1E-6

    elif operator.isNumberType(a):
        if b is None:
            self.tv_sec = int(a)
            usec = (a - self.tv_sec) * 1E6
            self.tv_usec = int(usec)
            self.tv_nsec = int((usec - self.tv_usec) * 1E3)
        else:
            self.tv_sec, self.tv_usec, self.tv_nsec = a, b, c

def __TimeVal__totime(self):
    """
    totime(self) -> float
    
        Returns a float representing this time value
    
        Parameters : None
        Return     : a float representing the time value
        
    .. versionadded:: 7.1.0"""
    return self.tv_sec + 1E-6*self.tv_usec + 1E-9*self.tv_nsec

def __TimeVal__todatetime(self):
    """
    todatetime(self) -> datetime.datetime
    
        Returns a :class:`datetime.datetime` object representing
        the same time value
    
        Parameters : None
        Return     : (datetime.datetime) the time value in datetime format
        
    .. versionadded:: 7.1.0"""
    return datetime.datetime.fromtimestamp(self.totime())

def __TimeVal__fromtimestamp(ts):
    """
    fromtimestamp(ts) -> TimeVal

        A static method returning a :class:`PyTango.TimeVal` object representing
        the given timestamp
    
        Parameters :
            - ts : (float) a timestamp
        Return     : (TimeVal) representing the given timestamp
        
    .. versionadded:: 7.1.0"""
    return TimeVal(ts)

def __TimeVal__fromdatetime(dt):
    """
    fromdatetime(dt) -> TimeVal

        A static method returning a :class:`PyTango.TimeVal` object representing
        the given :class:`datetime.datetime`
    
        Parameters :
            - dt : (datetime.datetime) a datetime object
        Return     : (TimeVal) representing the given timestamp
        
    .. versionadded:: 7.1.0

    .. versionadded:: 7.1.2
        Documented
    """
    return TimeVal(dt)

def __TimeVal__now():
    """
    now() -> TimeVal

        A static method returning a :class:`PyTango.TimeVal` object representing
        the current time
    
        Parameters : None
        Return     : (TimeVal) representing the current time
        
    .. versionadded:: 7.1.0

    .. versionadded:: 7.1.2
        Documented
    """
    return TimeVal(time.time())

def __TimeVal__strftime(self, format):
    """
    strftime(self, format) -> str

        Convert a time value to a string according to a format specification.
    
        Parameters : 
            format : (str) See the python library reference manual for formatting codes
        Return     : (str) a string representing the time according to a format specification.
        
    .. versionadded:: 7.1.0

    .. versionadded:: 7.1.2
        Documented
    """
    return self.todatetime().strftime(format)

def __TimeVal__isoformat(self, sep='T'):
    """
    isoformat(self, sep='T') -> str

        Returns a string in ISO 8601 format, YYYY-MM-DDTHH:MM:SS[.mmmmmm][+HH:MM]
    
        Parameters : 
            sep : (str) sep is used to separate the year from the time, and defaults to 'T'
        Return     : (str) a string representing the time according to a format specification.
        
    .. versionadded:: 7.1.0

    .. versionadded:: 7.1.2
        Documented
    
    .. versionchanged:: 7.1.2
        The `sep` parameter is not mandatory anymore and defaults to 'T' (same as :meth:`datetime.datetime.isoformat`)
    """
    return self.todatetime().isoformat(sep)

def __TimeVal__str__(self):
    """
    __str__(self) -> str

        Returns a string representation of TimeVal
    
        Parameters : None
        Return     : (str) a string representing the time (same as :class:`datetime.datetime`)
        
    .. versionadded:: 7.1.0

    .. versionadded:: 7.1.2
        Documented
    """
    return str(self.todatetime())

def __init_TimeVal():
    TimeVal.__init_original = TimeVal.__init__
    TimeVal.__init__ = __TimeVal__init
    TimeVal.totime = __TimeVal__totime
    TimeVal.todatetime = __TimeVal__todatetime
    TimeVal.fromtimestamp = staticmethod(__TimeVal__fromtimestamp)
    TimeVal.fromdatetime = staticmethod(__TimeVal__fromdatetime)
    TimeVal.now = staticmethod(__TimeVal__now)
    TimeVal.strftime = __TimeVal__strftime
    TimeVal.isoformat = __TimeVal__isoformat
    TimeVal.__str__ = __TimeVal__str__
    
def init(doc=True):
    __init_TimeVal()

