#ifndef x_net_starclient_stats_h
#define x_net_starclient_stats_h

#include "x/core/message.h"

struct x_net_starclient_stats_t {
  unsigned long connected_server_count;
};
typedef struct x_net_starclient_stats_t x_net_starclient_stats_t;

x_core_bool_t x_net_starclient_add_stats_to_message
(x_net_starclient_stats_t *starclient_stats, x_core_message_t *message);

x_net_starclient_stats_t *x_net_starclient_create_stats_from_message
(x_core_message_t *message);

#endif
