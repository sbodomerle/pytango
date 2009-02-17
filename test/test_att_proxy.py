from PyTango import *
import time

att = AttributeProxy('tangotest/1/1/long_scalar')
att.ping()

#generic methods
att.name()
att.state()
att.status()
dev = att.get_device_proxy()

#property related methods
att.get_property(['prova1'])
att.put_property({'prova1':'1', 'prova2':'2'})
att.get_property(['prova1', 'prova2'])
att.delete_property(['prova2'])
att.get_property(['prova2'])

#attribute methods
info = att.get_config()
print info
#this doesn't work because AttributeInfo is set as read only:
#should we change this?
#info.description = 'mytest'
#att.set_config(info)
#info = att.get_config()
#print info

att.read().value

val = AttributeValue()
val.value = 1000
att.write(val)

#history
h = att.history(10)
for el in h:
 print el.value.value


#polling related
att.is_polled()
att.poll(2000)
att.get_poll_period()
att.stop_poll()
att.is_polled()
att.poll(1000)
att.is_polled()

#asynch calls (write doesn't work)
class PyCallback:
 def push_event(self,event):
  if not event.err:
   print event.attr_name, event.attr_value.value
  else:
   print event.errors
 def attr_read(self,event):
  if not event.err:
   for el in event.argout:
    print el.value
  else:
   print event.errors

  
cb = PyCallback();

ApiUtil().set_asynch_cb_sub_model(cb_sub_model.PUSH_CALLBACK)

id = att.read_asynch()
att.read_reply(id).value

att.read_asynch(cb)

#events related methods 
ev = att.subscribe_event(EventType.CHANGE, cb, [])
time.sleep(5)
att.unsubscribe_event(ev)


