
from _PyTango import Connection, DeviceData, __CallBackAutoDie, CmdArgType
from _PyTango import DeviceProxy, Database
from utils import document_method as __document_method
import operator

def __CallBackAutoDie__cmd_ended_aux(fn):
    def __new_fn(cmd_done_event):
        try:
            cmd_done_event.argout = cmd_done_event.argout_raw.extract()
        except Exception:
            pass
        return fn(cmd_done_event)

def __get_command_inout_param(self, cmd_name, cmd_param=None):
    if cmd_param is None:
        return DeviceData()
    
    if isinstance(cmd_param, DeviceData):
        return cmd_param

    if isinstance(self, DeviceProxy):
        # This is not part of 'Connection' interface, but
        # DeviceProxy only.
        info = self.command_query(cmd_name)
        param = DeviceData()
        param.insert(info.in_type, cmd_param)
        return param
    elif isinstance(self, Database):
        # I just try to guess types DevString and DevVarStringArray
        # as they are used for Database
        param = DeviceData()
        if isinstance(cmd_param, str):
            param.insert(CmdArgType.DevString, cmd_param)
            return param
        elif operator.isSequenceType(cmd_param) and all([isinstance(x,str) for x in cmd_param]):
            param.insert(CmdArgType.DevVarStringArray, cmd_param)
            return param
        else:
            raise TypeError("command_inout() parameter must be a DeviceData object or a string or a sequence of strings")
    else:
        raise TypeError("command_inout() parameter must be a DeviceData object.")

def __Connection__command_inout(self, name, *args, **kwds):
    """
    command_inout( self, cmd_name, cmd_param=None) -> any

            Execute a command on a device.
            
        Parameters :
                - cmd_name  : (str) Command name.
                - cmd_param : (any) It should be a value of the type expected by the
                              command or a DeviceData object with this value inserted.
                              It can be ommited if the command should not get any argument.
        Return     : The result of the command. The type depends on the command. It may be None.

        Throws     : ConnectionFailed, CommunicationFailed, DeviceUnlocked, DevFailed from device 
    """
    r = Connection.command_inout_raw(self, name, *args, **kwds)
    if isinstance(r, DeviceData):
        try:
            return r.extract()
        except Exception:
            return None
    else:
        return r

def __Connection__command_inout_raw(self, cmd_name, cmd_param = None):
    """
    command_inout_raw( self, cmd_name, cmd_param=None) -> DeviceData

            Execute a command on a device.
        
        Parameters :
                - cmd_name  : (str) Command name.
                - cmd_param : (any) It should be a value of the type expected by the
                              command or a DeviceData object with this value inserted.
                              It can be ommited if the command should not get any argument.
        Return     : A DeviceData object.

        Throws     : ConnectionFailed, CommunicationFailed, DeviceUnlocked, DevFailed from device 
    """
    param = __get_command_inout_param(self, cmd_name, cmd_param)
    return self.__command_inout(cmd_name, param)



def __Connection__command_inout_asynch(self, cmd_name, *args):
    """
    command_inout_asynch(self, cmd_name, cmd_param=None, forget=False) -> id
    
            Execute asynchronously (polling model) a command on a device
            
        Parameters :
                - cmd_name  : (str) Command name.
                - cmd_param : (any) It should be a value of the type expected by the
                              command or a DeviceData object with this value inserted.
                              It can be ommited if the command should not get any argument.
                              If the command sould get no argument and you want
                              to set the 'forget' param, use None for cmd_param.
                - forget    : (bool) If this flag is set to true, this means that the client
                              does not care at all about the server answer and will even
                              not try to get it. A false default value is provided. Please,
                              note that device re-connection will not take place (in case
                              it is needed) if the fire and forget mode is used. Therefore,
                              an application using only fire and forget requests is not able
                              to automatically re-connnect to device.
        Return     : (int) This call returns an asynchronous call identifier which is
                     needed to get the command result (see command_inout_reply)

        Throws     : ConnectionFailed, TypeError, anything thrown by command_query
        
    command_inout_asynch( self, cmd_name, callback) -> None
    command_inout_asynch( self, cmd_name, cmd_param, callback) -> None
    
            Execute asynchronously (callback model) a command on a device.
            
        Parameters :
                - cmd_name  : (str) Command name.
                - cmd_param : (any)It should be a value of the type expected by the
                              command or a DeviceData object with this value inserted.
                              It can be ommited if the command should not get any argument.
                - callback  : Any callable object (function, lambda...) or any oject
                              with a method named "cmd_ended".
        Return     : None

        Throws     : ConnectionFailed, TypeError, anything thrown by command_query
    """
    if len(args) == 0: # command_inout_asynch()
        argin = DeviceData()
        forget = False
        return self.__command_inout_asynch_id(cmd_name, argin, forget)
    elif len(args) == 1:
        if callable(args[0]): # command_inout_asynch(lambda)
            cb = __CallBackAutoDie()
            cb.cmd_ended = __CallBackAutoDie__cmd_ended_aux(args[0])
            argin = __get_command_inout_param(self, cmd_name)
            return self.__command_inout_asynch_cb(cmd_name, argin, cb)
        elif hasattr(args[0], 'cmd_ended'): # command_inout_asynch(Cbclass)
            cb = __CallBackAutoDie()
            cb.cmd_ended = __CallBackAutoDie__cmd_ended_aux(args[0].cmd_ended)
            argin = __get_command_inout_param(self, cmd_name)
            return self.__command_inout_asynch_cb(cmd_name, argin, cb)
        else: # command_inout_asynch(value)
            argin = __get_command_inout_param(self, cmd_name, args[0])
            forget = False
            return self.__command_inout_asynch_id(cmd_name, argin, forget)
    elif len(args) == 2:
        if callable(args[1]): #command_inout_asynch( value, lambda)
            cb = __CallBackAutoDie()
            cb.cmd_ended = __CallBackAutoDie__cmd_ended_aux(args[1])
            argin = __get_command_inout_param(self, cmd_name, args[0])
            return self.__command_inout_asynch_cb(cmd_name, argin, cb)
        elif hasattr(args[1], 'cmd_ended'): #command_inout_asynch(value, cbClass)
            cb = __CallBackAutoDie()
            cb.cmd_ended = __CallBackAutoDie__cmd_ended_aux(args[1].cmd_ended)
            argin = __get_command_inout_param(self, cmd_name, args[0])
            return self.__command_inout_asynch_cb(cmd_name, argin, cb)
        else: # command_inout_asynch(value, forget)
            argin = __get_command_inout_param(self, cmd_name, args[0])
            forget = bool(args[1])
            return self.__command_inout_asynch_id(cmd_name, argin, forget)
    else:
        raise TypeError("Wrong number of attributes!")

def __Connection__command_inout_reply(self, idx, timeout=None):
    """
    command_inout_reply(self, id) -> DeviceData
    
            Check if the answer of an asynchronous command_inout is arrived
            (polling model). If the reply is arrived and if it is a valid
            reply, it is returned to the caller in a DeviceData object. If
            the reply is an exception, it is re-thrown by this call. An
            exception is also thrown in case of the reply is not yet arrived.

        Parameters :
            - id      : (int) Asynchronous call identifier.
        Return     : (DeviceData)
        Throws     : AsynCall, AsynReplyNotArrived, CommunicationFailed, DevFailed from device

    command_inout_reply(self, id, timeout) -> DeviceData
    
            Check if the answer of an asynchronous command_inout is arrived
            (polling model). id is the asynchronous call identifier. If the
            reply is arrived and if it is a valid reply, it is returned to
            the caller in a DeviceData object. If the reply is an exception,
            it is re-thrown by this call. If the reply is not yet arrived,
            the call will wait (blocking the process) for the time specified
            in timeout. If after timeout milliseconds, the reply is still
            not there, an exception is thrown. If timeout is set to 0, the
            call waits until the reply arrived.

        Parameters :
            - id      : (int) Asynchronous call identifier.
            - timeout : (int)
        Return     : (DeviceData)
        Throws     : AsynCall, AsynReplyNotArrived, CommunicationFailed, DevFailed from device
    """
    if timeout is None:
        r = self.command_inout_reply_raw(idx)
    else:
        r = self.command_inout_reply_raw(idx, timeout)
    
    if isinstance(r, DeviceData):
        try:
            return r.extract()
        except Exception:
            return None
    else:
        return r
    
def __init_Connection():
    Connection.command_inout_raw = __Connection__command_inout_raw
    Connection.command_inout = __Connection__command_inout
    Connection.command_inout_asynch = __Connection__command_inout_asynch
    Connection.command_inout_reply = __Connection__command_inout_reply
    
def __doc_Connection():
    def document_method(method_name, desc, append=True):
        return __document_method(Connection, method_name, desc, append)

    Connection.__doc__ = """
        The abstract Connection class for DeviceProxy. Not to be initialized directly.
    """

    document_method("dev_name", """
    dev_name(self) -> str
    
            Return the device name as it is stored locally

        Parameters : None
        Return     : (str)
    """)

    document_method("get_db_host", """
    get_db_host(self) -> str
    
            Returns a string with the database host.

        Parameters : None
        Return     : (str)

        New in PyTango 7.0.0
    """)

    document_method("get_db_port", """
    get_db_port(self) -> str
    
            Returns a string with the database port.

        Parameters : None
        Return     : (str)

        New in PyTango 7.0.0
    """)

    document_method("get_db_port_num", """
    get_db_port_num(self) -> int
    
            Returns an integer with the database port.

        Parameters : None
        Return     : (int)

        New in PyTango 7.0.0
    """)

    document_method("get_from_env_var", """
    get_from_env_var(self) -> bool
    
            Returns True if determined by environment variable or
            False otherwise

        Parameters : None
        Return     : (bool)

        New in PyTango 7.0.0
    """)


    document_method("connect", """
    connect(self, corba_name) -> None
    
            Creates a connection to a TANGO device using it's stringified
            CORBA reference i.e. IOR or corbaloc.

        Parameters :
            - corba_name : (str) Name of the CORBA object
        Return     : None

        New in PyTango 7.0.0
    """)

    document_method("reconnect", """
    reconnect(self, db_used) -> None
    
            Reconnecto to a CORBA object.

        Parameters :
            - db_used : (bool) Use thatabase
        Return     : None

        New in PyTango 7.0.0
    """)

    document_method("get_idl_version", """
    get_idl_version(self) -> int
    
            Get the version of the Tango Device interface implemented
            by the device

        Parameters : None
        Return     : (int)
    """)

    document_method("set_timeout_millis", """
    set_timeout_millis(self, timeout) -> None
    
            Set client side timeout for device in milliseconds. Any method
            which takes longer than this time to execute will throw an
            exception

        Parameters :
            - timeout : integer value of timeout in milliseconds
        Return     : None
        Example    :
                    dev.set_timeout_millis(1000)
    """)

    document_method("get_timeout_millis", """
    get_timeout_millis(self) -> int
    
            Get the client side timeout in milliseconds

        Parameters : None
        Return     : (int)
    """)

    document_method("get_source", """
    get_source(self) -> DevSource
    
            Get the data source(device, polling buffer, polling buffer
            then device) used by command_inout or read_attribute methods

        Parameters : None
        Return     : (DevSource)
        Example    :
                    source = dev.get_source()
                    if source == DevSource.CACHE_DEV : ...
    """)

    document_method("set_source", """
    set_source(self, source) -> None
    
            Set the data source(device, polling buffer, polling buffer
            then device) for command_inout and read_attribute methods.

        Parameters :
            - source: (DevSource) constant.
        Return     : None
        Example    :
                    dev.set_source(DevSource.CACHE_DEV)
    """)

    document_method("get_transparency_reconnection", """
    get_transparency_reconnection(self) -> bool
    
            Returns the device transparency reconnection flag.

        Parameters : None
        Return     : (bool) True if transparency reconnection is set
                            or False otherwise
    """)

    document_method("set_transparency_reconnection", """
    set_transparency_reconnection(self, yesno) -> None
    
            Set the device transparency reconnection flag

        Parameters :
            "    - val : (bool) True to set transparency reconnection
            "                   or False otherwise
        Return     : None
    """)

    document_method("command_inout_reply_raw", """
    command_inout_reply_raw(self, id) -> DeviceData
    
            Check if the answer of an asynchronous command_inout is arrived
            (polling model). If the reply is arrived and if it is a valid
            reply, it is returned to the caller in a DeviceData object. If
            the reply is an exception, it is re-thrown by this call. An
            exception is also thrown in case of the reply is not yet arrived.

        Parameters :
            - id      : (int) Asynchronous call identifier.
        Return     : (DeviceData)
        Throws     : AsynCall, AsynReplyNotArrived, CommunicationFailed, DevFailed from device
    """)

    document_method("command_inout_reply_raw", """
    command_inout_reply_raw(self, id, timeout) -> DeviceData
    
            Check if the answer of an asynchronous command_inout is arrived
            (polling model). id is the asynchronous call identifier. If the
            reply is arrived and if it is a valid reply, it is returned to
            the caller in a DeviceData object. If the reply is an exception,
            it is re-thrown by this call. If the reply is not yet arrived,
            the call will wait (blocking the process) for the time specified
            in timeout. If after timeout milliseconds, the reply is still
            not there, an exception is thrown. If timeout is set to 0, the
            call waits until the reply arrived.

        Parameters :
            - id      : (int) Asynchronous call identifier.
            - timeout : (int)
        Return     : (DeviceData)
        Throws     : AsynCall, AsynReplyNotArrived, CommunicationFailed, DevFailed from device
    """)

    #//
    #// Asynchronous methods
    #//

    document_method("get_asynch_replies", """
    get_asynch_replies(self) -> None
    
            Try to obtain data returned by a command asynchronously
            requested. This method does not block if the reply has not yet
            arrived. It fires callback for already arrived replies.
            
        Parameters : None
        Return     : None

        New in PyTango 7.0.0
    """)

    document_method("get_asynch_replies", """
    get_asynch_replies(self, call_timeout) -> None
    
            Try to obtain data returned by a command asynchronously
            requested. This method blocks for the specified timeout if the
            reply is not yet arrived. This method fires callback when the
            reply arrived. If the timeout is set to 0, the call waits
            undefinitely for the reply
            
        Parameters :
            - call_timeout : (int) timeout in miliseconds
        Return     : None

        New in PyTango 7.0.0
    """)

    document_method("cancel_asynch_request", """
    cancel_asynch_request(self, id ) -> None

            Cancel a running asynchronous request
            
            This is a client side call. Obviously, the call cannot be
            aborted while it is running in the device.
            
        Parameters :
            - id : The asynchronous call identifier
        Return     : None

            New in PyTango 7.0.0
    """)

    document_method("cancel_all_polling_asynch_request", """
    cancel_all_polling_asynch_request(self) -> None
    
            Cancel all running asynchronous request
            
            This is a client side call. Obviously, the calls cannot be
            aborted while it is running in the device.
            
        Parameters : None
        Return     : None

        New in PyTango 7.0.0
    """)

    #//
    #// Control access related methods
    #//
        
    document_method("get_access_control", """
    get_access_control(self) -> AccessControlType
    
            Returns the current access control type
            
        Parameters : None
        Return     : (AccessControlType) The current access control type

        New in PyTango 7.0.0
    """)

    document_method("set_access_control", """
    set_access_control(self, acc) -> None
            Sets the current access control type
            
        Parameters :
            - acc: (AccessControlType) the type of access
                   control to set
        Return     : None

        New in PyTango 7.0.0
    """)

def init_Connection():
    __init_Connection()
    __doc_Connection()
