//
// Created by Peter on 2025/2/10.
//

#ifndef COM_DREAMSMART_ERPC_SPP_API_H
#define COM_DREAMSMART_ERPC_SPP_API_H

#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif

#if 0
enum erpc_spp_status
{
    kErpcStatus_Spp_Success = 0,
    kErpcStatus_Spp_Already_Initialized = 1000,
    kErpcStatus_Spp_InitializationFailed,
    kErpcStatus_Spp_OpenFailed,
    kErpcStatus_Spp_Send_Failed,
    kErpcStatus_Spp_Not_initialized,
    kErpcStatus_Spp_Implementation_Not_Found
};
#endif

/**
 * dreamsmart_bluetooth_spp_api_t dreamsmart_bluetooth_spp_api = {
 *      sizeof(dreamsmart_bluetooth_spp_api),
 *      initImpl,
 *      openImpl,
 *      sendImpl,
 *      receiveImpl,
 *      closeImpl,
 *      destroyImpl
 *   };
 */
#define LIB_FILE_NAME "libSppImpl.so"
#define SPP_API_SYM "dreamsmart_bluetooth_spp_api"

// Platform-specific parameters. For example, Android's JVM.
struct PlatformData;

typedef struct
{
    size_t size;

    /*!
     * @brief Initialize resources
     *
     * @param[in] server Whether to act as a server.
     * @param[in] data Platform-specific parameters. For example, Android's JVM. Can be nullptr.
     *
     * @retval kSppStatus_Success When initialization was successful.
     */
    int (*init)(bool server, PlatformData *data);

    /*!
     * @brief Open communication channel
     *
     * When acting as a client, initiate a connection; when acting as a server, wait for a connection.
     * If this function returns successfully,
     * it means that the channel has been established and communication is possible.
     *
     * @param[in] address Server Bluetooth address. Can be nullptr.
     * @retval kErpcStatus_Success When initialization was successful.
     */
    int (*open)(const char *address);

    /*!
     * @brief send data
     *
     * @param[in] data Buffer to send.
     * @param[in] size Size of data to send.
     *
     * @retval kErpcStatus_Success When sending was successful.
     */
    int (*send)(const void *data, size_t size);

    /*!
     * @brief receive data
     *
     * @param[inout] buffer Pre-allocated buffer for receiving data.
     * @param[in] length Size of pre-allocated buffer.
     * @param[out] lengthReceived actual received length.
     *
     * @retval kErpcStatus_Success When receiving successful.
     */
    int (*receive)(void *buffer, size_t length, size_t *lengthReceived);

    /*!
     * @brief Close communication channel
     *
     */
    void (*close)();

    /*!
     * @brief Release resources
     *
     */
    void (*destroy)();
} dreamsmart_bluetooth_spp_api_t;

#ifdef __cplusplus
}
#endif

#endif // COM_DREAMSMART_ERPC_SPP_API_H