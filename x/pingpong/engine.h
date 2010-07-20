#ifndef x_pingpong_engine_h
#define x_pingpong_engine_h

#include "x/core/engine.h"
#include "x/net/server.h"

struct x_pingpong_engine_t;
typedef struct x_pingpong_engine_t x_pingpong_engine_t;

void *x_pingpong_engine_create(x_net_server_t *x_net_server,
    void *custom_server_object);

void x_pingpong_engine_destroy(void *engine_object);

x_net_server_handle_message_f x_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object);

void x_pingpong_engine_maintain(void *engine_object);

void x_pingpong_engine_run(void *engine_thread_object);

void x_pingpong_engine_start(void *engine_thread_object);

void x_pingpong_engine_stop(void *engine_thread_object);

#endif
