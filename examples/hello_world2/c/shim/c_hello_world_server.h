/*
 * Generated by erpcgen 1.13.0 on Fri Feb 14 14:15:26 2025.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_c_hello_world_server_h_)
#define _c_hello_world_server_h_

#include "hello_world_common.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef void * erpc_service_t;

#if !defined(ERPC_FUNCTIONS_DEFINITIONS)
#define ERPC_FUNCTIONS_DEFINITIONS


/*! @brief TextService identifiers */
enum _TextService_ids
{
    kTextService_service_id = 1,
    kTextService_printText_id = 1,
    kTextService_stopServer_id = 2,
};

//! @name TextService
//@{
bool printText(const char * text);

void stopServer(void);
//@}


#endif // ERPC_FUNCTIONS_DEFINITIONS

/*! @brief Return TextService_service service object. */
erpc_service_t create_TextService_service(void);

/*! @brief Destroy TextService_service service object. */
void destroy_TextService_service(erpc_service_t service);


#if defined(__cplusplus)
}
#endif

#endif // _c_hello_world_server_h_
