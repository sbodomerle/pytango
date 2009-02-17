from PyTango import *
import sys
import time

#  Protect the script from Exceptions raised by the Tango or python itself
try :
	# Get proxy on the tangotest1 device
	print "Getting DeviceProxy "
	tangotest = DeviceProxy("tango/tangotest/1")

	#Read a scalar attribute
	scalar=tangotest.read_attribute("long_scalar")
	print "attribute value", scalar.value
	#Read a spectrum attribute
	spectrum=tangotest.read_attribute("double_spectrum")
	print "attribute value", spectrum.value

	# Write a scalar attribute so use the scalar structure
	print "Writing attributes"
	
	# Write a scalar attribute so use the scalar structure
	scalar.value = 18
	print "attribute scalar  ", scalar
	print "Writing scalar attributes"
	tangotest.write_attribute(scalar)
	
	# Write a scalar attribute so use the scalar structure
	spectrum.value = [1.2,3.2,12.3]
	print "attribute spectrum ", spectrum
	print "Writing spectrum attributes"
	tangotest.write_attribute(spectrum)
	

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
	
