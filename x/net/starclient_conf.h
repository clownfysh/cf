#ifndef x_net_starclient_conf_h
#define x_net_starclient_conf_h

#include "x/config/file.h"

struct x_net_starclient_conf_t {
  x_config_file_t *conf;

  x_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_port;
};
typedef struct x_net_starclient_conf_t x_net_starclient_conf_t;

x_net_starclient_conf_t *x_net_starclient_conf_create(char *conf_filename);

void x_net_starclient_conf_destroy(x_net_starclient_conf_t *starclient_conf);

#endif
