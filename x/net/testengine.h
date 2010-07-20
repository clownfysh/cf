#ifndef x_net_testengine_h
#define x_net_testengine_h

#include "x/core/engine.h"
#include "x/net/server.h"

struct x_net_testengine_t;
typedef struct x_net_testengine_t x_net_testengine_t;

void *x_net_testengine_create(x_net_server_t *x_net_server,
    void *custom_server_object);

void x_net_testengine_destroy(void *engine_object);

x_net_server_handle_message_f x_net_testengine_get_handler_for_message
(void *engine_object, void *message_object);

void x_net_testengine_maintain(void *engine_object);

void x_net_testengine_run(void *engine_thread_object);

void x_net_testengine_start(void *engine_thread_object);

void x_net_testengine_stop(void *engine_thread_object);

#endif
