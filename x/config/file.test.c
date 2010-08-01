#include "cf/x/config/file.h"
#include "cf/x/core/tools.h"

#define DEFAULT_PORT_MIN 8000
#define DEFAULT_PORT_MAX 9000

int main(int argc, char *argv[])
{
  cf_x_config_file_t *conf;
  unsigned short port_min;
  unsigned short port_max;
  cf_x_case_list_t *node_ip_list;
  char *node_ip;

  conf = cf_x_config_file_create("x/config/file.test.conf");
  if (cf_x_core_bool_true) {

    if (!cf_x_config_file_find_as_unsigned_short
        (conf, "port_min", &port_min, DEFAULT_PORT_MIN)) {
      cf_x_core_trace_exit("x_config_file_find_as_unsigned_short");
    }

    if (!cf_x_config_file_find_as_unsigned_short
        (conf, "port_max", &port_max, DEFAULT_PORT_MAX)) {
      cf_x_core_trace_exit("x_config_file_find_as_unsigned_short");
    }

    if (!cf_x_config_file_find_list_as_strings
        (conf, "node_ips[list]", &node_ip_list)) {
      cf_x_core_trace_exit("x_config_file_find_list");
    }

    printf("port_min:%i\n", port_min);
    printf("port_max:%i\n", port_max);
    if (node_ip_list) {
      cf_x_case_list_iterate_start(node_ip_list);
      while ((node_ip = cf_x_case_list_iterate_next(node_ip_list))) {
        printf("node_ip:%s\n", node_ip);
      }
    }
  }
  cf_x_config_file_destroy(conf);

  return 0;
}
