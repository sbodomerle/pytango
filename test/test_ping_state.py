from PyTango import *
import sys
import time

#  Protect the script from Exceptions raised by the Tango or python itself
try :
	# Get proxy on the tangotest1 device
	print "Getting DeviceProxy "
	tango_test = DeviceProxy("tango/tangotest/1")

	#  ping the Device
	print "tango_test.ping() ",tango_test.ping()
	#  get the state of the Device
	tango_test_state=tango_test.state()
	print "tango_test.state() ", tango_test_state
	# Wait for the state to be ON
	while tango_test_state != DevState.ON:
		tango_test_state = tango_test.state()
		print "tango_test state ",tango_test_state
		time.sleep(1)

#	Catch Tango and Systems  Exceptions
except :
	print "Failed with exception !"
	print sys.exc_info()[0]
	print sys.exc_type
	print sys.exc_value
	print sys.exc_traceback
	raise

