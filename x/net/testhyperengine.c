#include "cf/x/core/content.h"
#include "cf/x/core/types.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/engine_thread.h"
#include "cf/x/net/hypermessage.h"
#include "cf/x/net/server.h"
#include "cf/x/net/testhyperengine.h"
#include "cf/x/net/testmessage.h"

struct cf_x_net_testhyperengine_t {
  void *things_the_engine_needs_to_share;
  cf_x_net_server_t *cf_x_net_server;
  void *custom_server_object;
};

static cf_x_net_message_status_t handle_about(void *engine_object,
    void *message_object);

static cf_x_net_message_status_t handle_not_found(void *engine_object,
    void *message_object);

cf_x_net_message_status_t handle_about(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_net_testhyperengine_t *engine;
  cf_x_net_hypermessage_t *message;
  cf_x_net_hypermessage_t *response_message;
  int client_socket;
  cf_x_net_server_t *cf_x_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  message = message_object;
  client_socket = cf_x_core_message_get_client_socket(message);
  body = "about this site";
  body_size = strlen(body);

  response_message = cf_x_net_hypermessage_create(client_socket,
      CF_X_NET_HYPERMETHOD_UNKNOWN, CF_X_NET_HYPERSTATUS_OK,
      CF_X_NET_HYPERMESSAGE_NO_RESOURCE_PATH, CF_X_NET_HYPERVERSION_1_0,
      CF_X_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    cf_x_net_hypermessage_set_body(response_message,
        CF_X_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    cf_x_core_trace("x_net_hypermessage_create");
  }

  if (!cf_x_net_server_send_message(cf_x_net_server, response_message)) {
    cf_x_core_trace("x_net_server_send_message");
  }

  return CF_X_NET_MESSAGE_STATUS_HANDLED;
}

cf_x_net_message_status_t handle_not_found(void *engine_object,
    void *message_object)
{
  assert(engine_object);
  assert(message_object);
  cf_x_net_testhyperengine_t *engine;
  cf_x_net_hypermessage_t *message;
  cf_x_net_hypermessage_t *response_message;
  int client_socket;
  cf_x_net_server_t *cf_x_net_server;
  char *body;
  unsigned long body_size;

  engine = engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  message = message_object;
  client_socket = cf_x_core_message_get_client_socket(message);
  body = "testhyperengine cannot handle this request";
  body_size = strlen(body);

  response_message = cf_x_net_hypermessage_create(client_socket,
      CF_X_NET_HYPERMETHOD_UNKNOWN, CF_X_NET_HYPERSTATUS_NOT_FOUND,
      CF_X_NET_HYPERMESSAGE_NO_RESOURCE_PATH, CF_X_NET_HYPERVERSION_1_0,
      CF_X_NET_HYPERMESSAGE_NO_HEADERS);
  if (response_message) {
    cf_x_net_hypermessage_set_body(response_message,
        CF_X_CORE_CONTENT_MIME_TEXT_HTML, body, body_size);
  } else {
    cf_x_core_trace("x_net_hypermessage_create");
  }

  if (!cf_x_net_server_send_message(cf_x_net_server, response_message)) {
    cf_x_core_trace("x_net_server_send_message");
  }

  return CF_X_NET_MESSAGE_STATUS_HANDLED;
}

void *cf_x_net_testhyperengine_create(cf_x_net_server_t *cf_x_net_server,
    void *custom_server_object)
{
  assert(cf_x_net_server);
  assert(custom_server_object);
  cf_x_net_testhyperengine_t *engine;

  printf("testhyperengine creating\n");

  engine = malloc(sizeof *engine);
  if (engine) {
    engine->cf_x_net_server = cf_x_net_server;
    engine->custom_server_object = custom_server_object;
  }

  return engine;
}

void cf_x_net_testhyperengine_destroy(void *engine_object)
{
  assert(engine_object);

  printf("testhyperengine destroying\n");
  free(engine_object);
}

cf_x_net_server_handle_message_f cf_x_net_testhyperengine_get_handler_for_message
(void *engine_object, void *hypermessage_object)
{
  assert(engine_object);
  assert(hypermessage_object);
  cf_x_net_testhyperengine_t *engine;
  cf_x_net_server_handle_message_f handler;
  cf_x_net_hypermessage_t *hypermessage;
  char *resource_name;

  engine = engine_object;
  hypermessage = hypermessage_object;

  resource_name = cf_x_net_hypermessage_get_resource_name(hypermessage);

  if (0 == strcmp("/", resource_name)) {
    handler = handle_about;

  } else if (0 == strcmp("/about", resource_name)) {
    handler = handle_about;

  } else {
    handler = handle_not_found;

  }

  return handler;
}

void cf_x_net_testhyperengine_maintain(void *engine_object)
{
  assert(engine_object);
  cf_x_net_testhyperengine_t *engine;

  engine = engine_object;
}

void cf_x_net_testhyperengine_run(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testhyperengine_t *engine;
  cf_x_net_server_t *cf_x_net_server;
  unsigned short thread_index;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;
  cf_x_net_server = engine->cf_x_net_server;
  thread_index = engine_thread->thread_index;

  cf_x_net_server_process_messages
    (cf_x_net_server, CF_X_NET_ENGINE_HYPER, thread_index);
}

void cf_x_net_testhyperengine_start(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine starting\n");
}

void cf_x_net_testhyperengine_stop(void *engine_thread_object)
{
  assert(engine_thread_object);
  cf_x_net_engine_thread_t *engine_thread;
  cf_x_net_testhyperengine_t *engine;

  engine_thread = engine_thread_object;
  engine = engine_thread->engine_object;

  printf("testhyperengine stopping\n");
}
