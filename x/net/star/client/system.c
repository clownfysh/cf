#include "cf/x/case/set.h"
#include "cf/x/core/nameobject.h"
#include "cf/x/sync/period.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/client/system.h"
#include "cf/x/net/star/client/system.h"
#include "cf/x/net/tools.h"

/*
  TODO: do we need to remove messages from the unsent queue if they've been
  there too long?
*/

#define DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE 1024
#define MAINTAIN_PERIOD 8
#define MAX_CLIENTS 1024

struct message_handler_info_t {
  cf_x_net_engine_id_t engine_id;
  unsigned long message_type;
  cf_x_net_client_system_handle_message_f message_handler;
};
typedef struct message_handler_info_t message_handler_info_t;

struct cf_x_net_star_client_system_t {
  cf_x_case_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_min_port;
  unsigned short node_server_exclude_max_port;

  cf_x_case_set_t *clients;
  cf_x_case_list_t *client_list;

  cf_x_net_maintain_t maintain_schedule;
  cf_x_core_bool_t stop_requested;
  cf_x_sync_period_t *maintain_period;

  cf_x_case_list_t *unsent_messages;
  unsigned long unsent_messages_queue_size;

  unsigned long last_star_arm_count;
  cf_x_core_bool_t need_to_print_stats;

  long message_type_counts[CF_X_NET_ENGINE_TYPE_COUNT];
  cf_x_case_list_t *message_handler_info;

  void *custom_client_context;
  cf_x_core_log_t *log;

  cf_x_core_objectey_t nameobject_objectey;
};

static cf_x_core_bool_t client_connected(cf_x_net_star_client_system_t *starclient,
    char *client_name);

static char *create_client_name(char *server_ip, unsigned short server_port);

static void establisx_connection(cf_x_net_star_client_system_t *starclient,
    char *client_name, char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context);

static cf_x_core_bool_t exclude_ip_port_combination
(cf_x_net_star_client_system_t *starclient, char *ip, unsigned short port);

static cf_x_net_client_system_t *find_client(cf_x_net_star_client_system_t *starclient,
    char *client_name);

static cf_x_net_client_system_t *get_random_client(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t cf_x_net_star_client_system_create_client_list
(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t cf_x_net_star_client_system_create_clients
(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t cf_x_net_star_client_system_create_maintain_period
(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t cf_x_net_star_client_system_create_message_handler_info
(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t cf_x_net_star_client_system_create_unsent_messages
(cf_x_net_star_client_system_t *starclient);

static void cf_x_net_star_client_system_create_rollback(cf_x_net_star_client_system_t *starclient);

static void print_stats(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t put_messinferno_in_unsent_queue
(cf_x_net_star_client_system_t *starclient, cf_x_core_message_t *message);

static void re_route_unsent_messages(cf_x_net_star_client_system_t *starclient);

static void rebuild_client_list(cf_x_net_star_client_system_t *starclient);

static cf_x_core_bool_t register_engines_witx_client
(cf_x_net_star_client_system_t *starclient, cf_x_net_client_system_t *client);

static void register_message_handlers_witx_client
(cf_x_net_star_client_system_t *starclient, cf_x_net_client_system_t *client);

static void take_unsent_messages(cf_x_net_star_client_system_t *starclient,
    cf_x_net_client_system_t *client);

cf_x_core_bool_t client_connected(cf_x_net_star_client_system_t *starclient,
    char *client_name)
{
  assert(starclient);
  assert(client_name);
  cf_x_core_bool_t connected;
  cf_x_net_client_system_t *client;
  cf_x_core_nameobject_t *nameclient_decoy;

  client = find_client(starclient, client_name);
  if (client) {
    if (cf_x_net_client_system_is_connected_to_server(client)) {
      connected = cf_x_core_bool_true;
    } else {
      connected = cf_x_core_bool_false;
      take_unsent_messages(starclient, client);
      nameclient_decoy = cf_x_core_nameobject_create_decoy(client_name);
      if (nameclient_decoy) {
        if (cf_x_case_set_remove(starclient->clients, nameclient_decoy)) {
          printf("[star] lost connection with %s\n", client_name);
          starclient->need_to_print_stats = cf_x_core_bool_true;
        } else {
          cf_x_core_trace("this should never happen");
        }
        cf_x_core_nameobject_destroy_decoy(nameclient_decoy);
      } else {
        cf_x_core_trace("x_core_nameobject_create_decoy");
      }
    }
  } else {
    connected = cf_x_core_bool_false;
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
        cf_x_core_trace("malloc");
      }
    } else {
      cf_x_core_trace("snprintf");
    }
  } else {
    cf_x_core_trace("malloc");
  }
  free(server_port_string);

  return client_name;
}

void establisx_connection(cf_x_net_star_client_system_t *starclient, char *client_name,
    char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context)
{
  assert(starclient);
  assert(client_name);
  cf_x_core_bool_t established_connection;
  cf_x_net_client_system_t *client;
  cf_x_core_nameobject_t *nameclient;

  established_connection = cf_x_core_bool_false;

  client = cf_x_net_client_system_create(server_ip, server_min_port, server_max_port,
      CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, custom_client_context,
      starclient->log);
  if (client) {
    nameclient = cf_x_core_nameobject_create(client_name, client,
        CF_X_CORE_OBJECT_NO_COPY_F, cf_x_net_client_system_destroy,
        cf_x_net_client_system_get_as_string);
    if (nameclient) {
      if (cf_x_case_set_add(starclient->clients, nameclient)) {
        if (register_engines_witx_client(starclient, client)) {
          register_message_handlers_witx_client(starclient, client);
          printf("[star] connect to %s\n", client_name);
          starclient->need_to_print_stats = cf_x_core_bool_true;
          established_connection = cf_x_core_bool_true;
        } else {
          cf_x_core_trace("register_engines_witx_client");
        }
      } else {
        cf_x_core_trace("x_case_set_add");
        cf_x_core_nameobject_destroy(nameclient);
      }
    } else {
      cf_x_core_trace("x_core_nameobject_create");
    }
  }
  if (!established_connection) {
    cf_x_net_client_system_destroy(client);
  }
}

cf_x_core_bool_t exclude_ip_port_combination(cf_x_net_star_client_system_t *starclient,
    char *ip, unsigned short port)
{
  assert(starclient);
  assert(ip);
  assert(port);
  cf_x_core_bool_t exclude;

  if (((port >= starclient->node_server_exclude_min_port)
          && (port <= starclient->node_server_exclude_max_port))
      && (0 == strcmp(ip, starclient->node_server_exclude_ip))) {
    exclude = cf_x_core_bool_true;
  } else {
    exclude = cf_x_core_bool_false;
  }

  return exclude;
}

cf_x_net_client_system_t *find_client(cf_x_net_star_client_system_t *starclient, char *client_name)
{
  assert(starclient);
  assert(client_name);
  cf_x_core_nameobject_t *nameclient_decoy;
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;

  nameclient_decoy = cf_x_core_nameobject_create_decoy(client_name);
  if (nameclient_decoy) {
    nameclient = cf_x_case_set_find(starclient->clients, nameclient_decoy);
    cf_x_core_nameobject_destroy_decoy(nameclient_decoy);
    if (nameclient) {
      client = cf_x_core_nameobject_get_object(nameclient);
    } else {
      client = NULL;
    }
  } else {
    cf_x_core_trace_exit("x_core_nameobject_create_decoy");
    client = NULL;
  }

  return client;
}

cf_x_net_client_system_t *get_random_client(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;

  nameclient = cf_x_case_list_find_random(starclient->client_list);
  if (nameclient) {
    client = cf_x_core_nameobject_get_object(nameclient);
  } else {
    client = NULL;
  }

  return client;
}

cf_x_core_bool_t cf_x_net_star_client_system_connect(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t connected_to_at_least_one;
  char *arm_ip;
  unsigned short port;
  char *client_name;
  unsigned long star_arm_count;

  cf_x_case_list_iterate_start(starclient->star_arm_ips);
  while ((arm_ip = cf_x_case_list_iterate_next(starclient->star_arm_ips))) {
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

  star_arm_count = cf_x_case_set_get_size(starclient->clients);
  if (star_arm_count != starclient->last_star_arm_count) {
    starclient->need_to_print_stats = cf_x_core_bool_true;
  }

  if (cf_x_case_set_get_size(starclient->clients) > 0) {
    connected_to_at_least_one = cf_x_core_bool_true;
  } else {
    connected_to_at_least_one = cf_x_core_bool_false;
  }

  return connected_to_at_least_one;
}

cf_x_net_star_client_system_t *cf_x_net_star_client_system_create(cf_x_case_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    cf_x_core_log_t *log)
{
  assert(star_arm_ips);
  assert(log);
  cf_x_net_star_client_system_t *starclient;
  cf_x_core_bool_t so_far_so_good;
  unsigned long engine_id;

  starclient = malloc(sizeof *starclient);
  if (starclient) {
    starclient->log = log;
    starclient->stop_requested = cf_x_core_bool_false;
    starclient->last_star_arm_count = 0;
    starclient->need_to_print_stats = cf_x_core_bool_false;
    starclient->star_arm_ips = star_arm_ips;
    starclient->star_arm_port_min = star_arm_port_min;
    starclient->star_arm_port_max = star_arm_port_max;
    starclient->node_server_exclude_ip = node_server_exclude_ip;
    starclient->node_server_exclude_min_port = node_server_exclude_min_port;
    starclient->node_server_exclude_max_port = node_server_exclude_max_port;
    starclient->custom_client_context = custom_client_context;
    starclient->unsent_messages_queue_size
      = DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE;
    cf_x_core_nameobject_init_objectey(&starclient->nameobject_objectey);
    for (engine_id = 0; engine_id < CF_X_NET_ENGINE_TYPE_COUNT; engine_id++) {
      *(starclient->message_type_counts + engine_id) = -1;
    }
    if (cf_x_net_star_client_system_create_clients(starclient)) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      so_far_so_good = cf_x_core_bool_false;
    }
  } else {
    cf_x_core_trace("malloc");
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_star_client_system_create_maintain_period(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_star_client_system_create_unsent_messages(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_star_client_system_create_client_list(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = cf_x_net_star_client_system_create_message_handler_info(starclient);
  }

  if (!so_far_so_good && starclient) {
    cf_x_net_star_client_system_create_rollback(starclient);
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

cf_x_core_bool_t cf_x_net_star_client_system_create_client_list
(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t success;

  starclient->client_list = cf_x_case_list_create(cf_x_core_nameobject_compare,
      cf_x_core_nameobject_copy, CF_X_CORE_OBJECT_NO_DESTROY_F);
  if (starclient->client_list) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t cf_x_net_star_client_system_create_clients(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t success;

  starclient->clients
    = cf_x_case_set_create(&starclient->nameobject_objectey);
  if (starclient->clients) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t cf_x_net_star_client_system_create_maintain_period
(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t success;

  starclient->maintain_period = cf_x_sync_period_create(MAINTAIN_PERIOD);
  if (starclient->maintain_period) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("new");
  }

  return success;
}

cf_x_core_bool_t cf_x_net_star_client_system_create_message_handler_info
(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t success;

  starclient->message_handler_info = cf_x_case_list_create
    (CF_X_CORE_OBJECT_NO_COMPARE_F, CF_X_CORE_OBJECT_NO_COPY_F, free);
  if (starclient->message_handler_info) {
    success = cf_x_core_bool_true;
  } else {
    cf_x_core_trace("x_case_list_create");
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t cf_x_net_star_client_system_create_unsent_messages
(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t success;

  starclient->unsent_messages = cf_x_case_list_create
    (CF_X_CORE_OBJECT_NO_COMPARE_F, cf_x_core_message_copy,
        CF_X_CORE_OBJECT_NO_DESTROY_F);
  if (starclient->unsent_messages) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

void cf_x_net_star_client_system_create_rollback(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);

  if (starclient->clients) {
    cf_x_case_set_destroy(starclient->clients);
  }
  if (starclient->maintain_period) {
    cf_x_sync_period_destroy(starclient->maintain_period);
  }
  if (starclient->unsent_messages) {
    cf_x_case_list_destroy(starclient->unsent_messages);
  }
  if (starclient->client_list) {
    cf_x_case_list_destroy(starclient->client_list);
  }
  if (starclient->message_handler_info) {
    cf_x_case_list_destroy(starclient->message_handler_info);
  }
  free(starclient);
}

void cf_x_net_star_client_system_destroy(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  unsigned int unsent_message_count;

  unsent_message_count
    = cf_x_case_list_get_size(starclient->unsent_messages);

  if (unsent_message_count > 0) {
    printf("[star] %u messages in unsent queue were lost\n",
        unsent_message_count);
  }

  cf_x_case_set_destroy(starclient->clients);
  cf_x_sync_period_destroy(starclient->maintain_period);
  cf_x_case_list_destroy(starclient->unsent_messages);
  cf_x_case_list_destroy(starclient->client_list);
  cf_x_case_list_destroy(starclient->message_handler_info);
  free(starclient);
}

cf_x_net_client_system_t *cf_x_net_star_client_system_get_client(cf_x_net_star_client_system_t *starclient,
    int socket)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;
  cf_x_core_bool_t found_it;
  int each_socket;

  found_it = cf_x_core_bool_false;
  client = NULL;

  cf_x_case_list_iterate_start(starclient->client_list);
  while (!found_it && (nameclient = cf_x_case_list_iterate_next
          (starclient->client_list))) {
    client = cf_x_core_nameobject_get_object(nameclient);
    each_socket = cf_x_net_client_system_get_socket(client);
    if (each_socket == socket) {
      found_it = cf_x_core_bool_true;
    }
  }

  return client;
}

void cf_x_net_star_client_system_get_stats(cf_x_net_star_client_system_t *starclient,
    cf_x_net_star_client_stats_t *starclient_stats)
{
  assert(starclient);
  assert(starclient_stats);

  starclient_stats->connected_server_count
    = cf_x_case_set_get_size(starclient->clients);
}

void cf_x_net_star_client_system_process_messages(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;

  cf_x_case_set_iterate_start(starclient->clients);
  while ((nameclient = cf_x_case_set_iterate_next(starclient->clients))) {
    client = cf_x_core_nameobject_get_object(nameclient);
    cf_x_net_client_system_process_messages(client);
  }

  if (cf_x_sync_period_once(starclient->maintain_period)) {
    cf_x_net_star_client_system_connect(starclient);
    rebuild_client_list(starclient);
    re_route_unsent_messages(starclient);
    if (starclient->need_to_print_stats) {
      print_stats(starclient);
    }
  }
}

cf_x_core_bool_t cf_x_net_star_client_system_register_engine(cf_x_net_star_client_system_t *starclient,
    cf_x_net_engine_id_t engine_id, unsigned long message_type_count)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  *(starclient->message_type_counts + engine_id) = message_type_count;

  cf_x_case_set_iterate_start(starclient->clients);
  while ((nameclient = cf_x_case_set_iterate_next(starclient->clients))) {
    client = cf_x_core_nameobject_get_object(nameclient);
    if (!cf_x_net_client_system_register_engine(client, engine_id, message_type_count)) {
      success = cf_x_core_bool_false;
    }
  }

  return success;
}

void cf_x_net_star_client_system_register_message_handler(cf_x_net_star_client_system_t *starclient,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_client_system_handle_message_f message_handler)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;
  message_handler_info_t *message_handler_info;

  message_handler_info = malloc(sizeof *message_handler_info);
  if (message_handler_info) {
    message_handler_info->engine_id = engine_id;
    message_handler_info->message_type = message_type;
    message_handler_info->message_handler = message_handler;
    if (!cf_x_case_list_add_last
        (starclient->message_handler_info, message_handler_info)) {
      cf_x_core_trace("x_case_list_add_last");
      free(message_handler_info);
    }
  } else {
    cf_x_core_trace("malloc");
  }

  cf_x_case_set_iterate_start(starclient->clients);
  while ((nameclient = cf_x_case_set_iterate_next(starclient->clients))) {
    client = cf_x_core_nameobject_get_object(nameclient);
    cf_x_net_client_system_register_message_handler
      (client, engine_id, message_type, message_handler);
  }
}

cf_x_core_bool_t cf_x_net_star_client_system_send_message_to_any_arm
(cf_x_net_star_client_system_t *starclient, cf_x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  cf_x_net_client_system_t *random_client;
  cf_x_core_bool_t success;
  cf_x_core_bool_t sent_to_client;

  random_client = get_random_client(starclient);
  if (random_client) {
    if (cf_x_net_client_system_send_message(random_client, message)) {
      sent_to_client = cf_x_core_bool_true;
    } else {
      sent_to_client = cf_x_core_bool_false;
    }
  } else {
    sent_to_client = cf_x_core_bool_false;
  }

  if (sent_to_client) {
    success = cf_x_core_bool_true;
  } else {
    if (put_messinferno_in_unsent_queue(starclient, message)) {
      success = cf_x_core_bool_true;
    } else {
      printf("[star] unsent message queue is full, unable to send message\n");
      success = cf_x_core_bool_false;
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_net_star_client_system_send_message_to_all_arms
(cf_x_net_star_client_system_t *starclient, cf_x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  cf_x_core_bool_t success;
  cf_x_core_nameobject_t *nameclient;
  cf_x_net_client_system_t *client;
  cf_x_core_message_t *message_copy;

  success = cf_x_core_bool_true;

  cf_x_case_set_iterate_start(starclient->clients);
  while ((nameclient = cf_x_case_set_iterate_next(starclient->clients))) {
    client = cf_x_core_nameobject_get_object(nameclient);
    message_copy = cf_x_core_message_copy(message);
    if (message_copy) {
      if (!cf_x_net_client_system_send_message(client, message_copy)) {
        success = cf_x_core_bool_false;
        cf_x_core_message_destroy(message_copy);
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("x_core_message_copy");
    }
  }

  cf_x_core_message_destroy(message);

  return success;
}

void cf_x_net_star_client_system_set_unsent_messages_queue_size
(cf_x_net_star_client_system_t *starclient, unsigned long queue_size)
{
  starclient->unsent_messages_queue_size = queue_size;
}

cf_x_core_bool_t cf_x_net_star_client_system_star_available(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_bool_t available;
  unsigned long client_count;

  client_count = cf_x_case_set_get_size(starclient->clients);

  if (client_count > 0) {
    available = cf_x_core_bool_true;
  } else {
    available = cf_x_core_bool_false;
  }

  return available;
}

void print_stats(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  unsigned long star_arm_count;

  star_arm_count = cf_x_case_set_get_size(starclient->clients);

  printf("[star] connected to %lu arms of the star\n", star_arm_count);
  starclient->last_star_arm_count = star_arm_count;
  starclient->need_to_print_stats = cf_x_core_bool_false;
}

cf_x_core_bool_t put_messinferno_in_unsent_queue(cf_x_net_star_client_system_t *starclient,
    cf_x_core_message_t *message)
{
  assert(starclient);
  assert(message);
  unsigned long messages_in_queue;
  cf_x_core_bool_t success;

  messages_in_queue = cf_x_case_list_get_size(starclient->unsent_messages);

  if (starclient->unsent_messages_queue_size > messages_in_queue) {
    if (cf_x_case_list_add_last(starclient->unsent_messages, message)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
    }
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

void re_route_unsent_messages(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_message_t *message;
  cf_x_case_list_t *unsent_messages;

  unsent_messages = starclient->unsent_messages;

  cf_x_case_list_iterate_start(unsent_messages);
  while ((message = cf_x_case_list_iterate_next(unsent_messages))) {
    cf_x_net_star_client_system_send_message_to_any_arm(starclient, message);
    cf_x_case_list_iterate_remove(unsent_messages);
  }
}

void rebuild_client_list(cf_x_net_star_client_system_t *starclient)
{
  assert(starclient);
  cf_x_core_nameobject_t *nameclient;

  cf_x_case_list_clear(starclient->client_list);
  cf_x_case_set_iterate_start(starclient->clients);
  while ((nameclient = cf_x_case_set_iterate_next(starclient->clients))) {
    cf_x_case_list_add_last(starclient->client_list, nameclient);
  }
}

cf_x_core_bool_t register_engines_witx_client(cf_x_net_star_client_system_t *starclient,
    cf_x_net_client_system_t *client)
{
  assert(starclient);
  assert(client);
  cf_x_core_bool_t success;
  unsigned long engine_id;
  long message_type_count_long;
  unsigned long message_type_count;

  success = cf_x_core_bool_true;

  for (engine_id = 0; engine_id < CF_X_NET_ENGINE_TYPE_COUNT; engine_id++) {
    message_type_count_long = *(starclient->message_type_counts + engine_id);
    if (-1 != message_type_count_long) {
      message_type_count = (unsigned long) message_type_count_long;
      if (!cf_x_net_client_system_register_engine
          (client, engine_id, message_type_count)) {
        cf_x_core_trace("x_net_client_register_engine");
        success = cf_x_core_bool_false;
      }
    }
  }

  return success;
}

void register_message_handlers_witx_client(cf_x_net_star_client_system_t *starclient,
    cf_x_net_client_system_t *client)
{
  assert(starclient);
  assert(client);
  message_handler_info_t *message_handler_info;

  cf_x_case_list_iterate_start(starclient->message_handler_info);
  while ((message_handler_info
          = cf_x_case_list_iterate_next(starclient->message_handler_info))) {
    cf_x_net_client_system_register_message_handler(client,
        message_handler_info->engine_id, message_handler_info->message_type,
        message_handler_info->message_handler);
  }
}

void take_unsent_messages(cf_x_net_star_client_system_t *starclient,
    cf_x_net_client_system_t *client)
{
  assert(starclient);
  assert(client);
  cf_x_case_list_t *unsent_messages;
  cf_x_core_message_t *message;
  unsigned long discarded_message_count;

  discarded_message_count = 0;

  unsent_messages = cf_x_net_client_system_take_unsent_messages(client);
  cf_x_case_list_iterate_start(unsent_messages);
  while ((message = cf_x_case_list_iterate_next(unsent_messages))) {
    if (!put_messinferno_in_unsent_queue(starclient, message)) {
      discarded_message_count++;
      cf_x_core_message_destroy(message);
    }
  }
  cf_x_case_list_destroy(unsent_messages);

  if (discarded_message_count > 0) {
    printf("[star] unsent message queue is full, unable to take %lu "
        "unsent messages from client\n", discarded_message_count);
  }
}
