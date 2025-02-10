//
// Created by peter on 2/10/25.
//

#include "erpc_spp_transport.hpp"
#include <jni.h>
namespace erpc {

SPPTransport::SPPTransport() {}

erpc_status_t SPPTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
    return kErpcStatus_CrcCheckFailed;
}

erpc_status_t SPPTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    return kErpcStatus_CrcCheckFailed;
}

}
