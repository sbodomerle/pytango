import PyTango
import sys
import copy
#import threading

class_list = []
cpp_class_list = []
constructed_class = []

class Release:
    
    name = "PyTango"
    version = PyTango._PyTango.__version__
    version_description = PyTango._PyTango.__version_description__
    version_number = int(version.replace('.',''))
    description = "A python binding for the Tango control system"
    long_description = PyTango._PyTango.__doc__
    authors = { 'Coutinho' : ('Tiago Coutinho' , 'tcoutinho@cells.es'),
                'Ounsy'    : ('Majid Ounsy',     'majid.ounsy@synchrotron-soleil.fr'),
                'Taurel'   : ('Emmanuel Taurel', 'taurel@esrf.eu'),
              }
    url = 'http://www.tango-controls.org'
    download_url = "http://www.tango-controls.org/download#bindings"
    platform = ['Linux', 'Windows XP/2000/NT', 'Windows 95/98/ME']
    keywords = ['Tango', 'CORBA', 'binding']

#
# Two little functions and a global variables to allow 
# DS debugging using Python with Eclipse/PyDev
#

pydev_dbg = 10
pydev_trace_func = None

def PyDev_debug(in_hook):
    if in_hook is not None:
        global pydev_trace_func
        global pydev_dbg
        pydev_trace_func = in_hook
        PyTango.pydev_dbg = 1
	
def set_dbg_fct():
    if pydev_trace_func is not None:
        sys.settrace(pydev_trace_func)


#
# Several little functions to solve a default value problems
#

def set_att_val(att,data,x=1,y=0):
    if ((x == 1) and (y == 0)):
       PyTango.set_attribute_value(att,data)
    else:
       PyTango.set_attribute_value(att,data,x,y)

def set_att_val_dq(att,data,ti,qua,x=1,y=0):
    if ((x == 1) and (y == 0)):
       PyTango.set_attribute_value_date_quality(att,data,ti,qua)
    else:
       PyTango.set_attribute_value_date_quality(att,data,ti,qua,x,y)
       
def push_ev(dev,att_name,data='not_defined',x=1,y=0,filt_names=[],filt_vals=[]):
    if (data == 'not_defined'):
       PyTango.push_att_event(dev,att_name,filt_names,filt_vals)
    elif ((x == 1) and (y == 0)):
       PyTango.push_att_event(dev,att_name,filt_names,filt_vals,data)
    else:
       PyTango.push_att_event(dev,att_name,filt_names,filt_vals,data,x,y)
       
def push_change_ev(dev,att_name,data='not_defined',x=1,y=0):
    if (data == 'not_defined'):
       PyTango.push_att_ch_event(dev,att_name)
    elif ((x == 1) and (y == 0)):
       PyTango.push_att_ch_event(dev,att_name,data)
    else:
       PyTango.push_att_ch_event(dev,att_name,data,x,y)
       
def push_archive_ev(dev,att_name,data='not_defined',x=1,y=0):
    if (data == 'not_defined'):
       PyTango.push_att_ar_event(dev,att_name)
    elif ((x == 1) and (y == 0)):
       PyTango.push_att_ar_event(dev,att_name,data)
    else:
       PyTango.push_att_ar_event(dev,att_name,data,x,y)
       
def get_device_properties(dev,ds_class):
    try:
        dev.prop_util = ds_class.prop_util
        dev.device_property_list = copy.deepcopy(ds_class.device_property_list)
        class_prop = ds_class.class_property_list
        dev.prop_util.get_device_properties(dev, class_prop, dev.device_property_list)

        for prop_name in dev.device_property_list.keys():
            setattr(dev,prop_name,dev.prop_util.get_property_values(prop_name,dev.device_property_list))

        #print dev.get_name(), " device Properties:"
        #for name in dev.device_property_list.keys():
        #    print name, " :	", dev.prop_util.get_property_values(name, dev.device_property_list)

    except PyTango.DevFailed, e:
        print "----> ", e

def py_execute_with_inc_ref(dev,name,in_par=None):
    met = getattr(dev,name)
    if in_par == None:
    	ret = met()
    else:
        ret = met(in_par)
    dev._inc_ref = ret
    return ret

#
# Methods on Util
#
   
def get_class_list(self):
    return constructed_class
    
def get_device_list_by_class(self,name):
    cl_list = self.get_class_list()
    for cl in cl_list:
        if cl.get_name() == name:
	    return cl.get_device_list()
    else:
        err_mess = "Class " + name + " not found"
        PyTango.Except.throw_exception("API_ClassNotFound",err_mess,"Util::get_device_list_by_class") 	
	
	
def get_device_by_name(self,name):
    cl_list = self.get_class_list()
    for cl in cl_list:
        dev_list = cl.get_device_list()
	for dev in dev_list:
	    if dev.get_name() == name:
	        return dev
    else:		
        err_mess = "Device " + name + " not found"
        PyTango.Except.throw_exception("API_DeviceNotFound",err_mess,"Util::get_device_by_name") 
			
#
# Methods on the DeviceProxy
#

def device_proxy_write_attribute(dev,name_or_value,value,x,y):
    """Write the specified attribute.\n
           Parameters : \n
                   - attr_val : AttributeValue type
            or 
                   - attr_name : attribute name
                   - value : value to be written
                   - dim_x : x length (optional, default = 1)
                   - dim_y : y length (optional, default = 0) 
           Return     : None\n
           Example :\n
                   attr_val = AttributeValue()
                   attr_val.name = 'short_scalar'
                   attr_val.dim_x = 1
                   attr_val.dim_y = 0
                   dev.write_attribute(attr_val)
    """
    if value == None:
        dev.write_attribute_value(name_or_value)
    else:
        att = PyTango.AttributeValue()
        att.name = name_or_value
        att.value = value
        att.dim_x = x
        att.dim_y = y
        dev.write_attribute_value(att)
        
PyTango.DeviceProxy.write_attribute_value = PyTango.DeviceProxy.write_attribute
PyTango.DeviceProxy.write_attribute = lambda self, name_or_value, value=None, x = 1, y = 0 : PyTango.device_proxy_write_attribute(self, name_or_value, value, x, y)

def attribute_proxy_write(attr_proxy,value,x,y):
    """Write the specified attribute.\n
           Parameters : \n
                   - attr_val : AttributeValue type
            or 
                   - attr_name : attribute name
                   - value : value to be written
                   - dim_x : x length (optional, default = 1)
                   - dim_y : y length (optional, default = 0) 
           Return     : None\n
           Example :\n
                   attr_val = AttributeValue()
                   attr_val.name = 'short_scalar'
                   attr_val.dim_x = 1
                   attr_val.dim_y = 0
                   dev.write_attribute(attr_val)
    """
    if isinstance(value,PyTango.AttributeValue):
        attr_proxy.write_value(value)
    else:
        att = PyTango.AttributeValue()
        att.name = attr_proxy.name()
        att.value = value
        att.dim_x = x
        att.dim_y = y
        attr_proxy.write_value(att)
        
PyTango.AttributeProxy.write_value = PyTango.AttributeProxy.write
PyTango.AttributeProxy.write = lambda self, value=None, x = 1, y = 0 : PyTango.attribute_proxy_write(self, value, x, y)

#
# Dynamic attribute for python DS
# In Python DS, to read/write attributes, the kernel searches for methods
# with name based on the attribute name. In case of dynamic attributes,
# we need to build these methods dynamically
#

def add_attribute(self,attr,r_meth=None,w_meth=None,is_allo_meth=None):
    att_name = attr.get_name()
    
    add_name_in_list = False
    if r_meth is not None: 
        r_meth_name = 'read_%s' % att_name
	if hasattr(self.__class__,r_meth_name) == False:
            setattr(self.__class__,r_meth_name,r_meth)
	    add_name_in_list = True
    
    if w_meth is not None:
        w_meth_name = 'write_%s' % att_name
	if hasattr(self.__class__,w_meth_nane) == False:
            setattr(self.__class__,w_meth_name,w_meth)
	    add_name_in_list = True

    if is_allo_meth is not None:    
        allo_meth_name = 'is_%s_allowed' % att_name
	if hasattr(self.__class__,allo_meth_name)  == False:
            setattr(self.__class__,allo_meth_name,is_allo_meth)
	    add_name_in_list = True

    try:        
        self.cpp_add_attribute(attr)
	if add_name_in_list == True:
	    cl = self.get_device_class()
	    cl.dyn_att_added_methods.append(att_name)
    except:
        if add_name_in_list == True:
	    _remove_attr_meth(self,att_name)

    
def remove_attribute(self,attr_name):
    try:
#
# Call this method in a try/except in case remove_attribute
# is called during the DS shutdown sequence
#
        cl = self.get_device_class()
    except:
        return
	
    dev_list = cl.get_device_list()
    nb_dev = len(dev_list)
    if nb_dev == 1:
        self._remove_attr_meth(attr_name)
    else:
        nb_except = 0
        for dev in dev_list:
	    try:
	        dev.get_device_attr().get_attr_by_name(attr_name)
	    except:
	        nb_except =+ 1
        if nb_except == nb_dev - 1:
	    self._remove_attr_meth(attr_name)
    self.cpp_remove_attribute(attr_name)


def _remove_attr_meth(self,attr_name):
    cl = self.get_device_class()
    if cl.dyn_att_added_methods.count(attr_name) != 0:
        r_meth_name = 'read_%s' % attr_name
	if hasattr(self.__class__,r_meth_name) == True:
            delattr(self.__class__,r_meth_name)
    
        w_meth_name = 'write_%s' % attr_name
	if hasattr(self.__class__,w_meth_name) == True:
            delattr(self.__class__,w_meth_name) 
    
        allo_meth_name = 'is_%s_allowed' % attr_name
	if hasattr(self.__class__,allo_meth_name) == True:
            delattr(self.__class__,allo_meth_name)
	cl.dyn_att_added_methods.remove(attr_name)
              
#
# The PyUtil class
#

        
class PyUtil(PyTango.Util):
    def __init__(self,args):
        dot=args[0].rfind('.')
        new_str=args[0][:dot]
        args[0]=new_str
        PyTango.Util.init(len(args),args)
        PyTango.Util.python_init()

# transform attribute function as Attribute class methods
	
	PyTango.WAttribute.get_write_value = lambda self, data: PyTango.get_write_value(self,data)
	PyTango.WAttribute.set_write_value = lambda self, data: PyTango.set_write_value(self,data)
	PyTango.Attribute.set_value = set_att_val
	PyTango.Attribute.set_value_date_quality = set_att_val_dq
	
# transform device function as Device class methods

	PyTango.DeviceImpl.push_event = push_ev
	PyTango.DeviceImpl.push_change_event = push_change_ev
	PyTango.DeviceImpl.push_archive_event = push_archive_ev
	PyTango.DeviceImpl.get_device_properties = get_device_properties
	PyTango.DeviceImpl.py_exec_wir = py_execute_with_inc_ref
	PyTango.DeviceImpl.add_attribute = add_attribute
	PyTango.DeviceImpl.remove_attribute = remove_attribute
	PyTango.DeviceImpl._remove_attr_meth = _remove_attr_meth
	
	PyTango.Util.get_class_list = get_class_list
	PyTango.Util.get_device_list_by_class = get_device_list_by_class
	PyTango.Util.get_device_by_name = get_device_by_name
	             	
    def add_TgClass(self,cl_class,cl,name):
	class_list.append((cl_class,cl,name))
	
    def add_Cpp_TgClass(self,class_name,name):
	cpp_class_list.append((class_name,name))
	

#
# A generic class_factory method
#
	
def class_factory():
    if ((len(class_list) + len(cpp_class_list)) == 0):
	print 'Oups, no Tango class defined within this device server !!!'
	print 'Sorry, but I exit'
	sys.exit()
    else:
        for tup in class_list:
            #print "Creating one Tango Class :",tup[2]
            constructed_class.append(tup[0](tup[2]))

#
# A method to delete Tango classes from Python
#
	    
def delete_class_list():
    if len(constructed_class) != 0:
       del(constructed_class[:])

#
# The device "get_device_class" method written in Python
#

def get_device_class(dev):
     loop = 0
     for tup in class_list:
	 if (tup[1].__name__ == dev.__class__.__name__):
	     return constructed_class[loop]
	 else:
	     loop += 1
     return None
     

#
# The PyDeviceClass class with the generic device_factory method
#
     

class PyDeviceClass(PyTango.CppDeviceClass):
    def __init__(self,name):
        PyTango.CppDeviceClass.__init__(self,name)
	self.dyn_att_added_methods = []
        try:
            self.prop_util = PropUtil()
	    self.py_dev_list = []
	    has_cl_prop = hasattr(self,"class_property_list")
	    has_dev_prop = hasattr(self,"device_property_list")
	    if has_cl_prop == True and has_dev_prop == True:
                self.prop_util.set_default_property_values(self,self.class_property_list, self.device_property_list)
            if has_cl_prop == True:
                self.prop_util.get_class_properties(self, self.class_property_list)
                for prop_name in self.class_property_list.keys():
                    setattr(self,prop_name,self.prop_util.get_property_values(prop_name,self.class_property_list))

                #print self.get_name(), " class Properties:"
                #for name in self.class_property_list.keys():
                #    print name, " :	", self.prop_util.get_property_values(name, self.class_property_list)

        except PyTango.DevFailed, e:
            print "----> ", e
	
	
    def device_factory(self,devicelist):
       loop = 0
       for tup in class_list:
           if (tup[0].__name__ == self.__class__.__name__):
	       tup[1].get_device_class = get_device_class
	       tmp_dev_list = []		     
               for dev_name in devicelist:
                   #print "Creating one Tango device of name",dev_name
                   self.z = tup[1](constructed_class[loop],dev_name)
		   self.cpp_add_device(self.z)
		   tmp_dev_list.append(self.z)
		   
	       self.dyn_attr(tmp_dev_list)
		   
	       for dev in tmp_dev_list:
		   if ((PyTango.Util._UseDb == True) and (PyTango.Util._FileDb == False)):
		        self.export_device(dev)
		   else:
		        self.export_device(dev,dev.get_name())
	       self.py_dev_list = self.py_dev_list + tmp_dev_list
	       break
	   loop += 1
	   
    def get_device_list(self):
       return self.py_dev_list
       
    def dyn_attr(self,devicelist):
	pass
       
    def device_destroyer(self,name):
       for d in self.py_dev_list:
           if d.get_name() == name:
	       dev_cl = d.get_device_class()
	       dev_cl.cpp_device_destroyer(name)
	       self.py_dev_list.remove(d)
	       break
       else:
           err_mess = "Device " + name + " not in tango class device list!"
           PyTango.Except.throw_exception("API_CantDestroyDevice",err_mess,"PyDeviceClass::device_destroyer") 
       

#---------------------------------------------------------------------------------

#==================================================================
#
#	A Tool Class
#
#==================================================================
class PropUtil:
    def __init__(self):
        #	Just create Util instance
        #print "in PropUtil constructor"
        self.db = PyTango.Database()

#------------------------------------------------------------------
#	Set Default Properties to Wizard Interface
#------------------------------------------------------------------
    def set_default_property_values(self, devclass, class_prop, dev_prop):
        for name in class_prop.keys():
            type = self.get_property_type(name, class_prop)
            val  = self.get_property_values(name, class_prop)
            val  = self.values2string(val, type)
            desc = self.get_property_description(name, class_prop)
            devclass.add_wiz_class_prop(name, desc, val)

        for name in dev_prop.keys():
            type = self.get_property_type(name, dev_prop)
            val  = self.get_property_values(name, dev_prop)
            val  = self.values2string(val, type)
            desc = self.get_property_description(name, dev_prop)
            devclass.add_wiz_dev_prop(name, desc, val)


#------------------------------------------------------------------
#	Get class Properties
#------------------------------------------------------------------
    def get_class_properties(self, devclass, class_prop):

		#	initialize default values
        if (class_prop == {} or PyTango.Util._UseDb==False):
            return

		#	Call database to get properties
        props = self.db.get_class_property(devclass.get_name(), class_prop.keys())
		
		#	if value defined in database, store it
        for name in class_prop.keys():
            if (props[name]!=[]):
                type   = self.get_property_type(name, class_prop)
                values = self.stringArray2values(props[name], type)
                self.set_property_values(name, class_prop, values)
            else:
                print name, " property NOT found in database"


#------------------------------------------------------------------
#	Get Device Properties
#------------------------------------------------------------------
    def get_device_properties(self, dev, class_prop, dev_prop):

		#	initialize default properties
        if (dev_prop == {} or PyTango.Util._UseDb==False):
            return

		#	Call database to get properties
        props = self.db.get_device_property(dev.get_name(),dev_prop.keys())
		
		#	if value defined in database, store it
        for name in dev_prop.keys():
            if (props[name]!=[]):
                type   = self.get_property_type(name, dev_prop)
                values = self.stringArray2values(props[name], type)
                if (values!=[]):
                    self.set_property_values(name, dev_prop, values)
                else:
					#	Try to get it from class property
                    values = self.get_property_values(name, class_prop)
                    if (values != []):
                        type   = self.get_property_type(name, class_prop)
                        values = self.stringArray2values(values, type)
                        if (values != []):
                            self.set_property_values(name, dev_prop, values)

#------------------------------------------------------------------
#	Get Property type
#------------------------------------------------------------------
    def get_property_type(self, propname, properties):
        try:
            type = properties[propname][0]
        except Exception, e:
            type = PyTango.ArgType.DevVoid
        return type


#------------------------------------------------------------------
#	Set Property Values
#------------------------------------------------------------------
    def set_property_values(self, propname, properties, values):
        properties[propname][2] = values

#------------------------------------------------------------------
#	Get Property Values
#------------------------------------------------------------------
    def get_property_values(self, propname, properties):
        try:
            tg_type = self.get_property_type(propname, properties)
            val  = properties[propname][2]
			#type = type(val)
        except Exception, e:
            val = []
		
        if (self.is_array(tg_type)==True or val==[]):
            return val
        else:
			#print propname, " -> ", val, ":	", type(val)
            if (type(val)==list):
                return val[0]
            else:
                return val

#------------------------------------------------------------------
#	Get Property Description
#------------------------------------------------------------------
    def get_property_description(self, propname, properties):
        return properties[propname][1]


#------------------------------------------------------------------
#	Check if type is an array (sequence) type
#------------------------------------------------------------------
    def is_array(self, type):
        if type in [PyTango.DevString,PyTango.DevBoolean,PyTango.DevShort,
                    PyTango.DevLong,PyTango.DevFloat,PyTango.DevDouble,PyTango.DevLong64]:
            return False
        else:
            return True
	
#------------------------------------------------------------------
#	Convert string array properties in specified type
#------------------------------------------------------------------
    def stringArray2values(self, argin, argout_type):

		#	Scalar cases
        if (argout_type==PyTango.DevString):
            return argin[0]

        if (argout_type==PyTango.DevBoolean):
            s = argin[0].lower().capitalize()
            return (s=='True')

        if (argout_type==PyTango.DevShort or
            argout_type==PyTango.DevLong):
            return int(argin[0])

        if(argout_type==PyTango.DevLong64):
            return long(argin[0])

        if (argout_type==PyTango.DevFloat or
            argout_type==PyTango.DevDouble):
            return float(argin[0])

		#sequence cases
        if (argout_type==PyTango.DevVarStringArray):
            return argin

        if (argout_type==PyTango.DevVarShortArray or
            argout_type==PyTango.DevVarLongArray):
            argout = []
            for x in argin:
                argout.append(int(x))
            return argout

        if (argout_type==PyTango.DevVarLong64Array):
            argout = []
            for x in argin:
                argout.append(long(x))
            return argout

        if (argout_type==PyTango.DevVarFloatArray or
            argout_type==PyTango.DevVarDoubleArray):
            argout = []
            for x in argin:
                argout.append(float(x))
            return argout
		
        return []

#------------------------------------------------------------------
#	Convert values in specified type in a String
#------------------------------------------------------------------
    def values2string(self, argin, argout_type):
        argout = ""
        if (argin==[]):
            return argout

		#	Scalar cases
        if (argout_type==PyTango.DevString):
            if (type(argin)==list):
                return argin[0].rstrip()
            else:
                return argin.rstrip()

        if (argout_type==PyTango.DevBoolean):
            if (type(argin)==list):
                return str(argin[0])
            else:
                return str(argin)

        if (argout_type==PyTango.DevShort or
            argout_type==PyTango.DevLong or
            argout_type==PyTango.DevLong64):
            if (type(argin)==list):
                return ("%d" % argin[0])
            else:
                return ("%d" % argin)

        if (argout_type==PyTango.DevFloat  or
            argout_type==PyTango.DevDouble):
            if (type(argin)==list):
                return ("%f" % argin[0])
            else:
                return ("%f" % argin)

		#sequence cases
        if (argout_type==PyTango.DevVarStringArray):
            for s in argin:
                argout = argout + s + "\n"
            return argout.rstrip()

        if (argout_type==PyTango.DevVarShortArray or
            argout_type==PyTango.DevVarLongArray or
            argout_type==PyTango.DevVarLong64Array):
            for x in argin:
                argout = argout + ("%d" % x) + "\n"
            return argout.rstrip()

        if (argout_type==PyTango.DevVarFloatArray or
            argout_type==PyTango.DevVarDoubleArray):
            for x in argin:
                argout = argout + ("%f" % x) + "\n"
            return argout.rstrip()

		#default case
        return argout

# In the future maybe necessary to use this special Thread class
# in order to execute tango code in a python thread which is not omni
# For now is in standby because the problem is being solved at the
# Tango C++ level  
#class Thread(threading.Thread):
#    
#    def run(self):
#        PyTango._PyTango._create_omni_dummy()
#        threading.Thread.run(self)
#        PyTango._PyTango._release_omni_dummy()
        
