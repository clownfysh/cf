#ifndef cf_x_net_starclient_conf_h
#define cf_x_net_starclient_conf_h

#include "cf/x/config/file.h"

struct cf_x_net_starclient_conf_t {
  cf_x_config_file_t *conf;

  cf_x_case_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_port;
};
typedef struct cf_x_net_starclient_conf_t cf_x_net_starclient_conf_t;

cf_x_net_starclient_conf_t *cf_x_net_starclient_conf_create(char *conf_filename);

void cf_x_net_starclient_conf_destroy(cf_x_net_starclient_conf_t *starclient_conf);

#endif
