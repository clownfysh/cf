#include "x/core/tools.h"
#include "x/net/node_conf.h"

#define DEFAULT_NODE_IP "127.0.0.1"
#define DEFAULT_MAX_THREADS 2
#define DEFAULT_NODE_PORT 67
#define DEFAULT_PEER_NODE_PORT_MIN 8000
#define DEFAULT_PEER_NODE_PORT_MAX 9000

static x_core_bool_t x_net_node_conf_create_node_ip
(x_net_node_conf_t *node_conf);

static x_core_bool_t x_net_node_conf_create_node_max_threads
(x_net_node_conf_t *node_conf);

static x_core_bool_t x_net_node_conf_create_node_port
(x_net_node_conf_t *node_conf);

static x_core_bool_t x_net_node_conf_create_peer_node_ips
(x_net_node_conf_t *node_conf);

static x_core_bool_t x_net_node_conf_create_peer_node_port_range
(x_net_node_conf_t *node_conf);

x_net_node_conf_t *x_net_node_conf_create(char *conf_filename)
{
  assert(conf_filename);
  x_net_node_conf_t *node_conf;
  x_core_bool_t so_far_so_good;

  node_conf = malloc(sizeof *node_conf);
  if (node_conf) {
    node_conf->conf = x_config_file_create(conf_filename);
    if (node_conf->conf) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("new");
    }
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_node_conf_create_node_ip(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_node_conf_create_node_max_threads(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_node_conf_create_node_port(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_node_conf_create_peer_node_ips(node_conf);
  }

  if (so_far_so_good) {
    so_far_so_good = x_net_node_conf_create_peer_node_port_range(node_conf);
  }

  if (!so_far_so_good && node_conf) {
    if (node_conf->conf) {
      x_config_file_destroy(node_conf->conf);
    }
    free(node_conf);
    node_conf = NULL;
  }

  return node_conf;
}

x_core_bool_t x_net_node_conf_create_node_ip(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_core_bool_t success;

  if (x_config_file_find_as_string
      (node_conf->conf, "node-ip", &node_conf->node_ip, DEFAULT_NODE_IP)) {
    success = x_core_bool_true;
  } else {
    printf("node conf doesn't specify node-ip\n");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_node_conf_create_node_max_threads
(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_core_bool_t success;

  success = x_config_file_find_as_unsigned_short(node_conf->conf,
      "node-max-threads", &node_conf->node_max_threads, DEFAULT_MAX_THREADS);
  if (success) {
    success = x_core_bool_true;
  } else {
    printf("node conf doesn't specify node-max-threads\n");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_node_conf_create_node_port(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_core_bool_t success;

  if (x_config_file_find_as_unsigned_short(node_conf->conf, "node-port",
          &node_conf->node_port, DEFAULT_NODE_PORT)) {
    success = x_core_bool_true;
  } else {
    printf("node conf doesn't specify node-port\n");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_node_conf_create_peer_node_ips
(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_core_bool_t success;

  if (x_config_file_find_list_as_strings
      (node_conf->conf, "peer-node-ips[list]", &node_conf->peer_node_ips)) {
    success = x_core_bool_true;
  } else {
    printf("node conf doesn't specify peer-node-ip[list]\n");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t x_net_node_conf_create_peer_node_port_range
(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_core_bool_t success;
  x_core_bool_t found_peer_node_port_min;
  x_core_bool_t found_peer_node_port_max;

  found_peer_node_port_min = x_config_file_find_as_unsigned_short
    (node_conf->conf, "peer-node-port-min", &node_conf->peer_node_port_min,
        DEFAULT_PEER_NODE_PORT_MIN);
  found_peer_node_port_max = x_config_file_find_as_unsigned_short
    (node_conf->conf, "peer-node-port-max", &node_conf->peer_node_port_max,
        DEFAULT_PEER_NODE_PORT_MAX);
  if (found_peer_node_port_min && found_peer_node_port_max) {
    success = x_core_bool_true;
  } else {
    printf("node conf doesn't specify both peer-node-port-min and "
        "peer-node-port-max\n");
    success = x_core_bool_false;
  }

  return success;
}

void x_net_node_conf_destroy(x_net_node_conf_t *node_conf)
{
  assert(node_conf);
  x_config_file_destroy(node_conf->conf);
  free(node_conf);
}
