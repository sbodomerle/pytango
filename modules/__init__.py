from _PyTango import *
from TgFactory import *

# For readability, add some global to the module
# For the ArgType, do not add the DevState type

for x in PyTango.AttrDataFormat.values.values():
    PyTango.__dict__.update({str(x):x})
for x in PyTango.AttrWriteType.values.values():
    PyTango.__dict__.update({str(x):x})
for x in PyTango.ArgType.values.values():
    if (x == PyTango.ArgType.DevState):
     continue
    PyTango.__dict__.update({str(x):x})

__author__ = "\n".join([ "%s <%s>" % x for x in PyTango.Release.authors.values()])
__version__ = PyTango.Release.version
__version_number__ = PyTango.Release.version_number
__version_description__  = PyTango.Release.version_description
__doc__ = PyTango.Release.long_description
