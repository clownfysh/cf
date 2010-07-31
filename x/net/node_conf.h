#ifndef x_net_node_conf_h
#define x_net_node_conf_h

#include "x/config/file.h"

struct x_net_node_conf_t {
  x_config_file_t *conf;

  char *node_ip;
  unsigned short node_port;
  unsigned short node_max_threads;

  x_case_list_t *peer_node_ips;
  unsigned short peer_node_port_min;
  unsigned short peer_node_port_max;
};
typedef struct x_net_node_conf_t x_net_node_conf_t;

x_net_node_conf_t *x_net_node_conf_create(char *conf_filename);

void x_net_node_conf_destroy(x_net_node_conf_t *node_conf);

#endif
