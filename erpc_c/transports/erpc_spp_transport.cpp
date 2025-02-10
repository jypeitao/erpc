//
// Created by peter on 2/10/25.
//

#include "erpc_spp_transport.hpp"
#include "erpc_spp_api.h"

#include <iostream>
#include <dlfcn.h>
namespace erpc {
static constexpr auto soName = "libSppImpl.so";

#define LOGI(_fmt_, ...)
#define LOGW(_fmt_, ...)
#define LOGE(_fmt_, ...)

SPPTransport::SPPTransport()
{

    // TODO already exit
    void *handle = dlopen(soName, RTLD_LAZY);
    if (!handle)
    {
        std::cerr << "Failed to load libbluetooth_spp.so: " << dlerror() << std::endl;
        return;
    }

    typedef BluetoothSppApi *(*CreateInstanceFunc)();
    const auto createInstance = static_cast<CreateInstanceFunc>(dlsym(handle, createSppApi_symbol));
    if (!createInstance)
    {
        std::cerr << "Failed to find createInstance: " << dlerror() << std::endl;
        dlclose(handle);
        return;
    }

    m_api = createInstance();
}
SPPTransport::~SPPTransport()
{
    destroySppApi(m_api);
}

erpc_status_t SPPTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
    m_api->receive(data, size);
    return kErpcStatus_Success;
}

erpc_status_t SPPTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    m_api->send(data, size);
    return kErpcStatus_Success;
}

} // namespace erpc
