#ifndef cf_x_net_testengine_h
#define cf_x_net_testengine_h

#include "cf/x/core/engine.h"
#include "cf/x/net/server.h"

struct cf_x_net_testengine_t;
typedef struct cf_x_net_testengine_t cf_x_net_testengine_t;

void *cf_x_net_testengine_create(cf_x_net_server_t *cf_x_net_server,
    void *custom_server_object);

void cf_x_net_testengine_destroy(void *engine_object);

cf_x_net_server_handle_message_f cf_x_net_testengine_get_handler_for_message
(void *engine_object, void *message_object);

void cf_x_net_testengine_maintain(void *engine_object);

void cf_x_net_testengine_run(void *engine_thread_object);

void cf_x_net_testengine_start(void *engine_thread_object);

void cf_x_net_testengine_stop(void *engine_thread_object);

#endif
