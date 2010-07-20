#include "x/core/tools.h"
#include "x/core/types.h"
#include "x/net/engine_thread.h"
#include "x/net/pong.h"
#include "x/net/server.h"
#include "x/net/testengine.h"
#include "x/net/testmessage.h"

struct x_net_testengine_t {
  void *things_the_engine_needs_to_share;
  x_net_server_t *x_net_server;
  void *custom_server_object;
};

static x_net_message_status_t handle_ping(void *engine_object,
    void *message_object);

x_net_message_status_t handle_ping(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  x_net_ping_t *ping;
  x_core_message_t *pong_message;
  x_net_testengine_t *engine;
  x_net_server_t *x_net_server;
  x_net_message_status_t message_status;
  int client_socket;
  x_core_message_t *message;
  x_net_pong_t *pong;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  x_net_server = engine->x_net_server;
  client_socket = x_core_message_get_client_socket(message);

  ping = x_net_ping_create_from_message(message);
  if (ping) {
    ping_string = x_net_ping_get_string(ping);
    if (ping_string) {
      pong = x_net_pong_create(ping_string);
      if (pong) {
        pong_message
          = x_net_testmessage_create(client_socket, X_NET_TESTMESSAGE_PONG);
        if (pong_message) {
          if (x_net_pong_add_to_message(pong, pong_message)) {
            if (x_net_server_send_message(x_net_server, pong_message)) {
              message_status = X_NET_MESSAGE_STATUS_HANDLED;
            } else {
              message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
              x_core_message_destroy(pong_message);
            }
          } else {
            message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
            x_core_trace("x_net_pong_add_to_message");
            x_core_message_destroy(pong_message);
          }
        } else {
          message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
          x_core_trace("x_net_testmessage_create");
        }
        x_net_pong_destroy(pong);
      } else {
        message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
        x_core_trace("x_net_pong_create");
      }
    } else {
      message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
      x_core_trace("x_net_ping_get_string");
    }
    x_net_ping_destroy(ping);
  } else {
    message_status = X_NET_MESSAGE_STATUS_CANT_HANDLE;
    x_core_trace("x_net_ping_create_from_message");
  }

  return message_status;
}

void *x_net_testengine_create(x_net_server_t *x_net_server,
    void *custom_server_object)
{
  assert(x_net_server);
  assert(custom_server_object);
  x_net_testengine_t *engine;

  printf("test engine creating context\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->x_net_server = x_net_server;
    engine->custom_server_object = custom_server_object;
  }

  x_net_server_register_message_handler(x_net_server, X_NET_ENGINE_TEST,
      X_NET_TESTMESSAGE_PING, handle_ping);
  return engine;
}

void x_net_testengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("test engine destroying\n");
  free(engine_object);
}

x_net_server_handle_message_f x_net_testengine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  x_net_testengine_t *engine;
  x_net_server_handle_message_f handler;
  x_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler = x_net_server_get_handler_for_message(engine->x_net_server, message);

  return handler;
}

void x_net_testengine_maintain(void *engine_object)
{
  assert(engine_object);
  x_net_testengine_t *engine;

  engine = engine_object;
}

void x_net_testengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testengine_t *engine;
  x_net_server_t *server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  server = engine->x_net_server;
  thread_index = engine_thread->thread_index;

  x_net_server_process_messages(server, X_NET_ENGINE_TEST, thread_index);
}

void x_net_testengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine starting\n");
}

void x_net_testengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine stopping\n");
}
