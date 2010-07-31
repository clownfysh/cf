#include "cf/x/config/tools.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/node.h"
#include "cf/x/net/node_conf.h"

int main(int argc, char *argv[])
{
  cf_x_config_disable_if_running_batcx_tests(argc, argv);

  cf_x_net_node_t *node;
  cf_x_net_node_conf_t *node_conf;
  cf_x_audit_log_t *log;

  node_conf = cf_x_net_node_conf_create("x_net_node.conf");
  if (!node_conf) {
    cf_x_core_trace_exit("x_net_node_conf_create");
  }

  log = cf_x_audit_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  node = cf_x_net_node_create(node_conf->node_ip, node_conf->node_port,
      node_conf->node_port, node_conf->node_max_threads,
      node_conf->peer_node_ips, node_conf->peer_node_port_min,
      node_conf->peer_node_port_max, log);
  if (!node) {
    cf_x_core_trace_exit("x_net_node_create");
  }

  cf_x_net_node_set_server_unresponsive_client_time_seconds(node, 32);

  if (!cf_x_net_node_start(node)) {
    cf_x_core_trace_exit("x_net_node_start");
  }

  cf_x_net_node_destroy(node);
  cf_x_net_node_conf_destroy(node_conf);
  cf_x_audit_log_destroy(log);

  return 0;
}
