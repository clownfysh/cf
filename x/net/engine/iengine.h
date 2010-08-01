#ifndef cf_x_net_engine_iengine_h
#define cf_x_net_engine_iengine_h

#include "cf/x/core/object.h"
#include "cf/x/net/tools.h"

typedef void *(*cf_x_net_engine_iengine_create_f)
  (cf_x_net_server_system_t *server, void *custom_server_context_object);

typedef void (*cf_x_net_engine_iengine_destroy_f)(void *engine_object);

typedef cf_x_net_server_system_handle_message_f(*cf_x_net_engine_iengine_get_handler_for_message_f)
(void *engine_object, void *message_object);

typedef void (*cf_x_net_engine_iengine_maintain_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_iengine_run_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_iengine_start_f)(void *engine_thread_context_object);

typedef void (*cf_x_net_engine_iengine_stop_f)(void *engine_thread_context_object);

struct cf_x_net_engine_iengine_t {
  cf_x_net_engine_iengine_create_f create;
  cf_x_net_engine_iengine_destroy_f destroy;
  cf_x_net_engine_iengine_get_handler_for_message_f get_handler_for_message;
  cf_x_net_engine_iengine_maintain_f maintain;
  cf_x_net_engine_iengine_run_f run;
  cf_x_net_engine_iengine_start_f start;
  cf_x_net_engine_iengine_stop_f stop;
};
typedef struct cf_x_net_engine_iengine_t cf_x_net_engine_iengine_t;

void cf_x_net_engine_iengine_init(cf_x_net_engine_iengine_t *iengine,
    cf_x_net_engine_iengine_create_f create, cf_x_net_engine_iengine_destroy_f destroy,
    cf_x_net_engine_iengine_get_handler_for_message_f get_handler_for_message,
    cf_x_net_engine_iengine_maintain_f maintain, cf_x_net_engine_iengine_run_f run,
    cf_x_net_engine_iengine_start_f start, cf_x_net_engine_iengine_stop_f stop);

#endif
