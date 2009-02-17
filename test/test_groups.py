"""
 Test Requirements:
   A Tango device server instance of DevTest must be running with at least
   one device called tango/tangotest/1* and one device called
   tango/tangotest/2*.
   For example, a possible configuration could be:
   tango/tangotest/1_1
   tango/tangotest/1_2
   tango/tangotest/2_1
   tango/tangotest/2_2
   These will be the devices used in a group.
"""

from PyTango import *
import sys
import time



def create_group(grp_type):
    g = PyTango.Group(grp_type)

    if grp_type == "plain":
        g.add("tango/tangotest/*")
    else:
        grp1 = PyTango.Group("group1")
        grp2 = PyTango.Group("group2")

        g.add(grp1);
        g.add(grp2);

        grp1.add("tango/tangotest/1*")
        grp2.add("tango/tangotest/2*")

    return g

def test_group(g):

    out_prefix = "-=- %s %s " % (g.get_name(),"group:")

    if g.ping() == False:
        print out_prefix, "At least one element is unavailable!"

#
# Write Scalar long attribute (mode 1)
#
    print out_prefix, "Write attribute 1 on long scalar ... ",
    attr_value = PyTango.AttributeValue()
    attr_value.name = "Long_attr_with_w2"
    attr_value.value = 1234
    res = g.write_attribute(attr_value)
    
    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write scalar (long):"
            print e.get_data()
    if success == True: print "[OK]"
    
#
# Read Scalar long attribute
#

    print out_prefix, "Read attribute scalar (long) ... ",
    res = g.read_attribute("Long_attr_with_w2")

    for e in res:
        if e.get_data() != attr_value.value:
            raise Exception("Unexpected value read scalar (long) Expected 1234, Received: ",e.get_data())
    print "[OK]"
    
#
# Read multiple scalar long attributes
#
    print out_prefix, "Read multiple attributes scalar (long) ... ",
    res = g.read_attributes(["Long_attr_with_w","Long_attr_with_w2"])
    
    exp_val = g.get_size()*[1246,attr_value.value]
    i = 0
    for e in res:
        if e.get_data() != exp_val[i]:
            raise Exception("Unexpected value read scalar (long) Expected 1234, Received: ",e.get_data())
        i = i+1
    print "[OK]"
    
#
# Write Scalar long attribute (mode 2)
#

    print out_prefix, "Write attribute 2 scalar (long) ... ",
    res = g.write_attribute("Long_attr_with_w2",1001)
    
    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write long scalar:"
            print e.get_data()
    if success == True: print "[OK]"
    
#
# Read Scalar long attribute
#

    print out_prefix, "Read attribute scalar (long) ... ",
    res = g.read_attribute("Long_attr_with_w2")

    for e in res:
        if e.get_data() != 1001:
            raise Exception("Unexpected value read scalar (long) Expected 1001, Received: ",e.get_data())
    print "[OK]"
    
#
# Write Scalar long attribute with individualized arguments
#
    print out_prefix, "Write attribute scalar (long) with individualized arguments ... ",
    arg = []
    for i in range(0,g.get_size()):
        arg.append(170-10*i)

    res = g.write_attribute("Long_attr_with_w2",arg)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write long scalar:"
            print e.get_data()
    if success == True: print "[OK]"
    
    print out_prefix, "Read attribute scalar (long) ... ",
    res = g.read_attribute("Long_attr_with_w2")

    i = 0
    for e in res:
        if e.get_data() != arg[i]:
            raise Exception("Unexpected value read scalar (long) Expected ",arg[i],", Received: ",e.get_data())
        i = i+1
    print "[OK]"
            
#
# Write Spectrum long attribute
#
    print out_prefix, "Write attribute 1 spectrum (long) ... ",
    attr_value = PyTango.AttributeValue()
    attr_value.name = "Long_spec_attr_rw"
    attr_value.value = [123,454,321]
    res = g.write_attribute(attr_value)
    
    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write spectrum (long):"
            print e.get_data()
    if success == True: print "[OK]"
    
    print out_prefix, "Read attribute spectrum (long) ... ",
    res = g.read_attribute("Long_spec_attr_rw")

    for e in res:
        for i in range(0,len(attr_value.value)):
            if e.get_data()[i] != attr_value.value[i]:
                raise Exception("Unexpected value read spectrum (long) Expected",attr_value.value,", Received: ",e.get_data())
    print "[OK]"


    print out_prefix, "Write attribute 2 spectrum (long) ... ",
    val = [5432,1234,5]
    res = g.write_attribute("Long_spec_attr_rw",val)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write long scalar:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute spectrum (long) ... ",
    res = g.read_attribute("Long_spec_attr_rw")

    for e in res:
        for i in range(0,len(val)):
            if e.get_data()[i] != val[i]:
                raise Exception("Unexpected value read spectrum (long) Expected",val,", Received: ",e.get_data())
    print "[OK]"

    print out_prefix, "Write attribute spectrum (long) with individualized arguments ... ",
    arg = []
    for i in range(0,g.get_size()):
        arg.append([170,170-10*i])

    res = g.write_attribute("Long_spec_attr_rw",arg,)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write long spectrum:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute spectrum (long) ... ",
    res = g.read_attribute("Long_spec_attr_rw")

    i = 0
    for e in res:
        for j in range(0,len(arg[i])):
            if e.get_data()[j] != arg[i][j]:
                raise Exception("Unexpected value read scalar (long) Expected",arg[i],", Received: ",e.get_data())
        i = i+1
    print "[OK]"

#
# Write asynch Scalar long attribute
#
    print out_prefix, "Write asynch attribute 1 on long scalar ... ",
    attr_value = PyTango.AttributeValue()
    attr_value.name = "Long_attr_with_w2"
    attr_value.value = 1111
    req_id = g.write_attribute_asynch(attr_value)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch scalar (long):"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute asynch scalar (long) ... ",
    req_id = g.read_attribute_asynch("Long_attr_with_w2")
    res = g.read_attribute_reply(req_id)    

    for e in res:
        if e.get_data() != 1111:
            raise Exception("Unexpected value read asynch scalar (long) Expected 1111, Received: ",e.get_data())
    print "[OK]"

    print out_prefix, "Write attribute asynch 2 scalar (long) ... ",
    req_id = g.write_attribute_asynch("Long_attr_with_w2",1222)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch long scalar:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute asynch scalar (long) ... ",
    req_id = g.read_attribute_asynch("Long_attr_with_w2")
    res = g.read_attribute_reply(req_id)

    for e in res:
        if e.get_data() != 1222:
            raise Exception("Unexpected value read asynch scalar (long) Expected 1222, Received: ",e.get_data())
    print "[OK]"

    print out_prefix, "Write attribute asynch scalar (long) with individualized arguments ... ",
    arg = []
    for i in range(0,g.get_size()):
        arg.append(270-10*i)

    req_id = g.write_attribute_asynch("Long_attr_with_w2",arg)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch long scalar:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute asynch scalar (long) ... ",
    req_id = g.read_attribute_asynch("Long_attr_with_w2")
    res = g.read_attribute_reply(req_id)

    i = 0
    for e in res:
        if e.get_data() != arg[i]:
            raise Exception("Unexpected value read asynch scalar (long) Expected",arg[i],", Received: ",e.get_data())
        i = i+1            
    print "[OK]"
                
#
# Write asynch Spectrum long attribute
#
    print out_prefix, "Write attribute asynch 1 spectrum (long) ... ",
    attr_value = PyTango.AttributeValue()
    attr_value.name = "Long_spec_attr_rw"
    attr_value.value = [111,222,333,444]
    req_id = g.write_attribute_asynch(attr_value)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch spectrum (long):"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute asynch spectrum (long) ... ",
    req_id = g.read_attribute_asynch("Long_spec_attr_rw")
    res = g.read_attribute_reply(req_id)

    for e in res:
        for i in range(0,len(attr_value.value)):
            if e.get_data()[i] != attr_value.value[i]:
                raise Exception("Unexpected value read asynch scalar (long) Expected",attr_value.value,", Received: ",e.get_data())
    print "[OK]"

    print out_prefix, "Write attribute asynch 2 spectrum (long) ... ",
    val = [444,333,222,111]
    req_id = g.write_attribute_asynch("Long_spec_attr_rw",val)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch long scalar:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute asynch spectrum (long) ... ",
    req_id = g.read_attribute_asynch("Long_spec_attr_rw")
    res = g.read_attribute_reply(req_id)

    for e in res:
        for i in range(0,len(val)):
            if e.get_data()[i] != val[i]:
                raise Exception("Unexpected value read asynch scalar (long) Expected",val,", Received: ",e.get_data())
    print "[OK]"

    print out_prefix, "Write attribute asynch spectrum (long) with individualized arguments ... ",
    arg = []
    for i in range(0,g.get_size()):
        arg.append([270,270-10*i])

    req_id = g.write_attribute_asynch("Long_spec_attr_rw",arg)
    res = g.write_attribute_reply(req_id)

    success = True
    for e in res:
        if e.has_failed():
            success = False
            print "Error trying to write asynch long spectrum:"
            print e.get_data()
    if success == True: print "[OK]"            

    print out_prefix, "Read attribute spectrum (long) ... ",
    req_id = g.read_attribute_asynch("Long_spec_attr_rw")
    res = g.read_attribute_reply(req_id)

    i = 0
    for e in res:
        for j in range(0,len(arg[i])):
            if e.get_data()[j] != arg[i][j]:
                raise Exception("Unexpected value read asynch scalar (long) Expected",arg[i],", Received: ",e.get_data())
        i = i+1
    print "[OK]"

#
# Command
#

    print out_prefix, "command scalar (long) ... ",
    res = g.command_inout("IOLong",123,True)

    success = True
    for e in res:
        if e.get_data() != 246:
            success = False
            raise Exception("Unexpected cmd result. Expected 246, Received: ",e.get_data())
    if success == True: print "[OK]"            

    print out_prefix, "command scalar (long) with individualized arguments ... ",
    res = g.command_inout("IOLong", [1,2,3,4], True)

    exp_res = [2,4,6,8]
    i = 0
    for e in res:
        if e.get_data() != exp_res[i]:
            raise Exception("Unexpected cmd result. Expected", exp_res,", Received: ",e.get_data())
        i = i+1
    print "[OK]"

#
# Command asynch
#

    print out_prefix, "command asynch scalar (long) ... ",
    req_id = g.command_inout_asynch("IOLong", 123, False, True)
    res = g.command_inout_reply(req_id)
    
    success = True
    for e in res:
        if e.get_data() != 246:
            success = False
            raise Exception("Unexpected cmd result. Expected 246, Received: ",e.get_data())
    if success == True: print "[OK]"            

    print out_prefix, "command asynch scalar (long) with individualized arguments ... ",
    req_id = g.command_inout_asynch("IOLong", [1,2,3,4], False, True)
    res = g.command_inout_reply(req_id)
    
    exp_res = [2,4,6,8]
    i = 0
    for e in res:
        if e.get_data() != exp_res[i]:
            raise Exception("Unexpected cmd result. Expected", exp_res,", Received: ",e.get_data())
        i = i+1
    print "[OK]"

            
if __name__ == '__main__':
    test_group(create_group("plain"))
    test_group(create_group("hierarquical"))
