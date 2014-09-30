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
This is an internal PyTango module.
"""

__all__ = [ "Release" ]

__docformat__ = "restructuredtext"

class Release:
    """
        Release information:
            - name : (str) package name
            - version_info : (tuple<int,int,int,str,int>) The five components
              of the version number: major, minor, micro, releaselevel, and
              serial.
            - version : (str) package version in format <major>.<minor>.<micro>
            - version_long : (str) package version in format
              <major>.<minor>.<micro><releaselevel><serial>
            - version_description : (str) short description for the current
              version
            - version_number : (int) <major>*100 + <minor>*10 + <micro>
            - description : (str) package description
            - long_description : (str) longer package description
            - authors : (dict<str(last name), tuple<str(full name),str(email)>>)
              package authors
            - url : (str) package url
            - download_url : (str) package download url
            - platform : (seq<str>) list of available platforms
            - keywords : (seq<str>) list of keywords
            - license : (str) the license"""
    name = 'PyTango'
    version_info = (8, 1, 4, 'final', 0)
    version = '.'.join(map(str, version_info[:3]))
    version_long = version + ''.join(map(str, version_info[3:]))
    version_description = 'This version implements the C++ Tango 8.1 API.'
    version_number = int(version.replace('.',''))
    description = 'A python binding for the Tango control system'
    long_description = 'This module implements the Python Tango Device API ' \
                       'mapping.'
    license = 'LGPL'
    authors = { 'Coutinho' : ('Tiago Coutinho' , 'coutinho@esrf.fr') }
    author_lines = "\n".join([ "%s <%s>" % x for x in authors.values()])
    url = 'http://www.tinyurl.com/PyTango/'
    download_url = 'http://pypi.python.org/pypi/PyTango'
    platform = ['Linux', 'Windows XP/Vista/7/8']
    keywords = ['Tango', 'CORBA', 'binding']
    
