import os
import sys

from PyTango import *

try : 
    #  A reference on the DataBase
    print ".."

    # In this example we use command_inout on the DataBase device and NOT the DataBase API
    # as there as been some bugs in the underlying TANGO DataBase
    print "********** Use command_inout on the DataBase device *************"

    device_name="px1/tdl/mouse2"
    db = DeviceProxy ("sys/database/dbds")

    print "Defining properties for first attribute in the correct python structure "
    
    properties_positionX=[device_name, "1", "positionX", "6", "min_value", "10", "max_value", "1000",
                      "min_alarm","50","max_alarm","950","label","Pos en pixel","format","%3d"]
    prop=db.command_inout ("DbPutDeviceAttributeProperty",properties_positionX)

    print "Defining properties for the second attribute in the correct python structure "
    properties_positionY=[device_name, "1", "positionY", "6", "min_value", "10", "max_value", "750",
                      "min_alarm","50","max_alarm","700","label","Pos en pixel","format","%3d"]
    prop=db.command_inout ("DbPutDeviceAttributeProperty",properties_positionY)


 # First use the dedicated DataBase API
    db = Database()

    print "**********  First use the dedicated DataBase API *************" 
    mouse_name="px1/tdl/mouse3"
    properties_positionX={"min_value" : ["20"], 
                      "max_value" : ["1000"], 
                      "min_alarm":["50"],
                     "max_alarm":["950"],
                     "label":["Pos en pixel"],
                     "format":["%3d"]}

    properties_positionY={"min_value" : ["20"], 
                      "max_value" : ["1000"], 
                      "min_alarm":["50"],
                     "max_alarm":["950"],
                     "label":["Pos en pixel"],
                     "format":["%3d"]}

    attr_props_mouse = { "positionX" : properties_positionX , 
                     "positionY" : properties_positionY }

    print "put_device_attribute_property"
    db.put_device_attribute_property(mouse_name,attr_props_mouse)




except :
    	print "Failed with exception !"
	print sys.exc_info()[0]
	print sys.exc_info()[1]
