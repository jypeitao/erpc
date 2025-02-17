/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "hello_world_server.h"
#include "hello_world_server.hpp"
#include "config.h"

#include "erpc_basic_codec.hpp"
#include "erpc_crc16.hpp"
#include "erpc_message_buffer.hpp"
#include "erpc_simple_server.hpp"
#include "erpc_tcp_transport.hpp"

#include "erpc_error_handler.h"
#include "erpc_spp_transport.hpp"
#include "hello_world_client.hpp"

#include <iostream>
#include <atomic>
#include <functional>
#include <thread>

using namespace erpcShim;
using namespace erpc;

namespace {
std::atomic_bool g_server_run{false};
std::atomic_bool g_use_tcp{true};

Transport *g_transport = nullptr;

TextService_interface *g_TextService_interface = nullptr;
TextService_service *g_TextService_service = nullptr;
SimpleServer *g_server = nullptr;

MessageBufferFactory *g_MsgFactory = nullptr;
BasicCodecFactory *g_BasicCodecFactory = nullptr;

std::thread g_thread;

using PrintTextCall = std::function<void(const char *text)>;
PrintTextCall g_call;

void wakeUp()
{
    if (g_use_tcp)
    {
        dynamic_cast<TCPTransport *>(g_transport)->close(true);
    }
    else
    {
        dynamic_cast<SPPTransport *>(g_transport)->close();
    }
}

class TextService : public TextService_interface
{
    bool printText(const char *text) override
    {
        std::cout << text << std::endl;
        if (g_call)
        {
            g_call(text);
        }
        return true;
    }

    void stopServer() override
    {
        g_server_run = false;
        wakeUp();
    }
};

class MyMessageBufferFactory : public MessageBufferFactory
{
public:
    MessageBuffer create() override
    {
        auto *buf = new uint8_t[ERPC_DEFAULT_BUFFER_SIZE];
        return { buf, ERPC_DEFAULT_BUFFER_SIZE };
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

void hello_world_init_server(bool useTcp, printTextCall call)
{
    g_use_tcp = useTcp;
    if (g_use_tcp)
    {
        g_transport = initTcp(ERPC_HOSTNAME, ERPC_PORT, true);
    }
    else
    {
        g_transport = initSpp(nullptr, true);
    }

    if (g_transport == nullptr)
    {
        return;
    }

    g_call = call;

    /* Init eRPC server components */
    //    TCPTransport transport(ERPC_HOSTNAME, ERPC_PORT, true);
    g_MsgFactory = new MyMessageBufferFactory();
    g_BasicCodecFactory = new BasicCodecFactory();
    g_server = new SimpleServer();

    /* Init service implementation and handler */
    g_TextService_interface = new TextService();
    g_TextService_service = new TextService_service(g_TextService_interface);

    /* Setup server */
    g_server->setTransport(g_transport);
    g_server->setCodecFactory(g_BasicCodecFactory);
    g_server->setMessageBufferFactory(g_MsgFactory);

    /* add custom service implementation to the server */
    g_server->addService(g_TextService_service);

    std::cout << "Starting server." << std::endl;
}

void hello_world_start_server()
{
    if (g_server_run)
    {
        return;
    }

    g_thread = std::thread([]() {
        g_server_run = true;

        erpc_status_t status;
        while (g_server_run)
        {
            /* poll for requests */
            status = g_server->poll();

            /* handle error status */
            if (status != kErpcStatus_Success)
            {
                /* print error description */
                erpc_error_handler(status, 0);
            }
        }
    });
}

void hello_world_stop_server()
{
    if (!g_server_run)
    {
        return;
    }

    g_server_run = false;

    wakeUp();

    g_thread.join();
}

void hello_world_destroy_server()
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

    if (g_server != nullptr)
    {
        delete g_server;
        g_server = nullptr;
    }

    if (g_TextService_service != nullptr)
    {
        delete g_TextService_service;
        g_TextService_service = nullptr;
    }

    if (g_TextService_interface != nullptr)
    {
        delete g_TextService_interface;
        g_TextService_interface = nullptr;
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

    g_call = nullptr;

    g_server_run = false;
}
