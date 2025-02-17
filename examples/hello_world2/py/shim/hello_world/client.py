#
# Generated by erpcgen 1.13.0 on Fri Feb 14 13:57:33 2025.
#
# AUTOGENERATED - DO NOT EDIT
#

import erpc
from . import common, interface

# Client for TextService
class TextServiceClient(interface.ITextService):
    def __init__(self, manager):
        super(TextServiceClient, self).__init__()
        self._clientManager = manager

    def printText(self, text):
        # Build remote function invocation message.
        request = self._clientManager.create_request()
        codec = request.codec
        codec.start_write_message(erpc.codec.MessageInfo(
                type=erpc.codec.MessageType.kInvocationMessage,
                service=self.SERVICE_ID,
                request=self.PRINTTEXT_ID,
                sequence=request.sequence))
        if text is None:
            raise ValueError("text is None")
        codec.write_string(text)

        # Send request and process reply.
        self._clientManager.perform_request(request)
        _result = codec.read_bool()
        return _result

    def stopServer(self):
        # Build remote function invocation message.
        request = self._clientManager.create_request(isOneway=True)
        codec = request.codec
        codec.start_write_message(erpc.codec.MessageInfo(
                type=erpc.codec.MessageType.kOnewayMessage,
                service=self.SERVICE_ID,
                request=self.STOPSERVER_ID,
                sequence=request.sequence))

        # Send request.
        self._clientManager.perform_request(request)


