#include "x/core/constants.h"
#include "x/core/message.h"
#include "x/core/messagey.h"
#include "x/core/tools.h"
#include "x/net/client.h"
#include "x/net/node.h"
#include "x/net/post.h"
#include "x/net/postey.h"
#include "x/net/server.h"
#include "x/net/starclient.h"

struct x_net_node_t {
  x_net_server_t *server;
  x_core_messagey_t messagey;
  x_net_postey_t postey;

  x_net_starclient_t *starclient;

  pthread_t node_thread;

  x_audit_log_t *log;
};

static void x_net_node_create_rollback(x_net_node_t *node);

static void *node_thread(void *node_object);

x_net_node_t *x_net_node_create(char *node_ip, unsigned short node_min_port,
    unsigned short node_max_port, unsigned short node_max_threads,
    x_case_list_t *peer_node_ips, unsigned short peer_node_port_min,
    unsigned short peer_node_port_max, x_audit_log_t *log)
{
  assert(peer_node_ips);
  x_net_node_t *node;
  x_core_bool_t so_far_so_good;

  node = malloc(sizeof *node);
  if (node) {
    x_core_messagey_init(&node->messagey, x_core_message_destroy,
        x_core_message_get_client_socket,
        x_core_message_get_engine_id, x_core_message_get_type);
    x_net_postey_init(&node->postey, x_net_post_compare,
        x_net_post_create, x_net_post_create_decoy,
        x_net_post_destroy, x_net_post_destroy_decoy,
        x_net_post_get_last_receive_activity_time,
        x_net_post_get_socket, x_net_post_get_stats,
        x_net_post_receive_message, x_net_post_receive_messages,
        x_net_post_send_message, x_net_post_send_messages,
        x_net_post_is_socket_closed);
    node->log = log;
    so_far_so_good = x_core_bool_true;
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    node->server = x_net_server_create("standard", node_min_port,
        node_max_port, node_max_threads, &node->messagey, &node->postey,
        X_NET_ENGINE_NO_GET_NAME_FUNCTION, X_NET_SERVER_NO_CONFIG_SYSTEM, log);
    if (node->server) {
      so_far_so_good = x_core_bool_true;
    } else {
      x_core_trace_exit("x_net_server_create failed()");
      so_far_so_good = x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    node->starclient = x_net_starclient_create(peer_node_ips,
        peer_node_port_min, peer_node_port_max, node_ip, node_min_port,
        node_max_port, NULL, log);
    if (node->starclient) {
      so_far_so_good = x_core_bool_true;
    } else {
      x_core_trace_exit("x_net_starclient_create() failed()");
      so_far_so_good = x_core_bool_false;
    }
  }

  if (!so_far_so_good && node) {
    x_net_node_create_rollback(node);
    node = NULL;
  }

  return node;
}

void x_net_node_create_rollback(x_net_node_t *node)
{
  assert(node);

  if (node->server) {
    x_core_request_stop();
    x_net_server_destroy(node->server);
  }
  if (node->starclient) {
    x_net_starclient_destroy(node->starclient);
  }
  free(node);
}

void x_net_node_destroy(x_net_node_t *node)
{
  assert(node);

  /*
    TODO: take unprocessed and unsent messages from the server and reroute
    them to another node
  */

  if (0 != pthread_join(node->node_thread, NULL)) {
    x_core_trace("pthread_join");
  }
  x_net_server_destroy(node->server);
  x_net_starclient_destroy(node->starclient);
  free(node);
}

x_net_server_t *x_net_node_get_server(x_net_node_t *node)
{
  return node->server;
}

void x_net_node_get_server_stats(x_net_node_t *node,
    x_net_server_stats_t *server_stats)
{
  x_net_server_get_stats(node->server, server_stats);
}

x_net_starclient_t *x_net_node_get_starclient(x_net_node_t *node)
{
  return node->starclient;
}

x_core_bool_t x_net_node_register_engine(x_net_node_t *node,
    x_net_engine_id_t engine_id, void *custom_server_context,
    x_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, x_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  return x_net_server_register_engine(node->server, engine_id,
      custom_server_context, enginey, min_run_threads, max_run_threads,
      maintain_schedule, message_type_count);
}

void x_net_node_register_message_handler(x_net_node_t *node,
    x_net_engine_id_t engine_id, unsigned long message_type,
    x_net_server_handle_message_f message_handler)
{
  x_net_server_register_message_handler(node->server, engine_id, message_type,
      message_handler);
}

x_core_bool_t x_net_node_send_message_to_all_peers(x_net_node_t *node,
    x_core_message_t *message)
{
  return x_net_starclient_send_message_to_all_arms(node->starclient, message);
}

x_core_bool_t x_net_node_send_message_to_any_peer(x_net_node_t *node,
    x_core_message_t *message)
{
  return x_net_starclient_send_message_to_any_arm(node->starclient, message);
}

x_core_bool_t x_net_node_send_message_to_client(x_net_node_t *node,
    x_core_message_t *message)
{
  return x_net_server_send_message(node->server, message);
}

void x_net_node_set_server_unresponsive_client_time_seconds(x_net_node_t *node,
    unsigned long unresponsive_client_time_seconds)
{
  x_net_server_set_unresponsive_client_time_seconds
    (node->server, unresponsive_client_time_seconds);
}

x_core_bool_t x_net_node_start(x_net_node_t *node)
{
  assert(node);
  x_core_bool_t success;

  if (x_net_server_start(node->server)) {
    x_net_starclient_connect(node->starclient);
    if (0 == pthread_create(&node->node_thread, NULL, node_thread, node)) {
      success = x_core_bool_true;
    } else {
      x_core_trace("pthread_create");
      success = x_core_bool_false;
    }
  } else {
    success = x_core_bool_false;
    x_core_request_stop();
  }

  return success;
}

void *node_thread(void *node_object)
{
  assert(node_object);
  x_net_node_t *node;

  node = node_object;

  while (!x_core_stop_requested) {
    x_net_starclient_process_messages(node->starclient);
    usleep(X_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
