#include "x/config/file.h"
#include "x/core/tools.h"

#define DEFAULT_PORT_MIN 8000
#define DEFAULT_PORT_MAX 9000

int main(int argc, char *argv[])
{
  x_config_file_t *conf;
  unsigned short port_min;
  unsigned short port_max;
  x_container_list_t *node_ip_list;
  char *node_ip;

  conf = x_config_file_create("x/config/file_test.conf");
  if (x_core_bool_true) {

    if (!x_config_file_find_as_unsigned_short
        (conf, "port_min", &port_min, DEFAULT_PORT_MIN)) {
      x_core_trace_exit("x_config_file_find_as_unsigned_short");
    }

    if (!x_config_file_find_as_unsigned_short
        (conf, "port_max", &port_max, DEFAULT_PORT_MAX)) {
      x_core_trace_exit("x_config_file_find_as_unsigned_short");
    }

    if (!x_config_file_find_list_as_strings
        (conf, "node_ips[list]", &node_ip_list)) {
      x_core_trace_exit("x_config_file_find_list");
    }

    printf("port_min:%i\n", port_min);
    printf("port_max:%i\n", port_max);
    if (node_ip_list) {
      x_container_list_iterate_start(node_ip_list);
      while ((node_ip = x_container_list_iterate_next(node_ip_list))) {
        printf("node_ip:%s\n", node_ip);
      }
    }
  }
  x_config_file_destroy(conf);

  return 0;
}
