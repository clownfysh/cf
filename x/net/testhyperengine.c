#include "x/core/content.h"
#include "x/core/types.h"
#include "x/core/tools.h"
#include "x/net/engine_thread.h"
#include "x/net/hypermessage.h"
#include "x/net/server.h"
#include "x/net/testhyperengine.h"
#include "x/net/testmessage.h"

struct x_net_testhyperengine_t {
  void *things_the_engine_needs_to_share;
  x_net_server_t *x_net_server;
  void *custom_server_object;
};

static x_net_message_status_t handle_about(void *engine_object,
    void *message_object);

static x_net_message_status_t handle_not_found(void *engine_object,
    void *message_object);

x_net_message_status_t handle_about(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  x_net_testhyperengine_t *engine;
  x_net_hypermessage_t *message;
  x_net_hypermessage_t *response_message;
  int client_socket;
  x_net_server_t *x_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  x_net_server = engine->x_net_server;
  message = message_object;
  client_socket = x_core_message_get_client_socket(message);
  body = "about this site";
  body_size = strlen(body);

  response_message = x_net_hypermessage_create(client_socket,
      X_NET_HYPERMETHOD_UNKNOWN, X_NET_HYPERSTATUS_OK,
      X_NET_HYPERMESSAGE_NO_RESOURCE_PATH, X_NET_HYPERVERSION_1_0,
      X_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    x_net_hypermessage_set_body(response_message,
        X_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    x_core_trace("x_net_hypermessage_create");
  }

  if (!x_net_server_send_message(x_net_server, response_message)) {
    x_core_trace("x_net_server_send_message");
  }

  return X_NET_MESSAGE_STATUS_HANDLED;
}

x_net_message_status_t handle_not_found(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  x_net_testhyperengine_t *engine;
  x_net_hypermessage_t *message;
  x_net_hypermessage_t *response_message;
  int client_socket;
  x_net_server_t *x_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  x_net_server = engine->x_net_server;
  message = message_object;
  client_socket = x_core_message_get_client_socket(message);
  body = "testhyperengine cannot handle this request";
  body_size = strlen(body);

  response_message = x_net_hypermessage_create(client_socket,
      X_NET_HYPERMETHOD_UNKNOWN, X_NET_HYPERSTATUS_NOT_FOUND,
      X_NET_HYPERMESSAGE_NO_RESOURCE_PATH, X_NET_HYPERVERSION_1_0,
      X_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    x_net_hypermessage_set_body(response_message,
        X_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    x_core_trace("x_net_hypermessage_create");
  }

  if (!x_net_server_send_message(x_net_server, response_message)) {
    x_core_trace("x_net_server_send_message");
  }

  return X_NET_MESSAGE_STATUS_HANDLED;
}

void *x_net_testhyperengine_create(x_net_server_t *x_net_server,
    void *custom_server_object)
{
  assert(x_net_server);
  assert(custom_server_object);
  x_net_testhyperengine_t *engine;

  printf("testhyperengine creating\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->x_net_server = x_net_server;
    engine->custom_server_object = custom_server_object;
  }

  return engine;
}

void x_net_testhyperengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("testhyperengine destroying\n");
  free(engine_object);
}

x_net_server_handle_message_f x_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object)
{
  assert(engine_object);
  assert(hypermessage_object);
  x_net_testhyperengine_t *engine;
  x_net_server_handle_message_f handler;
  x_net_hypermessage_t *hypermessage;
  char *resource_name;

  engine = engine_object;
  hypermessage = hypermessage_object;

  resource_name = x_net_hypermessage_get_resource_name(hypermessage);

  if (0 == strcmp("/", resource_name)) {
    handler = handle_about;

  } else if (0 == strcmp("/about", resource_name)) {
    handler = handle_about;

  } else {
    handler = handle_not_found;

  }

  return handler;
}

void x_net_testhyperengine_maintain(void *engine_object)
{
  assert(engine_object);
  x_net_testhyperengine_t *engine;

  engine = engine_object;
}

void x_net_testhyperengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testhyperengine_t *engine;
  x_net_server_t *x_net_server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  x_net_server = engine->x_net_server;
  thread_index = engine_thread->thread_index;

  x_net_server_process_messages
    (x_net_server, X_NET_ENGINE_HYPER, thread_index);
}

void x_net_testhyperengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine starting\n");
}

void x_net_testhyperengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  x_net_engine_thread_t *engine_thread;
  x_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine stopping\n");
}
