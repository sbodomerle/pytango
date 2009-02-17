# This is an example of Tango Device Server in Python
# It has one Tango class with 2 commands and 2 attributes
# The commands are
#
#	IOLong which receives a long and retrun it multiply by 2
#`	IOStringArray which receives an array of strings and
#		      returns it but in the reverse order
#
# The attributes are
#
#       Long_attr which is SCALAR, Long and READ
#	Short_attr_rw which is SCALAR, Short and READ_WRITE
#

import PyTango
import sys
	 
	       
class PyDsExp(PyTango.Device_3Impl):
    def __init__(self,cl,name):
        PyTango.Device_3Impl.__init__(self,cl,name)
        print 'In PyDsExp __init__'
	PyDsExp.init_device(self)
		
    def init_device(self):
        print 'In Python init_device method'
	self.set_state(PyTango.DevState.ON)
	self.attr_short_rw = 66
	self.attr_long = 1246

#------------------------------------------------------------------	

    def delete_device(self):
	print "[Device delete_device method] for device",self.get_name()
	
#------------------------------------------------------------------	
	
    def is_IOLong_allowed(self):
        if (self.get_state() == PyTango.DevState.ON):
	    return True
	else:
	    return False
	    
    def IOLong(self,in_data):
        print "[IOLong::execute] received number",in_data
        return in_data * 2;
	
#------------------------------------------------------------------	
	
    def is_IOStringArray_allowed(self):
        if (self.get_state() == PyTango.DevState.ON):
	    return True
	else:
	    return False
	    
    def IOStringArray(self,in_data):
	out_data=in_data
	out_data.reverse()
	self.y = out_data
        return out_data	
	
		
#------------------------------------------------------------------	
#------------------------------------------------------------------
#
#		ATTRIBUTES
#
#------------------------------------------------------------------
#------------------------------------------------------------------



    def read_attr_hardware(self,data):
        print 'In read_attr_hardware'

#------------------------------------------------------------------

    def read_Long_attr(self,the_att):
        print "[PyDsExp::read_attr] attribute name Long_attr"
      	the_att.set_value(self.attr_long)


#------------------------------------------------------------------

    def read_Short_attr_rw(self,the_att):
      	print "[PyDsExp::read_attr] attribute name Short_attr_rw"
      	the_att.set_value(self.attr_short_rw)
	
    
#------------------------------------------------------------------


    def write_Short_attr_rw(self,the_att):
	print "In write_Short_attr_rw for attribute ",the_att.get_name()
	data=[]
	the_att.get_write_value(data)
	self.attr_short_rw = data[0]

	
	
	
class PyDsExpClass(PyTango.PyDeviceClass):

    cmd_list = {'IOLong':[[PyTango.DevLong,"Number"],[PyTango.DevLong,"Number * 2"]],
                'IOStringArray':[[PyTango.DevVarStringArray,"Array of string"],[PyTango.DevVarStringArray,"This reversed array"]]	
	       }
	       	
	       
    attr_list = {'Long_attr':[[PyTango.DevLong,PyTango.SCALAR,PyTango.READ],{'min alarm':1000,'max alarm':1500}],
                 'Short_attr_rw':[[PyTango.DevShort,PyTango.SCALAR,PyTango.READ_WRITE]]
	      }	
			  
				  
if __name__ == '__main__':
    py = PyTango.PyUtil(sys.argv)
    py.add_TgClass(PyDsExpClass,PyDsExp,'PyDsExp')

    U = PyTango.Util.instance()
    U.server_init()
    U.server_run()
