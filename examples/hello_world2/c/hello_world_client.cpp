/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "include/hello_world_client.h"
#include "hello_world_client.hpp"
#include "config.h"

#include "erpc_basic_codec.hpp"
#include "erpc_client_manager.h"
#include "erpc_tcp_transport.hpp"

#include "erpc_error_handler.h"
#include "erpc_spp_transport.hpp"

#include <iostream>
#include <atomic>

using namespace erpcShim;
using namespace erpc;

#ifdef __ANDROID__
#include<android/log.h>
#define LOG_TAG "hello_world_client"
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGE(fmt, ...)                                                                                                 \
    ((void)__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "[%s:%d:%s] " fmt "\n", __FILE_NAME__, __LINE__,           \
                               __FUNCTION__, ##__VA_ARGS__))
#else
#define LOGE(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#endif

namespace {

ClientServerCommon *g_cs_common = nullptr;
Transport *g_transport = nullptr;
std::atomic_bool g_use_tcp{true};
TextService_client *g_client = nullptr;
MessageBufferFactory *g_MsgFactory = nullptr;
BasicCodecFactory *g_BasicCodecFactory = nullptr;

class MyMessageBufferFactoryClient : public MessageBufferFactory
{
public:
    MessageBuffer create() override
    {
        auto *buf = new uint8_t[1024];
        return {buf, 1024};
    }

    void dispose(MessageBuffer *buf) override
    {
        erpc_assert(buf);
        if (*buf)
        {
            delete[] buf->get();
        }
    }
};

Transport *initTcp(const char *ip, uint16_t port, bool asServer)
{
    static std::string sIp;
    sIp = ip;
    auto transport = new TCPTransport(sIp.c_str(), port, asServer);
    Crc16 crc16;
    transport->setCrc16(&crc16);
    auto status = transport->open();
    if (status != kErpcStatus_Success)
    {
        delete transport;
        return nullptr;
    }
    return transport;
}

void destroyTcp(Transport *transport)
{
    auto *t = dynamic_cast<TCPTransport *>(transport);
    t->close();
    delete t;
}

Transport *initSpp(const char *address, bool asServer)
{
    auto transport = new SPPTransport(asServer);
    Crc16 crc16;
    transport->setCrc16(&crc16);

    auto status = transport->init(nullptr);
    if (status != kErpcStatus_Success)
    {
        delete transport;
        return nullptr;
    }

    status = transport->open(address);
    if (status != kErpcStatus_Success)
    {
        transport->destroy();
        delete transport;
        return nullptr;
    }

    return transport;
}

void destroySpp(Transport *transport)
{
    auto *t = dynamic_cast<SPPTransport *>(transport);
    t->close();
    t->destroy();
    delete t;
}

} // namespace

bool hello_world_init_client(bool useTcp, const char *ip, int port)
{
    g_use_tcp = useTcp;
    if (g_use_tcp)
    {
        g_transport = initTcp(ip, port, false);
    }
    else
    {
        g_transport = initSpp(nullptr, false);
    }

    if (g_transport == nullptr)
    {
        return false;
    }

    g_MsgFactory = new MyMessageBufferFactoryClient();
    g_BasicCodecFactory = new BasicCodecFactory();
    g_cs_common = new ClientManager;

    /* Init client manager */
    g_cs_common->setMessageBufferFactory(g_MsgFactory);
    g_cs_common->setTransport(g_transport);
    g_cs_common->setCodecFactory(g_BasicCodecFactory);

    g_client = new TextService_client((ClientManager *)g_cs_common);

    return true;
}

void hello_world_destroy_client()
{
    if (g_transport != nullptr)
    {
        if (g_use_tcp)
        {
            destroyTcp(g_transport);
        }
        else
        {
            destroySpp(g_transport);
        }
        g_transport = nullptr;
    }

    if (g_client != nullptr)
    {
        delete g_client;
        g_client = nullptr;
    }

    if (g_cs_common != nullptr)
    {
        delete g_cs_common;
        g_cs_common = nullptr;
    }

    if (g_BasicCodecFactory != nullptr)
    {
        delete g_BasicCodecFactory;
        g_BasicCodecFactory = nullptr;
    }

    if (g_MsgFactory != nullptr)
    {
        delete g_MsgFactory;
        g_MsgFactory = nullptr;
    }
}

bool hello_world_call_printText(const char *text)
{
    auto ret = false;
    if (g_client != nullptr)
    {
        ret = g_client->printText(text);
    }
    LOGE("ret = %d",ret);
    return ret;
}

void hello_world_call_stopServer()
{
    if (g_client == nullptr)
    {
        return;
    }
    g_client->stopServer();
}