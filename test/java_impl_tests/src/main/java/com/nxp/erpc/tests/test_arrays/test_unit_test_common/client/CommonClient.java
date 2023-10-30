/** 
 * Generated by erpcgen 1.12.0 on Mon Oct 30 10:47:32 2023.
 * 
 * AUTOGENERATED - DO NOT EDIT
 */
 
package com.nxp.erpc.tests.test_arrays.test_unit_test_common.client;

import com.nxp.erpc.tests.test_arrays.test_unit_test_common.interfaces.ICommon;

import com.nxp.erpc.tests.test_arrays.test_unit_test_common.common.enums.*;


import com.nxp.erpc.auxiliary.MessageType;
import com.nxp.erpc.auxiliary.MessageInfo;
import com.nxp.erpc.auxiliary.RequestContext;
import com.nxp.erpc.auxiliary.Reference;
import com.nxp.erpc.auxiliary.Utils;
import com.nxp.erpc.client.ClientManager;
import com.nxp.erpc.codec.Codec;

import java.util.ArrayList;
import java.util.List;

public class CommonClient implements ICommon {
    private final ClientManager clientManager;

    public CommonClient(ClientManager clientManager) {
        this.clientManager = clientManager;
    }


    @Override
    public void quit() {
        // Create request
        RequestContext request = clientManager.createRequest(true);
        Codec codec = request.codec();

        // Write message info
        codec.startWriteMessage(new MessageInfo(
                MessageType.kOnewayMessage,
                this.SERVICE_ID,
                this.QUIT_ID,
                request.sequence()));

        // Write message data

        // Perform request
        clientManager.performRequest(request);

    }

    @Override
    public int getServerAllocated() {
        // Create request
        RequestContext request = clientManager.createRequest(false);
        Codec codec = request.codec();

        // Write message info
        codec.startWriteMessage(new MessageInfo(
                MessageType.kInvocationMessage,
                this.SERVICE_ID,
                this.GETSERVERALLOCATED_ID,
                request.sequence()));

        // Write message data

        // Perform request
        clientManager.performRequest(request);

        // Read out parameters

        // Read return value
        int _result = codec.readInt32();
        return _result;
    }
}
