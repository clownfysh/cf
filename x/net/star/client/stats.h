#ifndef cf_x_net_starclient_stats_h
#define cf_x_net_starclient_stats_h

#include "cf/x/core/message.h"

struct cf_x_net_starclient_stats_t {
  unsigned long connected_server_count;
};
typedef struct cf_x_net_starclient_stats_t cf_x_net_starclient_stats_t;

cf_x_core_bool_t cf_x_net_starclient_add_stats_to_message
(cf_x_net_starclient_stats_t *starclient_stats, cf_x_core_message_t *message);

cf_x_net_starclient_stats_t *cf_x_net_starclient_create_stats_from_message
(cf_x_core_message_t *message);

#endif
