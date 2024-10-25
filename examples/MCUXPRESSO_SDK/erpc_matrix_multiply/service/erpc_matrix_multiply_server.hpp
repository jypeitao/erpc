/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Generated by erpcgen 1.13.0 on Thu Apr 18 09:37:38 2024.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_erpc_matrix_multiply_server_hpp_)
#define _erpc_matrix_multiply_server_hpp_

#include "erpc_matrix_multiply_interface.hpp"

#include "erpc_server.hpp"
#include "erpc_codec.hpp"

#if 11300 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


namespace erpcShim
{

/*!
 * @brief Service subclass for MatrixMultiplyService.
 */
class MatrixMultiplyService_service : public erpc::Service
{
public:
    MatrixMultiplyService_service(MatrixMultiplyService_interface *_MatrixMultiplyService_interface);

    virtual ~MatrixMultiplyService_service();

    /*! @brief return service interface handler. */
    MatrixMultiplyService_interface* getHandler(void);

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, erpc::Transport * transport);

private:
    MatrixMultiplyService_interface *m_handler;
    /*! @brief Server shim for erpcMatrixMultiply of MatrixMultiplyService interface. */
    erpc_status_t erpcMatrixMultiply_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, erpc::Transport * transport, uint32_t sequence);
};

} // erpcShim


#endif // _erpc_matrix_multiply_server_hpp_
