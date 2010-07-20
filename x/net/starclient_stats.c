#include "x/core/tools.h"
#include "x/net/starclient_stats.h"

x_core_bool_t x_net_starclient_add_stats_to_message
(x_net_starclient_stats_t *starclient_stats, x_core_message_t *message)
{
  assert(message);
  assert(starclient_stats);
  x_core_bool_t success;

  if (x_core_message_add_long
      (message, &starclient_stats->connected_server_count)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_net_starclient_stats_t *x_net_starclient_create_stats_from_message
(x_core_message_t *message)
{
  assert(message);
  x_net_starclient_stats_t *starclient_stats;

  starclient_stats = malloc(sizeof *starclient_stats);
  if (starclient_stats) {
    starclient_stats->connected_server_count
      = x_core_message_take_long_value(message);
  } else {
    x_core_trace("malloc");
  }

  return starclient_stats;
}
