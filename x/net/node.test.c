#include "x/config/tools.h"
#include "x/core/tools.h"
#include "x/net/node.h"
#include "x/net/node_conf.h"

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  x_net_node_t *node;
  x_net_node_conf_t *node_conf;
  x_audit_log_t *log;

  node_conf = x_net_node_conf_create("x_net_node.conf");
  if (!node_conf) {
    x_core_trace_exit("x_net_node_conf_create");
  }

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  node = x_net_node_create(node_conf->node_ip, node_conf->node_port,
      node_conf->node_port, node_conf->node_max_threads,
      node_conf->peer_node_ips, node_conf->peer_node_port_min,
      node_conf->peer_node_port_max, log);
  if (!node) {
    x_core_trace_exit("x_net_node_create");
  }

  x_net_node_set_server_unresponsive_client_time_seconds(node, 32);

  if (!x_net_node_start(node)) {
    x_core_trace_exit("x_net_node_start");
  }

  x_net_node_destroy(node);
  x_net_node_conf_destroy(node_conf);
  x_audit_log_destroy(log);

  return 0;
}
