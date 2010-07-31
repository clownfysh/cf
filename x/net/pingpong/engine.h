#ifndef cf_x_pingpong_engine_h
#define cf_x_pingpong_engine_h

#include "cf/x/core/engine.h"
#include "cf/x/net/server/system.h"

struct cf_x_pingpong_engine_t;
typedef struct cf_x_pingpong_engine_t cf_x_pingpong_engine_t;

void *cf_x_pingpong_engine_create(cf_x_net_server_system_t *cf_x_net_server,
    void *custom_server_object);

void cf_x_pingpong_engine_destroy(void *engine_object);

cf_x_net_server_system_handle_message_f cf_x_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object);

void cf_x_pingpong_engine_maintain(void *engine_object);

void cf_x_pingpong_engine_run(void *engine_thread_object);

void cf_x_pingpong_engine_start(void *engine_thread_object);

void cf_x_pingpong_engine_stop(void *engine_thread_object);

#endif
