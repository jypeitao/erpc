//
// Created by peter on 2/14/25.
//

#pragma once

typedef void (*printTextCall)(const char *text);

void hello_world_init_server(bool useTcp, printTextCall call);

// MUST: one time init and one time start
void hello_world_start_server();

void hello_world_stop_server();

void hello_world_destroy_server();

