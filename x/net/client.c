#include "x/core/tools.h"
#include "x/net/client.h"
#include "x/net/clientsocket.h"
#include "x/net/exchange.h"
#include "x/net/post.h"
#include "x/net/postey.h"

struct engine_container_t {
  x_net_engine_id_t engine_id;
  x_net_client_handle_message_f *message_handlers;
  unsigned long message_handlers_size;
};
typedef struct engine_container_t engine_container_t;

struct x_net_client_t {
  int socket;

  x_net_post_t *post;
  x_net_exchange_t *exchange;
  pthread_mutex_t messaging_mutex;

  void *custom_client_context;
  char *server_ip_address;
  unsigned short server_min_port;
  unsigned short server_max_port;
  unsigned short server_port;

  x_case_list_t *engines;
  engine_container_t *engines_array[X_NET_ENGINE_TYPE_COUNT];

  x_core_bool_t server_socket_closed;

  x_net_postey_t postey;
  x_net_engine_get_name_f get_engine_name;

  x_audit_log_t *log;
};

static x_core_bool_t connect_to_server(x_net_client_t *client);

static void destroy_engine_container(void *object);

static x_core_bool_t ensure_client_is_connected(x_net_client_t *client);

static x_core_bool_t handle_disconnect(x_net_client_t *client);

static x_core_message_t *receive_message(x_net_client_t *client);

x_core_bool_t connect_to_server(x_net_client_t *client)
{
  assert(client);
  x_core_bool_t success;
  unsigned short port;

  success = x_core_bool_false;

  for (port = client->server_min_port;
       (port <= client->server_max_port) && (!success); port++) {
    client->socket
      = x_net_clientsocket_create(client->server_ip_address, port);
    if (client->socket >= 0) {
      client->server_socket_closed = x_core_bool_false;
      client->server_port = port;

      x_audit_log_enter(client->log, "hnet",
          "client connected to server on port %i", port);

      pthread_mutex_lock(&client->messaging_mutex);
      {
        client->post = x_net_post_create(client->socket);
        if (client->post) {
          if (x_net_exchange_register_post
              (client->exchange, client->post)) {
            success = x_core_bool_true;
          } else {
            x_audit_log_trace(client->log, "hnet",
                "x_net_exchange_register_post");
            x_net_clientsocket_destroy(client->socket);
            client->server_socket_closed = x_core_bool_true;
            x_net_post_destroy(client->post);
            client->post = NULL;
          }
        } else {
          x_audit_log_trace(client->log, "hnet", "x_net_post_create");
          x_net_clientsocket_destroy(client->socket);
          client->server_socket_closed = x_core_bool_true;
        }
      }
      pthread_mutex_unlock(&client->messaging_mutex);

    } else {
      x_audit_log_enter(client->log, "hnet",
          "client could not connect to server on port %i", port);
    }
  }

  return success;
}

void destroy_engine_container(void *object)
{
  assert(object);
  engine_container_t *engine_container;

  engine_container = object;
  free(engine_container->message_handlers);
  free(engine_container);
}

x_core_bool_t ensure_client_is_connected(x_net_client_t *client)
{
  assert(client);
  x_core_bool_t connected;

  connected = x_core_bool_true;

  if (client->server_socket_closed) {
    connected = connect_to_server(client);
    if (connected) {
      x_audit_log_enter(client->log, "hnet", "reconnected to server");
    } else {
      x_audit_log_enter(client->log, "hnet", "could not reconnect to server");
    }
  }

  return connected;
}

x_core_bool_t handle_disconnect(x_net_client_t *client)
{
  assert(client);
  x_core_bool_t success;

  x_audit_log_enter(client->log, "hnet", "lost connection to server");

  if (x_net_exchange_unregister_post(client->exchange, client->socket)) {
    success = x_core_bool_true;
    x_net_post_destroy(client->post);
    client->post = NULL;
  } else {
    success = x_core_bool_false;
    x_audit_log_trace(client->log, "hnet", "x_net_exchange_unregister_post");
  }

  return success;
}

void *x_net_client_copy(void *client_object)
{
  x_core_trace_exit("TODO: implement");
  return NULL;
}

x_net_client_t *x_net_client_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    x_net_engine_get_name_f get_engine_name, void *custom_client_context,
    x_audit_log_t *log)
{
  assert(server_ip_address);
  x_net_client_t *client;
  x_core_bool_t success;
  unsigned long each_engine_id;
  x_core_bool_t messaging_mutex_needs_destroy;
  x_core_bool_t connected;

  messaging_mutex_needs_destroy = x_core_bool_false;
  connected = x_core_bool_false;

  client = malloc(sizeof *client);
  if (client) {
    client->log = log;
    client->post = NULL;
    client->get_engine_name = get_engine_name;
    client->custom_client_context = custom_client_context;
    client->server_min_port = server_min_port;
    client->server_max_port = server_max_port;
    client->server_socket_closed = x_core_bool_true;
    for (each_engine_id = 0; each_engine_id < X_NET_ENGINE_TYPE_COUNT;
         each_engine_id++) {
      *(client->engines_array + each_engine_id) = NULL;
    }
    x_net_postey_init(&client->postey, x_net_post_compare,
        x_net_post_create, x_net_post_create_decoy,
        x_net_post_destroy, x_net_post_destroy_decoy,
        x_net_post_get_last_receive_activity_time,
        x_net_post_get_socket, x_net_post_get_stats,
        x_net_post_receive_message, x_net_post_receive_messages,
        x_net_post_send_message, x_net_post_send_messages,
        x_net_post_is_socket_closed);
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
    x_audit_log_trace(client->log, "hnet", "malloc");
  }

  if (success) {
    client->server_ip_address = strdup(server_ip_address);
    if (!client->server_ip_address) {
      success = x_core_bool_false;
    }
  }

  if (success) {
    client->exchange = x_net_exchange_create(&client->postey);
    if (!client->exchange) {
      success = x_core_bool_false;
    }
  }

  if (success) {
    client->engines = x_case_list_create(X_CORE_NO_COMPARE_FUNCTION,
        X_CORE_NO_COPY_FUNCTION, destroy_engine_container);
    if (!client->engines) {
      success = x_core_bool_false;
    }
  }

  if (success) {
    if (0 == pthread_mutex_init(&client->messaging_mutex, NULL)) {
      messaging_mutex_needs_destroy = x_core_bool_true;
    } else {
      success = x_core_bool_false;
      x_audit_log_trace(client->log, "hnet", "pthread_mutex_init");
    }
  }

  if (success) {
    connected = connect_to_server(client);
    if (!connected) {
      success = x_core_bool_false;
      x_audit_log_trace(client->log, "hnet", "connect_to_server");
    }
  }

  if (!success && client) {
    if (connected) {
      x_net_clientsocket_destroy(client->socket);
    }
    if (client->exchange) {
      x_net_exchange_destroy(client->exchange);
    }
    if (client->server_ip_address) {
      free(client->server_ip_address);
    }
    if (client->engines) {
      x_case_list_destroy(client->engines);
    }
    if (messaging_mutex_needs_destroy) {
      pthread_mutex_destroy(&client->messaging_mutex);
    }
    free(client);
    client = NULL;
  }

  return client;
}

void x_net_client_destroy(void *client_object)
{
  assert(client_object);
  x_net_client_t *client;

  client = client_object;

  x_net_clientsocket_destroy(client->socket);
  if (client->post) {
    x_net_post_destroy(client->post);
  }
  x_net_exchange_destroy(client->exchange);
  free(client->server_ip_address);
  x_case_list_destroy(client->engines);
  pthread_mutex_destroy(&client->messaging_mutex);

  free(client);
}

char *x_net_client_get_as_string(void *client_object)
{
  x_core_trace_exit("TODO: implement");
  return NULL;
}

char *x_net_client_get_server_ip_address(x_net_client_t *client)
{
  return client->server_ip_address;
}

unsigned short x_net_client_get_server_port(x_net_client_t *client)
{
  return client->server_port;
}

int x_net_client_get_socket(x_net_client_t *client)
{
  return client->socket;
}

x_core_bool_t x_net_client_is_connected_to_server(x_net_client_t *client)
{
  return !client->server_socket_closed;
}

void x_net_client_print(void *client_object)
{
  x_core_trace_exit("TODO: implement");
}

void x_net_client_process_messages(x_net_client_t *client)
{
  x_core_message_t *message;
  unsigned long message_type;
  x_net_client_handle_message_f handler;
  engine_container_t *engine_container;
  x_net_engine_id_t engine_id;

  handler = NULL;

  if (ensure_client_is_connected(client)) {
    while ((message = receive_message(client))) {
      message_type = x_core_message_get_type(message);
      engine_id = x_core_message_get_engine_id(message);
      engine_container = *(client->engines_array + engine_id);
      if (message_type < engine_container->message_handlers_size) {
        handler = *(engine_container->message_handlers + message_type);
        if (handler) {
          (*handler)(client->custom_client_context, message);
        }
      }
      if (!handler) {
        printf("client has no handler for %s engine's message %lu\n",
            x_net_engine_get_name(engine_container->engine_id,
                client->get_engine_name),
            message_type);
      }
      x_core_message_destroy(message);
    }
  } else {
    x_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }
}

x_core_bool_t x_net_client_register_engine(x_net_client_t *client,
    x_net_engine_id_t engine_id, unsigned long message_type_count)
{
  engine_container_t *engine_container;
  x_core_bool_t success;
  unsigned short each_handler;

  engine_container = malloc(sizeof *engine_container);
  if (engine_container) {
    engine_container->engine_id = engine_id;
    success = x_core_bool_true;
  } else {
    x_audit_log_trace(client->log, "hnet", "malloc");
    success = x_core_bool_false;
  }

  if (success) {
    engine_container->message_handlers_size = message_type_count;
    engine_container->message_handlers
      = malloc((sizeof *engine_container->message_handlers)
          * message_type_count);
    if (engine_container->message_handlers) {
      for (each_handler = 0; each_handler < message_type_count;
           each_handler++) {
        *(engine_container->message_handlers + each_handler) = NULL;
      }
      success = x_core_bool_true;
    } else {
      x_audit_log_trace(client->log, "hnet", "malloc");
      success = x_core_bool_false;
    }
  }

  if (success) {
    *(client->engines_array + engine_id) = engine_container;
    if (!x_case_list_add_last(client->engines, engine_container)) {
      success = x_core_bool_false;
    }
  }

  return success;
}

void x_net_client_register_message_handler(x_net_client_t *client,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_client_handle_message_f message_handler)
{
  assert(client);
  assert(message_handler);
  assert(client->engines_array[engine_id]);
  assert(message_type
      < client->engines_array[engine_id]->message_handlers_size);
  engine_container_t *engine_container;

  engine_container = client->engines_array[engine_id];
  *(engine_container->message_handlers + message_type) = message_handler;
}

x_core_bool_t x_net_client_send_message(x_net_client_t *client,
    x_core_message_t *message)
{
  x_core_bool_t success;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      if (x_net_post_send_message(client->post, message)) {
        success = x_core_bool_true;
        x_net_exchange_send_and_receive_messages(client->exchange);

        if (x_net_post_is_socket_closed(client->post)) {
          client->server_socket_closed = x_core_bool_true;
          if (!handle_disconnect(client)) {
            x_audit_log_trace(client->log, "hnet", "handle_disconnect");
          }
        }
      } else {
        success = x_core_bool_false;
        x_audit_log_trace(client->log, "hnet", "x_net_post_send_message");
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    success = x_core_bool_false;
    x_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return success;
}

x_case_list_t *x_net_client_take_unsent_messages(x_net_client_t *client)
{
  assert(client);
  x_case_list_t *messages;

  pthread_mutex_lock(&client->messaging_mutex);
  messages = x_net_post_take_unsent_messages(client->post);
  pthread_mutex_unlock(&client->messaging_mutex);

  return messages;
}

x_core_message_t *receive_message(x_net_client_t *client)
{
  x_core_message_t *message;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      x_net_exchange_send_and_receive_messages(client->exchange);
      message = x_net_post_receive_message(client->post);

      if (x_net_post_is_socket_closed(client->post)) {
        client->server_socket_closed = x_core_bool_true;
        if (!handle_disconnect(client)) {
          x_audit_log_trace(client->log, "hnet", "handle_disconnect");
        }
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    message = NULL;
    x_audit_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return message;
}
