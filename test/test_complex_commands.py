from PyTango import *
import sys
import time

#  Protect the script from Exceptions raised by the Tango or python itself
try :
	# Get proxy on the tangotest1 device
	print "Getting DeviceProxy "
	tangotest = DeviceProxy("tango/tangotest/1")

	# The input argument is an array of strings so create it
	argin=["Hello", "TangoTest device"]
	result= tangotest.DevVarStringArray(argin)
	print "Result of execution of DevVarStringArray command=", result
	
	# The input argument is an array of doubles so create it
	argin=[1.2,2.365,587.123]
	result= tangotest.DevVarDoubleArray(argin)
	print "Result of execution of DevVarDoubleArray command=", result
	
	# The input argument is a composite DevVarLongStringArray
	# so create the argin variable containing
	# an array of longs and an array of strings

	argin = ([1,2,3], ["Hello", "TangoTest device"])

	result= tangotest.DevVarLongStringArray(argin)
	print "Result of execution of DevVarLongStringArray command=", result
	

#	Catch Tango and Systems  Exceptions
except DevFailed:
	exctype , value = sys.exc_info()[:2]
	print "Failed with exception !", exctype
	for err in value :
		print "---ERROR ELEMENT-------"
		print "reason:" , err['reason']
		print "description:" , err["desc"]
		print "origin:" , err["origin"]
		print "severity:" , err["severity"]
	
