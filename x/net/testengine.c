#include "cf/x/core/tools.h"
#include "cf/x/core/types.h"
#include "cf/x/net/engine_thread.h"
#include "cf/x/net/pong.h"
#include "cf/x/net/server.h"
#include "cf/x/net/testengine.h"
#include "cf/x/net/testmessage.h"

struct cf_x_net_testengine_t {
  void *things_the_engine_needs_to_share;
  cf_x_net_server_t *cf_x_net_server;
  void *custom_server_object;
};

static cf_x_net_message_status_t handle_ping(void *engine_object,
    void *message_object);

cf_x_net_message_status_t handle_ping(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_net_ping_t *ping;
  cf_x_core_message_t *pong_message;
  cf_x_net_testengine_t *engine;
  cf_x_net_server_t *cf_x_net_server;
  cf_x_net_message_status_t message_status;
  int client_socket;
  cf_x_core_message_t *message;
  cf_x_net_pong_t *pong;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  client_socket = cf_x_core_message_get_client_socket(message);

  ping = cf_x_net_ping_create_from_message(message);
  if (ping) {
    ping_string = cf_x_net_ping_get_string(ping);
    if (ping_string) {
      pong = cf_x_net_pong_create(ping_string);
      if (pong) {
        pong_message
          = cf_x_net_testmessage_create(client_socket, CF_X_NET_TESTMESSAGE_PONG);
        if (pong_message) {
          if (cf_x_net_pong_add_to_message(pong, pong_message)) {
            if (cf_x_net_server_send_message(cf_x_net_server, pong_message)) {
              message_status = CF_X_NET_MESSAGE_STATUS_HANDLED;
            } else {
              message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
              cf_x_core_message_destroy(pong_message);
            }
          } else {
            message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
            cf_x_core_trace("x_net_pong_add_to_message");
            cf_x_core_message_destroy(pong_message);
          }
        } else {
          message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
          cf_x_core_trace("x_net_testmessage_create");
        }
        cf_x_net_pong_destroy(pong);
      } else {
        message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
        cf_x_core_trace("x_net_pong_create");
      }
    } else {
      message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
      cf_x_core_trace("x_net_ping_get_string");
    }
    cf_x_net_ping_destroy(ping);
  } else {
    message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
    cf_x_core_trace("x_net_ping_create_from_message");
  }

  return message_status;
}

void *cf_x_net_testengine_create(cf_x_net_server_t *cf_x_net_server,
    void *custom_server_object)
{
  assert(cf_x_net_server);
  assert(custom_server_object);
  cf_x_net_testengine_t *engine;

  printf("test engine creating context\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->cf_x_net_server = cf_x_net_server;
    engine->custom_server_object = custom_server_object;
  }

  cf_x_net_server_register_message_handler(cf_x_net_server, CF_X_NET_ENGINE_TEST,
      CF_X_NET_TESTMESSAGE_PING, handle_ping);
  return engine;
}

void cf_x_net_testengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("test engine destroying\n");
  free(engine_object);
}

cf_x_net_server_handle_message_f cf_x_net_testengine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_net_testengine_t *engine;
  cf_x_net_server_handle_message_f handler;
  cf_x_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler = cf_x_net_server_get_handler_for_message(engine->cf_x_net_server, message);

  return handler;
}

void cf_x_net_testengine_maintain(void *engine_object)
{
  assert(engine_object);
  cf_x_net_testengine_t *engine;

  engine = engine_object;
}

void cf_x_net_testengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testengine_t *engine;
  cf_x_net_server_t *server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  server = engine->cf_x_net_server;
  thread_index = engine_thread->thread_index;

  cf_x_net_server_process_messages(server, CF_X_NET_ENGINE_TEST, thread_index);
}

void cf_x_net_testengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine starting\n");
}

void cf_x_net_testengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("test engine stopping\n");
}
