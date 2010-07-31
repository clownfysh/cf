#ifndef cf_x_net_star_node_conf_h
#define cf_x_net_star_node_conf_h

#include "cf/x/config/file.h"

struct cf_x_net_star_node_conf_t {
  cf_x_config_file_t *conf;

  char *node_ip;
  unsigned short node_port;
  unsigned short node_max_threads;

  cf_x_case_list_t *peer_node_ips;
  unsigned short peer_node_port_min;
  unsigned short peer_node_port_max;
};
typedef struct cf_x_net_star_node_conf_t cf_x_net_star_node_conf_t;

cf_x_net_star_node_conf_t *cf_x_net_star_node_conf_create(char *conf_filename);

void cf_x_net_star_node_conf_destroy(cf_x_net_star_node_conf_t *node_conf);

#endif
