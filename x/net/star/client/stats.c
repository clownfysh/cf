#include "cf/x/core/tools.h"
#include "cf/x/net/star/client/stats.h"

cf_x_core_bool_t cf_x_net_star_client_system_add_stats_to_message
(cf_x_net_star_client_stats_t *starclient_stats, cf_x_core_message_t *message)
{
  assert(message);
  assert(starclient_stats);
  cf_x_core_bool_t success;

  if (cf_x_core_message_add_long
      (message, &starclient_stats->connected_server_count)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_net_star_client_stats_t *cf_x_net_star_client_system_create_stats_from_message
(cf_x_core_message_t *message)
{
  assert(message);
  cf_x_net_star_client_stats_t *starclient_stats;

  starclient_stats = malloc(sizeof *starclient_stats);
  if (starclient_stats) {
    starclient_stats->connected_server_count
      = cf_x_core_message_take_long_value(message);
  } else {
    cf_x_core_trace("malloc");
  }

  return starclient_stats;
}
