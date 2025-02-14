//
// Created by peter on 2/14/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
bool hello_world_init_client(bool useTcp, const char *ip, int port);

void hello_world_destroy_client();

bool hello_world_call_printText(const char *text);

void hello_world_call_stopServer();
#ifdef __cplusplus
}
#endif