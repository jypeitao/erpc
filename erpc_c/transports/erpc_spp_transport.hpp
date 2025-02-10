//
// Created by peter on 2/10/25.
//

#ifndef EMBEDDED_RPC_SPP_TRANSPORT_H_
#define EMBEDDED_RPC_SPP_TRANSPORT_H_

#include "erpc_framed_transport.hpp"

#include <erpc_spp_api.h>

namespace erpc {

class SPPTransport final : public FramedTransport
{

public:
    SPPTransport();
    ~SPPTransport() override;

protected:
    /*!
     * @brief This function read data.
     *
     * @param[inout] data Preallocated buffer for receiving data.
     * @param[in] size Size of data to read.
     *
     * @retval #kErpcStatus_Success When data was read successfully.
     * @retval #kErpcStatus_ReceiveFailed When reading data ends with error.
     * @retval #kErpcStatus_ConnectionClosed Peer closed the connection.
     */
    erpc_status_t underlyingReceive(uint8_t *data, uint32_t size) override;

    /*!
     * @brief This function writes data.
     *
     * @param[in] data Buffer to send.
     * @param[in] size Size of data to send.
     *
     * @retval #kErpcStatus_Success When data was written successfully.
     * @retval #kErpcStatus_SendFailed When writing data ends with error.
     * @retval #kErpcStatus_ConnectionClosed Peer closed the connection.
     */
    erpc_status_t underlyingSend(const uint8_t *data, uint32_t size) override;

private:
    BluetoothSppApi *m_api;
};

} // namespace erpc

#endif
