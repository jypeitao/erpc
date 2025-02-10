//
// Created by Peter on 2025/2/10.
//

#ifndef EMBEDDED_RPC_SPP_API_H_
#define EMBEDDED_RPC_SPP_API_H_

#include<string>

namespace erpc {

class BluetoothSppApi {

public:
    virtual ~BluetoothSppApi();

    virtual bool connect(const std::string &deviceAddress) = 0;
    virtual bool startServer() = 0;
    virtual size_t send(const void *data, size_t size) = 0;
    virtual size_t receive(void *data, size_t size) = 0;
    virtual void close() = 0;
};

}

#define createSppApi_symbol "createSppApi"
#define destroySppApi_symbol "destroySppApi"

extern "C" erpc::BluetoothSppApi* createSppApi();
extern "C" void destroySppApi(erpc::BluetoothSppApi *api);

#endif //_EMBEDDED_RPC__SPP_API_H_

