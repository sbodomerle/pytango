
from utils import document_method as __document_method
from _PyTango import GroupReply
from _PyTango import GroupCmdReply
from _PyTango import GroupAttrReply

def __GroupCmdReply__get_data(self):
    return self.get_data_raw().extract()

def __init_GroupReply():
    GroupCmdReply.get_data = __GroupCmdReply__get_data

def __doc_GroupReply():
    def document_method(method_name, desc, append=True):
        return __document_method(GroupReply, method_name, desc, append)

    GroupReply.__doc__ = """
        This is the base class for the result of an operation on a
        PyTangoGroup, being it a write attribute, read attribute, or
        command inout operation.

        It has some trivial common operations:

            - has_failed(self) -> bool
            - group_element_enabled(self) ->bool
            - dev_name(self) -> str
            - obj_name(self) -> str
            - get_err_stack(self) -> DevErrorList
    """

    __document_method(GroupCmdReply, "get_data", """
    get_data(self) -> any

            Get the actual value stored in the GroupCmdRply, the command
            output value.
            It's the same as self.get_data_raw().extract()

        Parameters : None
        Return     : (any) Whatever is stored there, or None.
    """ )

    __document_method(GroupCmdReply, "get_data_raw", """
    get_data_raw(self) -> any

            Get the DeviceData containing the output parameter
            of the command.

        Parameters : None
        Return     : (DeviceData) Whatever is stored there, or None.
    """ )

    __document_method(GroupAttrReply, "get_data", """
    get_data(self, extract_as=ExtractAs.Numpy) -> DeviceAttribute

            Get the DeviceAttribute.

        Parameters :
            - extract_as : (ExtractAs)

        Return     : (DeviceAttribute) Whatever is stored there, or None.
    """ )


def init_GroupReply():
    __init_GroupReply()
    __doc_GroupReply()
