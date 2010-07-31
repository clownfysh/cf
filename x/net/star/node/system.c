#include "cf/x/core/constants.h"
#include "cf/x/core/message.h"
#include "cf/x/core/imessage.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/client/system.h"
#include "cf/x/net/star/node/system.h"
#include "cf/x/net/post/system.h"
#include "cf/x/net/post/ipost.h"
#include "cf/x/net/server/system.h"
#include "cf/x/net/star/client/system.h"

struct cf_x_net_star_node_system_t {
  cf_x_net_server_system_t *server;
  cf_x_core_imessage_t messagey;
  cf_x_net_post_ipost_t postey;

  cf_x_net_star_client_system_t *starclient;

  pthread_t node_thread;

  cf_x_core_log_t *log;
};

static void cf_x_net_star_node_system_create_rollback(cf_x_net_star_node_system_t *node);

static void *node_thread(void *node_object);

cf_x_net_star_node_system_t *cf_x_net_star_node_system_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    cf_x_case_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, cf_x_core_log_t *log)
{
  assert(peer_node_ips);
  cf_x_net_star_node_system_t *node;
  cf_x_core_bool_t so_far_so_good;

  node = malloc(sizeof *node);
  if (node) {
    cf_x_core_imessage_init(&node->messagey, cf_x_core_message_destroy,
        cf_x_core_message_get_client_socket,
        cf_x_core_message_get_engine_id, cf_x_core_message_get_type);
    cf_x_net_post_ipost_init(&node->postey, cf_x_net_post_system_compare,
        cf_x_net_post_system_create, cf_x_net_post_system_create_decoy,
        cf_x_net_post_system_destroy, cf_x_net_post_system_destroy_decoy,
        cf_x_net_post_system_get_last_receive_activity_time,
        cf_x_net_post_system_get_socket, cf_x_net_post_system_get_stats,
        cf_x_net_post_system_receive_message, cf_x_net_post_system_receive_messages,
        cf_x_net_post_system_send_message, cf_x_net_post_system_send_messages,
        cf_x_net_post_system_is_socket_closed);
    node->log = log;
    so_far_so_good = cf_x_core_bool_true;
  } else {
    cf_x_core_trace("malloc");
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    node->server = cf_x_net_server_system_create("standard", node_min_port,
        node_max_port, node_max_threads, &node->messagey, &node->postey,
        CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, CF_X_NET_SERVER_SYSTEM_NO_CONFIG_SYSTEM, log);
    if (node->server) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      cf_x_core_trace_exit("x_net_server_create failed()");
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    node->starclient = cf_x_net_star_client_system_create(peer_node_ips,
        peer_node_port_min, peer_node_port_max, node_ip, node_min_port,
        node_max_port, NULL, log);
    if (node->starclient) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      cf_x_core_trace_exit("x_net_starclient_create() failed()");
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (!so_far_so_good && node) {
    cf_x_net_star_node_system_create_rollback(node);
    node = NULL;
  }

  return node;
}

void cf_x_net_star_node_system_create_rollback(cf_x_net_star_node_system_t *node)
{
  assert(node);

  if (node->server) {
    cf_x_core_request_stop();
    cf_x_net_server_system_destroy(node->server);
  }
  if (node->starclient) {
    cf_x_net_star_client_system_destroy(node->starclient);
  }
  free(node);
}

void cf_x_net_star_node_system_destroy(cf_x_net_star_node_system_t *node)
{
  assert(node);

  /*
    TODO: take unprocessed and unsent messages from the server and reroute
    them to another node
  */

  if (0 != pthread_join(node->node_thread, NULL)) {
    cf_x_core_trace("pthread_join");
  }
  cf_x_net_server_system_destroy(node->server);
  cf_x_net_star_client_system_destroy(node->starclient);
  free(node);
}

cf_x_net_server_system_t *cf_x_net_star_node_system_get_server(cf_x_net_star_node_system_t *node)
{
  return node->server;
}

void cf_x_net_star_node_system_get_server_stats(cf_x_net_star_node_system_t *node,
    cf_x_net_server_stats_t *server_stats)
{
  cf_x_net_server_system_get_stats(node->server, server_stats);
}

cf_x_net_star_client_system_t *cf_x_net_star_node_system_get_starclient(cf_x_net_star_node_system_t *node)
{
  return node->starclient;
}

cf_x_core_bool_t cf_x_net_star_node_system_register_engine(cf_x_net_star_node_system_t *node,
    cf_x_net_engine_id_t engine_id, void *custom_server_context,
    cf_x_net_engine_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, cf_x_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  return cf_x_net_server_system_register_engine(node->server, engine_id,
      custom_server_context, enginey, min_run_threads, max_run_threads,
      maintain_schedule, message_type_count);
}

void cf_x_net_star_node_system_register_message_handler(cf_x_net_star_node_system_t *node,
    cf_x_net_engine_id_t engine_id, unsigned long message_type,
    cf_x_net_server_system_handle_message_f message_handler)
{
  cf_x_net_server_system_register_message_handler(node->server, engine_id, message_type,
      message_handler);
}

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_all_peers(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message)
{
  return cf_x_net_star_client_system_send_message_to_all_arms(node->starclient, message);
}

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_any_peer(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message)
{
  return cf_x_net_star_client_system_send_message_to_any_arm(node->starclient, message);
}

cf_x_core_bool_t cf_x_net_star_node_system_send_message_to_client(cf_x_net_star_node_system_t *node,
    cf_x_core_message_t *message)
{
  return cf_x_net_server_system_send_message(node->server, message);
}

void cf_x_net_star_node_system_set_server_unresponsive_client_time_seconds(cf_x_net_star_node_system_t *node,
    unsigned long unresponsive_client_time_seconds)
{
  cf_x_net_server_system_set_unresponsive_client_time_seconds
    (node->server, unresponsive_client_time_seconds);
}

cf_x_core_bool_t cf_x_net_star_node_system_start(cf_x_net_star_node_system_t *node)
{
  assert(node);
  cf_x_core_bool_t success;

  if (cf_x_net_server_system_start(node->server)) {
    cf_x_net_star_client_system_connect(node->starclient);
    if (0 == pthread_create(&node->node_thread, NULL, node_thread, node)) {
      success = cf_x_core_bool_true;
    } else {
      cf_x_core_trace("pthread_create");
      success = cf_x_core_bool_false;
    }
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_request_stop();
  }

  return success;
}

void *node_thread(void *node_object)
{
  assert(node_object);
  cf_x_net_star_node_system_t *node;

  node = node_object;

  while (!cf_x_core_stop_requested) {
    cf_x_net_star_client_system_process_messages(node->starclient);
    usleep(CF_X_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
