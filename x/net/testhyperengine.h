#ifndef x_net_testhyperengine_h
#define x_net_testhyperengine_h

#include "x/core/engine.h"

struct x_net_testhyperengine_t;
typedef struct x_net_testhyperengine_t x_net_testhyperengine_t;

void *x_net_testhyperengine_create(x_net_server_t *x_net_server,
    void *custom_server_object);

x_net_server_handle_message_f x_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object);

void x_net_testhyperengine_destroy(void *engine_object);

void x_net_testhyperengine_maintain(void *engine_object);

void x_net_testhyperengine_run(void *engine_thread_object);

void x_net_testhyperengine_start(void *engine_thread_object);

void x_net_testhyperengine_stop(void *engine_thread_object);

#endif
