#include "cf/x/core/object.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/engine/thread.h"
#include "cf/x/net/pingpong/engine.h"
#include "cf/x/net/pingpong/message.h"
#include "cf/x/net/pingpong/ping.h"
#include "cf/x/net/pingpong/pong.h"
#include "cf/x/net/server/system.h"

struct cf_x_pingpong_engine_t {
  void *things_the_engine_needs_to_share;
  cf_x_net_server_system_t *cf_x_net_server;
  void *custom_server_object;
};

static cf_x_net_message_status_t handle_ping(void *engine_object,
    void *message_object);

cf_x_net_message_status_t handle_ping(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_pingpong_ping_t *ping;
  cf_x_pingpong_pong_t *pong;
  cf_x_core_message_t *pong_message;
  cf_x_pingpong_engine_t *engine;
  cf_x_net_server_system_t *cf_x_net_server;
  cf_x_net_message_status_t message_status;
  int client_socket;
  cf_x_core_message_t *message;
  char *ping_string;

  message = message_object;
  engine = engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  client_socket = cf_x_core_message_get_client_socket(message);

  ping = cf_x_pingpong_ping_create_from_message(message);
  if (ping) {
    ping_string = cf_x_pingpong_ping_get_string(ping);
    if (ping_string) {
      pong = cf_x_pingpong_pong_create(ping_string);
      if (pong) {
        pong_message = cf_x_pingpong_message_create
          (client_socket, cf_x_PINGPONG_MESSAGE_PONG);
        if (pong_message) {
          if (cf_x_pingpong_pong_add_to_message(pong, pong_message)) {
            if (cf_x_net_server_system_send_message(cf_x_net_server, pong_message)) {
              message_status = CF_X_NET_MESSAGE_STATUS_HANDLED;
            } else {
              cf_x_core_trace("x_net_server_send_message() failed");
              message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
              cf_x_core_message_destroy(pong_message);
            }
          } else {
            cf_x_core_trace("x_pingpong_pong_add_to_message");
            message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
            cf_x_core_message_destroy(pong_message);
          }
        } else {
          cf_x_core_trace("x_pingpong_message_create");
          message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
        }
        cf_x_pingpong_pong_destroy(pong);
      } else {
        cf_x_core_trace("x_pingpong_pong_create");
        message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
      }
      cf_x_pingpong_ping_destroy(ping);
    } else {
      cf_x_core_trace("x_pingpong_ping_get_string");
      message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
    }
  } else {
    cf_x_core_trace("x_pingpong_ping_create");
    message_status = CF_X_NET_MESSAGE_STATUS_CANT_HANDLE;
  }

  return message_status;
}

void *cf_x_pingpong_engine_create(cf_x_net_server_system_t *cf_x_net_server,
    void *custom_server_object)
{
  assert(cf_x_net_server);
  assert(custom_server_object);
  cf_x_pingpong_engine_t *engine;

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->cf_x_net_server = cf_x_net_server;
    engine->custom_server_object = custom_server_object;
  }

  cf_x_net_server_system_register_message_handler(cf_x_net_server, CF_X_NET_ENGINE_PING,
      cf_x_PINGPONG_MESSAGE_PING, handle_ping);

  return engine;
}

void cf_x_pingpong_engine_destroy(void *engine_object)
{
  free(engine_object);
}

cf_x_net_server_system_handle_message_f cf_x_pingpong_engine_get_handler_for_message
(void *engine_object, void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_pingpong_engine_t *engine;
  cf_x_net_server_system_handle_message_f handler;
  cf_x_core_message_t *message;

  engine = engine_object;
  message = message_object;
  handler
    = cf_x_net_server_system_get_handler_for_message(engine->cf_x_net_server, message);

  return handler;
}

void cf_x_pingpong_engine_maintain(void *engine_object)
{
  assert(engine_object);
  cf_x_pingpong_engine_t *engine;
  cf_x_net_server_system_t *cf_x_net_server;

  engine = engine_object;
  cf_x_net_server = engine->cf_x_net_server;

  cf_x_net_server_system_print_stats(cf_x_net_server);
}

void cf_x_pingpong_engine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_pingpong_engine_t *engine;
  cf_x_net_server_system_t *cf_x_net_server;
  unsigned char thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  thread_index = engine_thread->thread_index;

  cf_x_net_server_system_process_messages(cf_x_net_server, CF_X_NET_ENGINE_PING, thread_index);
}

void cf_x_pingpong_engine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  /*  cf_x_net_engine_thread_t *engine_thread;  */
  /*  cf_x_pingpong_engine_t *engine;  */

  /*  engine_thread = engine_thread_object;  */
  /*  engine = engine_thread->engine_object;  */

  printf("[ping] ping engine starting\n");
}

void cf_x_pingpong_engine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  /*  cf_x_net_engine_thread_t *engine_thread;  */
  /*  cf_x_pingpong_engine_t *engine;  */

  /*  engine_thread = engine_thread_object;  */
  /*  engine = engine_thread->engine_object;  */

  printf("[ping] ping engine stopping\n");
}
