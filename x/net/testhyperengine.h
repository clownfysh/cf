#ifndef cf_x_net_testhyperengine_h
#define cf_x_net_testhyperengine_h

#include "cf/x/core/engine.h"

struct cf_x_net_testhyperengine_t;
typedef struct cf_x_net_testhyperengine_t cf_x_net_testhyperengine_t;

void *cf_x_net_testhyperengine_create(cf_x_net_server_system_t *cf_x_net_server,
    void *custom_server_object);

cf_x_net_server_system_handle_message_f cf_x_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object);

void cf_x_net_testhyperengine_destroy(void *engine_object);

void cf_x_net_testhyperengine_maintain(void *engine_object);

void cf_x_net_testhyperengine_run(void *engine_thread_object);

void cf_x_net_testhyperengine_start(void *engine_thread_object);

void cf_x_net_testhyperengine_stop(void *engine_thread_object);

#endif
