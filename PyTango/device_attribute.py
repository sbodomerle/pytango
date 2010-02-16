
from utils import document_method as __document_method
from utils import document_enum as __document_enum
from _PyTango import DeviceAttribute, ExtractAs

def __DeviceAttribute__get_data(self):
    return self.get_data_raw().extract()

def __init_DeviceAttribute():
    DeviceAttribute.ExtractAs = ExtractAs
    pass

def __doc_DeviceAttribute():
    def document_method(method_name, desc, append=True):
        return __document_method(DeviceAttribute, method_name, desc, append)

    DeviceAttribute.__doc__ = """
        This is the fundamental type for RECEIVING data from device attributes.

        It contains several fields. The most important ones depend on the
        ExtractAs method used to theg the value. Normally they are:
        
            - value   : Normal scalar value or numpy array of values.
            - w_value : The write part of the attribute.
            
        See other ExtractAs for different possibilities. There are some more
        fields, these really fixed:
        
            - name        : (str)
            - data_format : (AttrDataFormat) Attribute format
            - quality     : (AttrQuality)
            - time        : (TimeVal)
            - dim_x       : (int) attribute dimension x
            - dim_y       : (int) attribute dimension y
            - w_dim_x     : (int) attribute written dimension x
            - w_dim_y     : (int) attribute written dimension y
            - r_rimension : (tuple<int,int>) Attribute read dimensions.
            - w_dimension : (tuple<int,int>) Attribute written dimensions.
            - nb_read     : (int) attribute read total length
            - nb_written  : (int) attribute written total length
            

        And two methods:
            - get_date
            - get_err_stack
    """

    document_method("get_date", """
    get_date(self) -> TimeVal

            Get the time at which the attribute was read by the server.
            
            Note: It's the same as reading the "time" attribute.
        
        Parameters : None
        Return     : (TimeVal) The attribute read timestamp.
    """ )

    document_method("get_err_stack", """
    get_err_stack(self) -> sequence<DevError>

            Returns the error stack reported by the server when the
            attribute was read.

        Parameters : None
        Return     : (sequence<DevError>)
    """ )

def init_DeviceAttribute():
    __init_DeviceAttribute()
    __doc_DeviceAttribute()
