#ifndef cf_x_net_engine_enginey_h
#define cf_x_net_engine_enginey_h

#include "cf/x/core/object.h"
#include "cf/x/net/tools.h"

typedef void *(*cf_x_net_engine_enginey_create_f)
  (cf_x_net_server_t *server, void *custom_server_context_object);

typedef void (*cf_x_net_engine_enginey_destroy_f)(void *engine_object);

typedef cf_x_net_server_handle_message_f(*cf_x_net_engine_enginey_get_handler_for_message_f)
(void *engine_object, void *message_object);

typedef void (*cf_x_net_engine_enginey_maintain_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_enginey_run_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_enginey_start_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_enginey_stop_f)(void *engine_thread_context_object);

struct cf_x_net_engine_enginey_t {
  cf_x_net_engine_enginey_create_f create;
  cf_x_net_engine_enginey_destroy_f destroy;
  cf_x_net_engine_enginey_get_handler_for_message_f get_handler_for_message;
  cf_x_net_engine_enginey_maintain_f maintain;
  cf_x_net_engine_enginey_run_f run;
  cf_x_net_engine_enginey_start_f start;
  cf_x_net_engine_enginey_stop_f stop;
};
typedef struct cf_x_net_engine_enginey_t cf_x_net_engine_enginey_t;

void cf_x_net_engine_enginey_init(cf_x_net_engine_enginey_t *enginey,
    cf_x_net_engine_enginey_create_f create, cf_x_net_engine_enginey_destroy_f destroy,
    cf_x_net_engine_enginey_get_handler_for_message_f get_handler_for_message,
    cf_x_net_engine_enginey_maintain_f maintain, cf_x_net_engine_enginey_run_f run,
    cf_x_net_engine_enginey_start_f start, cf_x_net_engine_enginey_stop_f stop);

#endif
