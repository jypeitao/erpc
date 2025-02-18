//
// Created by peter on 2/10/25.
//

#include "erpc_spp_transport.hpp"

#include <dlfcn.h>
namespace erpc {

#define LOGI(_fmt_, ...)
#define LOGW(_fmt_, ...)
#define LOGE(_fmt_, ...)

SPPTransport::SPPTransport(bool isServer) : m_isServer(isServer) {}

SPPTransport::~SPPTransport()
{
    if (m_spp_handle != nullptr)
    {
        dlclose(m_spp_handle);
    }
}

erpc_status_t SPPTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
    if (m_api == nullptr)
    {
        return kErpcStatus_Spp_Not_initialized;
    }

    size_t length;
    int status = 0;
    status = m_api->receive(data, size, &length);
    // Loop until all requested data is received.
//    while (size > 0U)
//    {
//        status = m_api->receive(data, size, &length);
//        if (status != 0)
//        {
//            break;
//        }
//
//        // TODO length is 0 ?? break ??
//        if (length == 0)
//        {
//            LOGW("wow, receive is zero");
//        }
//
//        size -= length;
//        data += length;
//    }

    return static_cast<erpc_status_t>(status);
}

erpc_status_t SPPTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    if (m_api == nullptr)
    {
        return kErpcStatus_Spp_Not_initialized;
    }
    return static_cast<erpc_status_t>(m_api->send((void *)data, size));
}

erpc_status_t SPPTransport::open()
{
    return open(nullptr);
}

erpc_status_t SPPTransport::open(const char *address)
{
    if (m_api == nullptr)
    {
        return kErpcStatus_Spp_Not_initialized;
    }

    return static_cast<erpc_status_t>(m_api->open(address));
}

void SPPTransport::close()
{
    if (m_api != nullptr)
    {
        m_api->close();
    }
}

erpc_status_t SPPTransport::init(PlatformData *data)
{
    if (!load())
    {
        return kErpcStatus_Spp_Implementation_Not_Found;
    }

    return static_cast<erpc_status_t>(m_api->init(m_isServer, data));
}

void SPPTransport::destroy()
{
    if (m_api != nullptr)
    {
        m_api->destroy();
    }
}

bool SPPTransport::load()
{

    // Android其它模块先加载并做初始化工作
    m_spp_handle = dlopen(LIB_FILE_NAME, RTLD_NOLOAD);
    if (m_spp_handle == nullptr)
    {
        LOGE("can not find spp lib, %s", dlerror());
        return false;
    }

    m_api = (dreamsmart_bluetooth_spp_api_t *)dlsym(m_spp_handle, SPP_API_SYM);
    if (!m_api)
    {
        LOGE("failed to load symbol from spp library");
        dlclose(m_spp_handle);
        m_spp_handle = nullptr;
        return false;
    }

    return true;
}

} // namespace erpc