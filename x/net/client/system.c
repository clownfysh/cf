#include "cf/x/core/tools.h"
#include "cf/x/net/client/system.h"
#include "cf/x/net/client/socket.h"
#include "cf/x/net/exchange.h"
#include "cf/x/net/post/system.h"
#include "cf/x/net/post/ipost.h"

struct engine_container_t {
  cf_x_net_engine_id_t engine_id;
  cf_x_net_client_system_handle_message_f *message_handlers;
  unsigned long message_handlers_size;
};
typedef struct engine_container_t engine_container_t;

struct cf_x_net_client_system_t {
  int socket;

  cf_x_net_post_system_t *post;
  cf_x_net_exchange_t *exchange;
  pthread_mutex_t messaging_mutex;

  void *custom_client_context;
  char *server_ip_address;
  unsigned short server_min_port;
  unsigned short server_max_port;
  unsigned short server_port;

  cf_x_case_list_t *engines;
  engine_container_t *engines_array[CF_X_NET_ENGINE_TYPE_COUNT];

  cf_x_core_bool_t server_socket_closed;

  cf_x_net_post_ipost_t postey;
  cf_x_net_engine_get_name_f get_engine_name;

  cf_x_core_log_t *log;
};

static cf_x_core_bool_t connect_to_server(cf_x_net_client_system_t *client);

static void destroy_engine_container(void *object);

static cf_x_core_bool_t ensure_client_is_connected(cf_x_net_client_system_t *client);

static cf_x_core_bool_t handle_disconnect(cf_x_net_client_system_t *client);

static cf_x_core_message_t *receive_message(cf_x_net_client_system_t *client);

cf_x_core_bool_t connect_to_server(cf_x_net_client_system_t *client)
{
  assert(client);
  cf_x_core_bool_t success;
  unsigned short port;

  success = cf_x_core_bool_false;

  for (port = client->server_min_port;
       (port <= client->server_max_port) && (!success); port++) {
    client->socket
      = cf_x_net_client_socket_create(client->server_ip_address, port);
    if (client->socket >= 0) {
      client->server_socket_closed = cf_x_core_bool_false;
      client->server_port = port;

      cf_x_core_log_enter(client->log, "hnet",
          "client connected to server on port %i", port);

      pthread_mutex_lock(&client->messaging_mutex);
      {
        client->post = cf_x_net_post_system_create(client->socket);
        if (client->post) {
          if (cf_x_net_exchange_register_post
              (client->exchange, client->post)) {
            success = cf_x_core_bool_true;
          } else {
            cf_x_core_log_trace(client->log, "hnet",
                "x_net_exchange_register_post");
            cf_x_net_client_socket_destroy(client->socket);
            client->server_socket_closed = cf_x_core_bool_true;
            cf_x_net_post_system_destroy(client->post);
            client->post = NULL;
          }
        } else {
          cf_x_core_log_trace(client->log, "hnet", "x_net_post_create");
          cf_x_net_client_socket_destroy(client->socket);
          client->server_socket_closed = cf_x_core_bool_true;
        }
      }
      pthread_mutex_unlock(&client->messaging_mutex);

    } else {
      cf_x_core_log_enter(client->log, "hnet",
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

cf_x_core_bool_t ensure_client_is_connected(cf_x_net_client_system_t *client)
{
  assert(client);
  cf_x_core_bool_t connected;

  connected = cf_x_core_bool_true;

  if (client->server_socket_closed) {
    connected = connect_to_server(client);
    if (connected) {
      cf_x_core_log_enter(client->log, "hnet", "reconnected to server");
    } else {
      cf_x_core_log_enter(client->log, "hnet", "could not reconnect to server");
    }
  }

  return connected;
}

cf_x_core_bool_t handle_disconnect(cf_x_net_client_system_t *client)
{
  assert(client);
  cf_x_core_bool_t success;

  cf_x_core_log_enter(client->log, "hnet", "lost connection to server");

  if (cf_x_net_exchange_unregister_post(client->exchange, client->socket)) {
    success = cf_x_core_bool_true;
    cf_x_net_post_system_destroy(client->post);
    client->post = NULL;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(client->log, "hnet", "x_net_exchange_unregister_post");
  }

  return success;
}

void *cf_x_net_client_system_copy(void *client_object)
{
  cf_x_core_trace_exit("TODO: implement");
  return NULL;
}

cf_x_net_client_system_t *cf_x_net_client_system_create(const char *server_ip_address,
    unsigned short server_min_port, unsigned short server_max_port,
    cf_x_net_engine_get_name_f get_engine_name, void *custom_client_context,
    cf_x_core_log_t *log)
{
  assert(server_ip_address);
  cf_x_net_client_system_t *client;
  cf_x_core_bool_t success;
  unsigned long each_engine_id;
  cf_x_core_bool_t messaging_mutex_needs_destroy;
  cf_x_core_bool_t connected;

  messaging_mutex_needs_destroy = cf_x_core_bool_false;
  connected = cf_x_core_bool_false;

  client = malloc(sizeof *client);
  if (client) {
    client->log = log;
    client->post = NULL;
    client->get_engine_name = get_engine_name;
    client->custom_client_context = custom_client_context;
    client->server_min_port = server_min_port;
    client->server_max_port = server_max_port;
    client->server_socket_closed = cf_x_core_bool_true;
    for (each_engine_id = 0; each_engine_id < CF_X_NET_ENGINE_TYPE_COUNT;
         each_engine_id++) {
      *(client->engines_array + each_engine_id) = NULL;
    }
    cf_x_net_post_ipost_init(&client->postey, cf_x_net_post_system_compare,
        cf_x_net_post_system_create, cf_x_net_post_system_create_decoy,
        cf_x_net_post_system_destroy, cf_x_net_post_system_destroy_decoy,
        cf_x_net_post_system_get_last_receive_activity_time,
        cf_x_net_post_system_get_socket, cf_x_net_post_system_get_stats,
        cf_x_net_post_system_receive_message, cf_x_net_post_system_receive_messages,
        cf_x_net_post_system_send_message, cf_x_net_post_system_send_messages,
        cf_x_net_post_system_is_socket_closed);
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(client->log, "hnet", "malloc");
  }

  if (success) {
    client->server_ip_address = strdup(server_ip_address);
    if (!client->server_ip_address) {
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    client->exchange = cf_x_net_exchange_create(&client->postey);
    if (!client->exchange) {
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    client->engines = cf_x_case_list_create(CF_X_CORE_OBJECT_NO_COMPARE_F,
        CF_X_CORE_OBJECT_NO_COPY_F, destroy_engine_container);
    if (!client->engines) {
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    if (0 == pthread_mutex_init(&client->messaging_mutex, NULL)) {
      messaging_mutex_needs_destroy = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(client->log, "hnet", "pthread_mutex_init");
    }
  }

  if (success) {
    connected = connect_to_server(client);
    if (!connected) {
      success = cf_x_core_bool_false;
      cf_x_core_log_trace(client->log, "hnet", "connect_to_server");
    }
  }

  if (!success && client) {
    if (connected) {
      cf_x_net_client_socket_destroy(client->socket);
    }
    if (client->exchange) {
      cf_x_net_exchange_destroy(client->exchange);
    }
    if (client->server_ip_address) {
      free(client->server_ip_address);
    }
    if (client->engines) {
      cf_x_case_list_destroy(client->engines);
    }
    if (messaging_mutex_needs_destroy) {
      pthread_mutex_destroy(&client->messaging_mutex);
    }
    free(client);
    client = NULL;
  }

  return client;
}

void cf_x_net_client_system_destroy(void *client_object)
{
  assert(client_object);
  cf_x_net_client_system_t *client;

  client = client_object;

  cf_x_net_client_socket_destroy(client->socket);
  if (client->post) {
    cf_x_net_post_system_destroy(client->post);
  }
  cf_x_net_exchange_destroy(client->exchange);
  free(client->server_ip_address);
  cf_x_case_list_destroy(client->engines);
  pthread_mutex_destroy(&client->messaging_mutex);

  free(client);
}

char *cf_x_net_client_system_get_as_string(void *client_object)
{
  cf_x_core_trace_exit("TODO: implement");
  return NULL;
}

char *cf_x_net_client_system_get_server_ip_address(cf_x_net_client_system_t *client)
{
  return client->server_ip_address;
}

unsigned short cf_x_net_client_system_get_server_port(cf_x_net_client_system_t *client)
{
  return client->server_port;
}

int cf_x_net_client_system_get_socket(cf_x_net_client_system_t *client)
{
  return client->socket;
}

cf_x_core_bool_t cf_x_net_client_system_is_connected_to_server(cf_x_net_client_system_t *client)
{
  return !client->server_socket_closed;
}

void cf_x_net_client_system_print(void *client_object)
{
  cf_x_core_trace_exit("TODO: implement");
}

void cf_x_net_client_system_process_messages(cf_x_net_client_system_t *client)
{
  cf_x_core_message_t *message;
  unsigned long message_type;
  cf_x_net_client_system_handle_message_f handler;
  engine_container_t *engine_container;
  cf_x_net_engine_id_t engine_id;

  handler = NULL;

  if (ensure_client_is_connected(client)) {
    while ((message = receive_message(client))) {
      message_type = cf_x_core_message_get_type(message);
      engine_id = cf_x_core_message_get_engine_id(message);
      engine_container = *(client->engines_array + engine_id);
      if (message_type < engine_container->message_handlers_size) {
        handler = *(engine_container->message_handlers + message_type);
        if (handler) {
          (*handler)(client->custom_client_context, message);
        }
      }
      if (!handler) {
        printf("client has no handler for %s engine's message %lu\n",
            cf_x_net_engine_get_name(engine_container->engine_id,
                client->get_engine_name),
            message_type);
      }
      cf_x_core_message_destroy(message);
    }
  } else {
    cf_x_core_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }
}

cf_x_core_bool_t cf_x_net_client_system_register_engine(cf_x_net_client_system_t *client,
    cf_x_net_engine_id_t engine_id, unsigned long message_type_count)
{
  engine_container_t *engine_container;
  cf_x_core_bool_t success;
  unsigned short each_handler;

  engine_container = malloc(sizeof *engine_container);
  if (engine_container) {
    engine_container->engine_id = engine_id;
    success = cf_x_core_bool_true;
  } else {
    cf_x_core_log_trace(client->log, "hnet", "malloc");
    success = cf_x_core_bool_false;
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
      success = cf_x_core_bool_true;
    } else {
      cf_x_core_log_trace(client->log, "hnet", "malloc");
      success = cf_x_core_bool_false;
    }
  }

  if (success) {
    *(client->engines_array + engine_id) = engine_container;
    if (!cf_x_case_list_add_last(client->engines, engine_container)) {
      success = cf_x_core_bool_false;
    }
  }

  return success;
}

void cf_x_net_client_system_register_message_handler(cf_x_net_client_system_t *client,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_client_system_handle_message_f message_handler)
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

cf_x_core_bool_t cf_x_net_client_system_send_message(cf_x_net_client_system_t *client,
    cf_x_core_message_t *message)
{
  cf_x_core_bool_t success;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      if (cf_x_net_post_system_send_message(client->post, message)) {
        success = cf_x_core_bool_true;
        cf_x_net_exchange_send_and_receive_messages(client->exchange);

        if (cf_x_net_post_system_is_socket_closed(client->post)) {
          client->server_socket_closed = cf_x_core_bool_true;
          if (!handle_disconnect(client)) {
            cf_x_core_log_trace(client->log, "hnet", "handle_disconnect");
          }
        }
      } else {
        success = cf_x_core_bool_false;
        cf_x_core_log_trace(client->log, "hnet", "x_net_post_send_message");
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    success = cf_x_core_bool_false;
    cf_x_core_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return success;
}

cf_x_case_list_t *cf_x_net_client_system_take_unsent_messages(cf_x_net_client_system_t *client)
{
  assert(client);
  cf_x_case_list_t *messages;

  pthread_mutex_lock(&client->messaging_mutex);
  messages = cf_x_net_post_system_take_unsent_messages(client->post);
  pthread_mutex_unlock(&client->messaging_mutex);

  return messages;
}

cf_x_core_message_t *receive_message(cf_x_net_client_system_t *client)
{
  cf_x_core_message_t *message;

  if (ensure_client_is_connected(client)) {

    pthread_mutex_lock(&client->messaging_mutex);
    {
      cf_x_net_exchange_send_and_receive_messages(client->exchange);
      message = cf_x_net_post_system_receive_message(client->post);

      if (cf_x_net_post_system_is_socket_closed(client->post)) {
        client->server_socket_closed = cf_x_core_bool_true;
        if (!handle_disconnect(client)) {
          cf_x_core_log_trace(client->log, "hnet", "handle_disconnect");
        }
      }
    }
    pthread_mutex_unlock(&client->messaging_mutex);

  } else {
    message = NULL;
    cf_x_core_log_trace(client->log, "hnet", "ensure_client_is_connected");
  }

  return message;
}
