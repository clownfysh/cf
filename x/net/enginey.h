#ifndef x_net_enginey_h
#define x_net_enginey_h

#include "x/core/object.h"
#include "x/net/types.h"

typedef void *(*x_net_enginey_create_f)
  (x_net_server_t *server, void *custom_server_context_object);

typedef void (*x_net_enginey_destroy_f)(void *engine_object);

typedef x_net_server_handle_message_f(*x_net_enginey_get_handler_for_message_f)
(void *engine_object, void *message_object);

typedef void (*x_net_enginey_maintain_f)(void *engine_thread_context_object);

typedef void (*x_net_enginey_run_f)(void *engine_thread_context_object);

typedef void (*x_net_enginey_start_f)(void *engine_thread_context_object);

typedef void (*x_net_enginey_stop_f)(void *engine_thread_context_object);

struct x_net_enginey_t {
  x_net_enginey_create_f create;
  x_net_enginey_destroy_f destroy;
  x_net_enginey_get_handler_for_message_f get_handler_for_message;
  x_net_enginey_maintain_f maintain;
  x_net_enginey_run_f run;
  x_net_enginey_start_f start;
  x_net_enginey_stop_f stop;
};
typedef struct x_net_enginey_t x_net_enginey_t;

void x_net_enginey_init(x_net_enginey_t *enginey,
    x_net_enginey_create_f create, x_net_enginey_destroy_f destroy,
    x_net_enginey_get_handler_for_message_f get_handler_for_message,
    x_net_enginey_maintain_f maintain, x_net_enginey_run_f run,
    x_net_enginey_start_f start, x_net_enginey_stop_f stop);

#endif
