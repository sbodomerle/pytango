.. currentmodule:: PyTango

Green mode
----------

PyTango supports cooperative green Tango objects. Since version 8.1 two *green*
modes have been added: :obj:`~PyTango.GreenMode.Futures` and
:obj:`~PyTango.GreenMode.Gevent`.

The :obj:`~PyTango.GreenMode.Futures` uses the standard python module
:mod:`concurrent.futures`.
The :obj:`~PyTango.GreenMode.Gevent` mode uses the well known gevent_ library.

Currently, in version 8.1, only :class:`DeviceProxy` has been modified to work
in a green cooperative way. If the work is found to be useful, the same can
be implemented in the future for :class:`AttributeProxy`, :class:`Database`, 
:class:`Group` or even in the server side.

You can set the PyTango green mode at a global level. Set the environment
variable :envvar:`PYTANGO_GREEN_MODE` to either *futures* or *gevent*
(case incensitive). If this environment variable is not defined the PyTango
global green mode defaults to *Synchronous*.

You can also change the active global green mode at any time in your program::

    >>> from PyTango import DeviceProxy, GreenMode
    >>> from PyTango import set_green_mode, get_green_mode

    >>> get_green_mode()
    PyTango.GreenMode.Synchronous    

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> dev.get_green_mode()
    PyTango.GreenMode.Synchronous    

    >>> set_green_mode(GreenMode.Futures)
    >>> get_green_mode()
    PyTango.GreenMode.Futures

    >>> dev.get_green_mode()
    PyTango.GreenMode.Futures

As you can see by the example, the global green mode will affect any previously
created :class:`DeviceProxy` using the default *DeviceProxy* constructor
parameters.

You can specificy green mode on a :class:`DeviceProxy` at creation time.
You can also change the green mode at any time::

    >>> from PyTango.futures import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> dev.get_green_mode()
    PyTango.GreenMode.Futures

    >>> dev.set_green_mode(GreenMode.Synchronous)
    >>> dev.get_green_mode()
    PyTango.GreenMode.Synchronous

futures mode
~~~~~~~~~~~~

Using :mod:`concurrent.futures` cooperative mode in PyTango is relatively easy::

    >>> from PyTango.futures import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> dev.get_green_mode()
    PyTango.GreenMode.Futures

    >>> print(dev.state())
    RUNNING

The :func:`PyTango.futures.DeviceProxy` API is exactly the same as the standard
:class:`~PyTango.DeviceProxy`. The difference is in the semantics of the methods
that involve synchronous network calls (constructor included) which may block
the execution for a relatively big amount of time.
The list of methods that have been modified to accept *futures* semantics are,
on the :func:`PyTango.futures.DeviceProxy`:

* Constructor
* :meth:`~DeviceProxy.state`
* :meth:`~DeviceProxy.status`
* :meth:`~DeviceProxy.read_attribute` 
* :meth:`~DeviceProxy.write_attribute`
* :meth:`~DeviceProxy.write_read_attribute`
* :meth:`~DeviceProxy.read_attributes`
* :meth:`~DeviceProxy.write_attributes`
* :meth:`~DeviceProxy.ping`

So how does this work in fact? I see no difference from using the *standard* 
:class:`~PyTango.DeviceProxy`.
Well, this is, in fact, one of the goals: be able to use a *futures* cooperation
without changing the API. Behind the scenes the methods mentioned before have
been modified to be able to work cooperatively.

All of the above methods have been boosted with two extra keyword arguments
*wait* and *timeout* which allow to fine tune the behaviour.
The *wait* parameter is by default set to `True` meaning wait for the request
to finish (the default semantics when not using green mode).
If *wait* is set to `True`, the timeout determines the maximum time to wait for
the method to execute. The default is `None` which means wait forever. If *wait*
is set to `False`, the *timeout* is ignored.

If *wait* is set to `True`, the result is the same as executing the 
*standard* method on a :class:`~PyTango.DeviceProxy`.
If, *wait* is set to `False`, the result will be a 
:class:`concurrent.futures.Future`. In this case, to get the actual value
you will need to do something like::

    >>> from PyTango.futures import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> result = dev.state(wait=False)
    >>> result
    <Future at 0x16cb310 state=pending>

    >>> # this will be the blocking code
    >>> state = result.result()
    >>> print(state)
    RUNNING

Here is another example using :meth:`~DeviceProxy.read_attribute`::

    >>> from PyTango.futures import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> result = dev.read_attribute('wave', wait=False)
    >>> result
    <Future at 0x16cbe50 state=pending>

    >>> dev_attr = result.result()
    >>> print(dev_attr)
    DeviceAttribute[
    data_format = PyTango.AttrDataFormat.SPECTRUM
          dim_x = 256
          dim_y = 0
     has_failed = False
       is_empty = False
           name = 'wave'
        nb_read = 256
     nb_written = 0
        quality = PyTango.AttrQuality.ATTR_VALID
    r_dimension = AttributeDimension(dim_x = 256, dim_y = 0)
           time = TimeVal(tv_nsec = 0, tv_sec = 1383923329, tv_usec = 451821)
           type = PyTango.CmdArgType.DevDouble
          value = array([ -9.61260664e-01,  -9.65924853e-01,  -9.70294813e-01,
            -9.74369212e-01,  -9.78146810e-01,  -9.81626455e-01,
            -9.84807087e-01,  -9.87687739e-01,  -9.90267531e-01,
            ...
            5.15044507e-1])
        w_dim_x = 0
        w_dim_y = 0
    w_dimension = AttributeDimension(dim_x = 0, dim_y = 0)
        w_value = None]

gevent mode
~~~~~~~~~~~

.. warning::
   Before using gevent mode please note that at the time of writing this
   documentation, *PyTango.gevent* requires the latest version 1.0 of
   gevent (which has been released the day before :-P). Also take into 
   account that gevent_ 1.0 is *not* available on python 3.
   Please consider using the *Futures* mode instead.

Using gevent_ cooperative mode in PyTango is relatively easy::

    >>> from PyTango.gevent import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> dev.get_green_mode()
    PyTango.GreenMode.Gevent

    >>> print(dev.state())
    RUNNING

The :func:`PyTango.gevent.DeviceProxy` API is exactly the same as the standard
:class:`~PyTango.DeviceProxy`. The difference is in the semantics of the methods
that involve synchronous network calls (constructor included) which may block
the execution for a relatively big amount of time.
The list of methods that have been modified to accept *gevent* semantics are,
on the :func:`PyTango.gevent.DeviceProxy`:

* Constructor
* :meth:`~DeviceProxy.state`
* :meth:`~DeviceProxy.status`
* :meth:`~DeviceProxy.read_attribute` 
* :meth:`~DeviceProxy.write_attribute`
* :meth:`~DeviceProxy.write_read_attribute`
* :meth:`~DeviceProxy.read_attributes`
* :meth:`~DeviceProxy.write_attributes`
* :meth:`~DeviceProxy.ping`

So how does this work in fact? I see no difference from using the *standard* 
:class:`~PyTango.DeviceProxy`.
Well, this is, in fact, one of the goals: be able to use a gevent cooperation
without changing the API. Behind the scenes the methods mentioned before have
been modified to be able to work cooperatively with other greenlets.

All of the above methods have been boosted with two extra keyword arguments
*wait* and *timeout* which allow to fine tune the behaviour.
The *wait* parameter is by default set to `True` meaning wait for the request
to finish (the default semantics when not using green mode).
If *wait* is set to `True`, the timeout determines the maximum time to wait for
the method to execute. The default timeout is `None` which means wait forever.
If *wait* is set to `False`, the *timeout* is ignored.

If *wait* is set to `True`, the result is the same as executing the 
*standard* method on a :class:`~PyTango.DeviceProxy`.
If, *wait* is set to `False`, the result will be a 
:class:`gevent.event.AsyncResult`. In this case, to get the actual value
you will need to do something like::

    >>> from PyTango.gevent import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> result = dev.state(wait=False)
    >>> result
    <gevent.event.AsyncResult at 0x1a74050>

    >>> # this will be the blocking code
    >>> state = result.get()
    >>> print(state)
    RUNNING

Here is another example using :meth:`~DeviceProxy.read_attribute`::

    >>> from PyTango.gevent import DeviceProxy

    >>> dev = DeviceProxy("sys/tg_test/1")
    >>> result = dev.read_attribute('wave', wait=False)
    >>> result
    <gevent.event.AsyncResult at 0x1aff54e>

    >>> dev_attr = result.get()
    >>> print(dev_attr)
    DeviceAttribute[
    data_format = PyTango.AttrDataFormat.SPECTRUM
          dim_x = 256
          dim_y = 0
     has_failed = False
       is_empty = False
           name = 'wave'
        nb_read = 256
     nb_written = 0
        quality = PyTango.AttrQuality.ATTR_VALID
    r_dimension = AttributeDimension(dim_x = 256, dim_y = 0)
           time = TimeVal(tv_nsec = 0, tv_sec = 1383923292, tv_usec = 886720)
           type = PyTango.CmdArgType.DevDouble
          value = array([ -9.61260664e-01,  -9.65924853e-01,  -9.70294813e-01,
            -9.74369212e-01,  -9.78146810e-01,  -9.81626455e-01,
            -9.84807087e-01,  -9.87687739e-01,  -9.90267531e-01,
            ...
            5.15044507e-1])
        w_dim_x = 0
        w_dim_y = 0
    w_dimension = AttributeDimension(dim_x = 0, dim_y = 0)
        w_value = None]

.. note::
   due to the internal workings of gevent, setting the *wait* flag to 
   `True` (default) doesn't prevent other greenlets from running in *parallel*.
   This is, in fact, one of the major bonus of working with :mod:`gevent` when
   compared with :mod:`concurrent.futures`

