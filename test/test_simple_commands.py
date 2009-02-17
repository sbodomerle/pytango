from PyTango import *
import sys
import time

#  Protect the script from Exceptions raised by the Tango or python itself
try :
	# Get proxy on the tangotest1 device
	print "Getting DeviceProxy "
	tangotest = DeviceProxy("tango/tangotest/1")

	# First use the classical command_inout way to execute the DevString command
	# (DevString in this case is a command of the TangoTest device)
	
	result= tangotest.command_inout("DevString", "First hello to device")
	print "Result of execution of DevString command=", result

	# the same with a Device specific command 
	result= tangotest.DevString("Second Hello to device")
	print "Result of execution of DevString command=", result

	# Please note that argin argument type is automagically managed by python
	result= tangotest.DevULong(12456)
	print "Result of execution of Status command=", result
	

#	Catch Tango and Systems  Exceptions
except :
	print "Failed with exception !"
	print sys.exc_info()[0]
	
