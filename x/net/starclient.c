#include "x/container/set.h"
#include "x/core/nameobject.h"
#include "x/core/period.h"
#include "x/core/tools.h"
#include "x/net/client.h"
#include "x/net/starclient.h"
#include "x/net/types.h"

/*
  TODO: do we need to remove messages from the unsent queue if they've been
  there too long?
*/

#define DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE 1024
#define MAINTAIN_PERIOD 8
#define MAX_CLIENTS 1024

struct message_handler_info_t {
  x_net_engine_id_t engine_id;
  unsigned long message_type;
  x_net_client_handle_message_f message_handler;
};
typedef struct message_handler_info_t message_handler_info_t;

struct x_net_starclient_t {
  x_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_min_port;
  unsigned short node_server_exclude_max_port;

  x_container_set_t *clients;
  x_container_list_t *client_list;

  x_net_maintain_t maintain_schedule;
  x_core_bool_t stop_requested;
  x_core_period_t *maintain_period;

  x_container_list_t *unsent_messages;
  unsigned long unsent_messages_queue_size;

  unsigned long last_star_arm_count;
  x_core_bool_t need_to_print_stats;

  long message_type_counts[X_NET_ENGINE_TYPE_COUNT];
  x_container_list_t *message_handler_info;

  void *custom_client_context;
  x_audit_log_t *log;

  x_core_objectey_t nameobject_objectey;
};

static x_core_bool_t client_connected(x_net_starclient_t *starclient,
    char *client_name);

static char *create_client_name(char *server_ip, unsigned short server_port);

static void establisx_connection(x_net_starclient_t *starclient,
    char *client_name, char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context);

static x_core_bool_t exclude_ip_port_combination
(x_net_starclient_t *starclient, char *ip, unsigned short port);

static x_net_client_t *find_client(x_net_starclient_t *starclient,
    char *client_name);

static x_net_client_t *get_random_client(x_net_starclient_t *starclient);

static x_core_bool_t x_net_starclient_create_client_list
(x_net_starclient_t *starclient);

static x_core_bool_t x_net_starclient_create_clients
(x_net_starclient_t *starclient);

static x_core_bool_t x_net_starclient_create_maintain_period
(x_net_starclient_t *starclient);

static x_core_bool_t x_net_starclient_create_message_handler_info
(x_net_starclient_t *starclient);

static x_core_bool_t x_net_starclient_create_unsent_messages
(x_net_starclient_t *starclient);

static void x_net_starclient_create_rollback(x_net_starclient_t *starclient);

static void print_stats(x_net_starclient_t *starclient);

static x_core_bool_t put_messsage_in_unsent_queue
(x_net_starclient_t *starclient, x_core_message_t *message);

static void re_route_unsent_messages(x_net_starclient_t *starclient);

static void rebuild_client_list(x_net_starclient_t *starclient);

static x_core_bool_t register_engines_witx_client
(x_net_starclient_t *starclient, x_net_client_t *client);

static void register_message_handlers_witx_client
(x_net_starclient_t *starclient, x_net_client_t *client);

static void take_unsent_messages(x_net_starclient_t *starclient,
    x_net_client_t *client);

x_core_bool_t client_connected(x_net_starclient_t *starclient,
    char *client_name)
{
  assert(starclient);
  assert(client_name);
  x_core_bool_t connected;
  x_net_client_t *client;
  x_core_nameobject_t *nameclient_decoy;

  client = find_client(starclient, client_name);
  if (client) {
    if (x_net_client_is_connected_to_server(client)) {
      connected = x_core_bool_true;
    } else {
      connected = x_core_bool_false;
      take_unsent_messages(starclient, client);
      nameclient_decoy = x_core_nameobject_create_decoy(client_name);
      if (nameclient_decoy) {
        if (x_container_set_remove(starclient->clients, nameclient_decoy)) {
          printf("[star] lost connection with %s\n", client_name);
          starclient->need_to_print_stats = x_core_bool_true;
        } else {
          x_core_trace("this should never happen");
        }
        x_core_nameobject_destroy_decoy(nameclient_decoy);
      } else {
        x_core_trace("x_core_nameobject_create_decoy");
      }
    }
  } else {
    connected = x_core_bool_false;
  }

  return connected;
}

char *create_client_name(char *server_ip, unsigned short server_port)
{
  assert(server_ip);
  char *client_name;
  unsigned short server_ip_size;
  unsigned short server_port_string_size;
  unsigned short client_name_size;
  char *server_port_string;

  client_name = NULL;

  server_ip_size = strlen(server_ip);
  server_port_string = malloc(6);
  if (server_port_string) {
    if (snprintf(server_port_string, 6, "%i", server_port) >= 0) {
      server_port_string_size = strlen(server_port_string);
      client_name_size = server_ip_size + 1 + server_port_string_size;
      client_name = malloc(client_name_size + 1);
      if (client_name) {
        memcpy(client_name, server_ip, server_ip_size);
        memcpy(client_name + server_ip_size, ":", 1);
        memcpy(client_name + server_ip_size + 1, server_port_string,
            server_port_string_size);
        *(client_name + client_name_size) = '\0';
      } else {
        x_core_trace("malloc");
      }
    } else {
      x_core_trace("snprintf");
    }
  } else {
    x_core_trace("malloc");
  }
  free(server_port_string);

  return client_name;
}

void establisx_connection(x_net_starclient_t *starclient, char *client_name,
    char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context)
{
  assert(starclient);
  assert(client_name);
  x_core_bool_t established_connection;
  x_net_client_t *client;
  x_core_nameobject_t *nameclient;

  established_connection = x_core_bool_false;

  client = x_net_client_create(server_ip, server_min_port, server_max_port,
      X_NET_ENGINE_NO_GET_NAME_FUNCTION, custom_client_context,
      starclient->log);
  if (client) {
    nameclient = x_core_nameobject_create(client_name, client,
        X_CORE_NO_COPY_FUNCTION, x_net_client_destroy,
        x_net_client_get_as_string);
    if (nameclient) {
      if (x_container_set_add(starclient->clients, nameclient)) {
        if (register_engines_witx_client(starclient, client)) {
          register_message_handlers_witx_client(starclient, client);
          printf("[star] connect to %s\n", client_name);
          starclient->need_to_print_stats = x_core_bool_true;
          established_connection = x_core_bool_true;
        } else {
          x_core_trace("register_engines_witx_client");
        }
      } else {
        x_core_trace("x_container_set_add");
        x_core_nameobject_destroy(nameclient);
      }
    } else {
      x_core_trace("x_core_nameobject_create");
    }
  }
  if (!established_connection) {
    x_net_client_destroy(client);
  }
}

x_core_bool_t exclude_ip_port_combination(x_net_starclient_t *starclient,
    char *ip, unsigned short port)
{
  assert(starclient);
  assert(ip);
  assert(port);
  x_core_bool_t exclude;

  if (((port >= starclient->node_server_exclude_min_port)
          && (port <= starclient->node_server_exclude_max_port))
      && (0 == strcmp(ip, starclient->node_server_exclude_ip))) {
    exclude = x_core_bool_true;
  } else {
    exclude = x_core_bool_false;
  }

  return exclude;
}

x_net_client_t *find_client(x_net_starclient_t *starclient, char *client_name)
{
  assert(starclient);
  assert(client_name);
  x_core_nameobject_t *nameclient_decoy;
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;

  nameclient_decoy = x_core_nameobject_create_decoy(client_name);
  if (nameclient_decoy) {
    nameclient = x_container_set_find(starclient->clients, nameclient_decoy);
    x_core_nameobject_destroy_decoy(nameclient_decoy);
    if (nameclient) {
      client = x_core_nameobject_get_object(nameclient);
    } else {
      client = NULL;
    }
  } else {
    x_core_trace_exit("x_core_nameobject_create_decoy");
    client = NULL;
  }

  return client;
}

x_net_client_t *get_random_client(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;

  nameclient = x_container_list_find_random(starclient->client_list);
  if (nameclient) {
    client = x_core_nameobject_get_object(nameclient);
  } else {
    client = NULL;
  }

  return client;
}

x_core_bool_t x_net_starclient_connect(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t connected_to_at_least_one;
  char *arm_ip;
  unsigned short port;
  char *client_name;
  unsigned long star_arm_count;

  x_container_list_iterate_start(starclient->star_arm_ips);
  while ((arm_ip = x_container_list_iterate_next(starclient->star_arm_ips))) {
    for (port = starclient->star_arm_port_min;
         port <= starclient->star_arm_port_max; port++) {
      if (!exclude_ip_port_combination(starclient, arm_ip, port)) {
        client_name = create_client_name(arm_ip, port);
        if (!client_connected(starclient, client_name)) {
          establisx_connection(starclient, client_name, arm_ip, port, port,
              starclient->custom_client_context);
        }
        free(client_name);
        client_name = NULL;
      }
    }
  }

  star_arm_count = x_container_set_get_size(starclient->clients);
  if (star_arm_count != starclient->last_star_arm_count) {
    starclient->need_to_print_stats = x_core_bool_true;
  }

  if (x_container_set_get_size(starclient->clients) > 0) {
    connected_to_at_least_one = x_core_bool_true;
  } else {
    connected_to_at_least_one = x_core_bool_false;
  }

  return connected_to_at_least_one;
}

x_net_starclient_t *x_net_starclient_create(x_container_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    x_audit_log_t *log)
{
  assert(star_arm_ips);
  assert(log);
  x_net_starclient_t *starclient;
  x_core_bool_t so_far_so_good;
  unsigned long engine_id;

  starclient = malloc(sizeof *starclient);
  if (starclient) {
    starclient->log = log;
    starclient->stop_requested = x_core_bool_false;
    starclient->last_star_arm_count = 0;
    starclient->need_to_print_stats = x_core_bool_false;
    starclient->star_arm_ips = star_arm_ips;
    starclient->star_arm_port_min = star_arm_port_min;
    starclient->star_arm_port_max = star_arm_port_max;
    starclient->node_server_exclude_ip = node_server_exclude_ip;
    starclient->node_server_exclude_min_port = node_server_exclude_min_port;
    starclient->node_server_exclude_max_port = node_server_exclude_max_port;
    starclient->custom_client_context = custom_client_context;
    starclient->unsent_messages_queue_size
      = DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE;
    x_core_nameobject_init_objectey(&starclient->nameobject_objectey);
    for (engine_id = 0; engine_id < X_NET_ENGINE_TYPE_COUNT; engine_id++) {
      *(starclient->message_type_counts + engine_id) = -1;
    }
    if (x_net_starclient_create_clients(starclient)) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
    }
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_starclient_create_maintain_period(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_starclient_create_unsent_messages(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_starclient_create_client_list(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_starclient_create_message_handler_info(starclient);
  }

  if (!so_far_so_good && starclient) {
    x_net_starclient_create_rollback(starclient);
    starclient = NULL;
  }

  assert(!starclient
      || (
        starclient->clients && starclient->maintain_period
        && starclient->unsent_messages && starclient->client_list
        )
    );
  return starclient;
}

x_core_bool_t x_net_starclient_create_client_list
(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t success;

  starclient->client_list = x_container_list_create(x_core_nameobject_compare,
      x_core_nameobject_copy, X_CORE_NO_DESTROY_FUNCTION);
  if (starclient->client_list) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_starclient_create_clients(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t success;

  starclient->clients
    = x_container_set_create(&starclient->nameobject_objectey);
  if (starclient->clients) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_starclient_create_maintain_period
(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t success;

  starclient->maintain_period = x_core_period_create(MAINTAIN_PERIOD);
  if (starclient->maintain_period) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
    x_core_trace("new");
  }

  return success;
}

x_core_bool_t x_net_starclient_create_message_handler_info
(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t success;

  starclient->message_handler_info = x_container_list_create
    (X_CORE_NO_COMPARE_FUNCTION, X_CORE_NO_COPY_FUNCTION, free);
  if (starclient->message_handler_info) {
    success = x_core_bool_true;
  } else {
    x_core_trace("x_container_list_create");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_starclient_create_unsent_messages
(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t success;

  starclient->unsent_messages = x_container_list_create
    (X_CORE_NO_COMPARE_FUNCTION, x_core_message_copy,
        X_CORE_NO_DESTROY_FUNCTION);
  if (starclient->unsent_messages) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

void x_net_starclient_create_rollback(x_net_starclient_t *starclient)
{
  assert(starclient);

  if (starclient->clients) {
    x_container_set_destroy(starclient->clients);
  }
  if (starclient->maintain_period) {
    x_core_period_destroy(starclient->maintain_period);
  }
  if (starclient->unsent_messages) {
    x_container_list_destroy(starclient->unsent_messages);
  }
  if (starclient->client_list) {
    x_container_list_destroy(starclient->client_list);
  }
  if (starclient->message_handler_info) {
    x_container_list_destroy(starclient->message_handler_info);
  }
  free(starclient);
}

void x_net_starclient_destroy(x_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned int unsent_message_count;

  unsent_message_count
    = x_container_list_get_size(starclient->unsent_messages);

  if (unsent_message_count > 0) {
    printf("[star] %u messages in unsent queue were lost\n",
        unsent_message_count);
  }

  x_container_set_destroy(starclient->clients);
  x_core_period_destroy(starclient->maintain_period);
  x_container_list_destroy(starclient->unsent_messages);
  x_container_list_destroy(starclient->client_list);
  x_container_list_destroy(starclient->message_handler_info);
  free(starclient);
}

x_net_client_t *x_net_starclient_get_client(x_net_starclient_t *starclient,
    int socket)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;
  x_core_bool_t found_it;
  int each_socket;

  found_it = x_core_bool_false;
  client = NULL;

  x_container_list_iterate_start(starclient->client_list);
  while (!found_it && (nameclient = x_container_list_iterate_next
          (starclient->client_list))) {
    client = x_core_nameobject_get_object(nameclient);
    each_socket = x_net_client_get_socket(client);
    if (each_socket == socket) {
      found_it = x_core_bool_true;
    }
  }

  return client;
}

void x_net_starclient_get_stats(x_net_starclient_t *starclient,
    x_net_starclient_stats_t *starclient_stats)
{
  assert(starclient);
  assert(starclient_stats);

  starclient_stats->connected_server_count
    = x_container_set_get_size(starclient->clients);
}

void x_net_starclient_process_messages(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;

  x_container_set_iterate_start(starclient->clients);
  while ((nameclient = x_container_set_iterate_next(starclient->clients))) {
    client = x_core_nameobject_get_object(nameclient);
    x_net_client_process_messages(client);
  }

  if (x_core_period_once(starclient->maintain_period)) {
    x_net_starclient_connect(starclient);
    rebuild_client_list(starclient);
    re_route_unsent_messages(starclient);
    if (starclient->need_to_print_stats) {
      print_stats(starclient);
    }
  }
}

x_core_bool_t x_net_starclient_register_engine(x_net_starclient_t *starclient,
    x_net_engine_id_t engine_id, unsigned long message_type_count)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;
  x_core_bool_t success;

  success = x_core_bool_true;

  *(starclient->message_type_counts + engine_id) = message_type_count;

  x_container_set_iterate_start(starclient->clients);
  while ((nameclient = x_container_set_iterate_next(starclient->clients))) {
    client = x_core_nameobject_get_object(nameclient);
    if (!x_net_client_register_engine(client, engine_id, message_type_count)) {
      success = x_core_bool_false;
    }
  }

  return success;
}

void x_net_starclient_register_message_handler(x_net_starclient_t *starclient,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_client_handle_message_f message_handler)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;
  message_handler_info_t *message_handler_info;

  message_handler_info = malloc(sizeof *message_handler_info);
  if (message_handler_info) {
    message_handler_info->engine_id = engine_id;
    message_handler_info->message_type = message_type;
    message_handler_info->message_handler = message_handler;
    if (!x_container_list_add_last
        (starclient->message_handler_info, message_handler_info)) {
      x_core_trace("x_container_list_add_last");
      free(message_handler_info);
    }
  } else {
    x_core_trace("malloc");
  }

  x_container_set_iterate_start(starclient->clients);
  while ((nameclient = x_container_set_iterate_next(starclient->clients))) {
    client = x_core_nameobject_get_object(nameclient);
    x_net_client_register_message_handler
      (client, engine_id, message_type, message_handler);
  }
}

x_core_bool_t x_net_starclient_send_message_to_any_arm
(x_net_starclient_t *starclient, x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  x_net_client_t *random_client;
  x_core_bool_t success;
  x_core_bool_t sent_to_client;

  random_client = get_random_client(starclient);
  if (random_client) {
    if (x_net_client_send_message(random_client, message)) {
      sent_to_client = x_core_bool_true;
    } else {
      sent_to_client = x_core_bool_false;
    }
  } else {
    sent_to_client = x_core_bool_false;
  }

  if (sent_to_client) {
    success = x_core_bool_true;
  } else {
    if (put_messsage_in_unsent_queue(starclient, message)) {
      success = x_core_bool_true;
    } else {
      printf("[star] unsent message queue is full, unable to send message\n");
      success = x_core_bool_false;
    }
  }

  return success;
}

x_core_bool_t x_net_starclient_send_message_to_all_arms
(x_net_starclient_t *starclient, x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  x_core_bool_t success;
  x_core_nameobject_t *nameclient;
  x_net_client_t *client;
  x_core_message_t *message_copy;

  success = x_core_bool_true;

  x_container_set_iterate_start(starclient->clients);
  while ((nameclient = x_container_set_iterate_next(starclient->clients))) {
    client = x_core_nameobject_get_object(nameclient);
    message_copy = x_core_message_copy(message);
    if (message_copy) {
      if (!x_net_client_send_message(client, message_copy)) {
        success = x_core_bool_false;
        x_core_message_destroy(message_copy);
      }
    } else {
      success = x_core_bool_false;
      x_core_trace("x_core_message_copy");
    }
  }

  x_core_message_destroy(message);

  return success;
}

void x_net_starclient_set_unsent_messages_queue_size
(x_net_starclient_t *starclient, unsigned long queue_size)
{
  starclient->unsent_messages_queue_size = queue_size;
}

x_core_bool_t x_net_starclient_star_available(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_bool_t available;
  unsigned long client_count;

  client_count = x_container_set_get_size(starclient->clients);

  if (client_count > 0) {
    available = x_core_bool_true;
  } else {
    available = x_core_bool_false;
  }

  return available;
}

void print_stats(x_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned long star_arm_count;

  star_arm_count = x_container_set_get_size(starclient->clients);

  printf("[star] connected to %lu arms of the star\n", star_arm_count);
  starclient->last_star_arm_count = star_arm_count;
  starclient->need_to_print_stats = x_core_bool_false;
}

x_core_bool_t put_messsage_in_unsent_queue(x_net_starclient_t *starclient,
    x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  unsigned long messages_in_queue;
  x_core_bool_t success;

  messages_in_queue = x_container_list_get_size(starclient->unsent_messages);

  if (starclient->unsent_messages_queue_size > messages_in_queue) {
    if (x_container_list_add_last(starclient->unsent_messages, message)) {
      success = x_core_bool_true;
    } else {
      success = x_core_bool_false;
    }
  } else {
    success = x_core_bool_false;
  }

  return success;
}

void re_route_unsent_messages(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_message_t *message;
  x_container_list_t *unsent_messages;

  unsent_messages = starclient->unsent_messages;

  x_container_list_iterate_start(unsent_messages);
  while ((message = x_container_list_iterate_next(unsent_messages))) {
    x_net_starclient_send_message_to_any_arm(starclient, message);
    x_container_list_iterate_remove(unsent_messages);
  }
}

void rebuild_client_list(x_net_starclient_t *starclient)
{
  assert(starclient);
  x_core_nameobject_t *nameclient;

  x_container_list_clear(starclient->client_list);
  x_container_set_iterate_start(starclient->clients);
  while ((nameclient = x_container_set_iterate_next(starclient->clients))) {
    x_container_list_add_last(starclient->client_list, nameclient);
  }
}

x_core_bool_t register_engines_witx_client(x_net_starclient_t *starclient,
    x_net_client_t *client)
{
  assert(starclient);
  assert(client);
  x_core_bool_t success;
  unsigned long engine_id;
  long message_type_count_long;
  unsigned long message_type_count;

  success = x_core_bool_true;

  for (engine_id = 0; engine_id < X_NET_ENGINE_TYPE_COUNT; engine_id++) {
    message_type_count_long = *(starclient->message_type_counts + engine_id);
    if (-1 != message_type_count_long) {
      message_type_count = (unsigned long) message_type_count_long;
      if (!x_net_client_register_engine
          (client, engine_id, message_type_count)) {
        x_core_trace("x_net_client_register_engine");
        success = x_core_bool_false;
      }
    }
  }

  return success;
}

void register_message_handlers_witx_client(x_net_starclient_t *starclient,
    x_net_client_t *client)
{
  assert(starclient);
  assert(client);
  message_handler_info_t *message_handler_info;

  x_container_list_iterate_start(starclient->message_handler_info);
  while ((message_handler_info
          = x_container_list_iterate_next(starclient->message_handler_info))) {
    x_net_client_register_message_handler(client,
        message_handler_info->engine_id, message_handler_info->message_type,
        message_handler_info->message_handler);
  }
}

void take_unsent_messages(x_net_starclient_t *starclient,
    x_net_client_t *client)
{
  assert(starclient);
  assert(client);
  x_container_list_t *unsent_messages;
  x_core_message_t *message;
  unsigned long discarded_message_count;

  discarded_message_count = 0;

  unsent_messages = x_net_client_take_unsent_messages(client);
  x_container_list_iterate_start(unsent_messages);
  while ((message = x_container_list_iterate_next(unsent_messages))) {
    if (!put_messsage_in_unsent_queue(starclient, message)) {
      discarded_message_count++;
      x_core_message_destroy(message);
    }
  }
  x_container_list_destroy(unsent_messages);

  if (discarded_message_count > 0) {
    printf("[star] unsent message queue is full, unable to take %lu "
        "unsent messages from client\n", discarded_message_count);
  }
}
