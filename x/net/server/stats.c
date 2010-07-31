#include "cf/x/core/basic/long.h"
#include "cf/x/core/basic/short.h"
#include "cf/x/core/message.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/server/stats.h"

cf_x_core_bool_t cf_x_net_server_add_stats_to_message
(cf_x_net_server_stats_t *server_stats, cf_x_core_message_t *message)
{
  assert(message);
  assert(server_stats);
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  if (!cf_x_core_basic_long_add_to_message(&server_stats->engine_count, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->server_send_message_failure_count, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->engine_handled_message_count, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->discarded_message_for_nonexistent_client_count,
          message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->discarded_message_for_unregistered_engine_count,
          message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message
      (&server_stats->discarded_message_engine_inbox_add_failed_count,
          message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message(&server_stats->uptime_seconds, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_long_add_to_message(&server_stats->connected_client_count,
          message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_short_add_to_message(&server_stats->max_threads, message)) {
    success = cf_x_core_bool_false;
  }

  if (!cf_x_core_basic_short_add_to_message(&server_stats->thread_count, message)) {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_net_server_stats_t *cf_x_net_server_create_stats_from_message
(cf_x_core_message_t *message)
{
  assert(message);
  cf_x_net_server_stats_t *server_stats;

  server_stats = malloc(sizeof *server_stats);
  if (server_stats) {
    server_stats->engine_count = cf_x_core_message_take_long_value(message);

    server_stats->server_send_message_failure_count
      = cf_x_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_now_count
      = cf_x_core_message_take_long_value(message);

    server_stats->engine_handled_message_count
      = cf_x_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_count
      = cf_x_core_message_take_long_value(message);

    server_stats->discarded_message_for_nonexistent_client_count
      = cf_x_core_message_take_long_value(message);

    server_stats->discarded_message_for_unregistered_engine_count
      = cf_x_core_message_take_long_value(message);

    server_stats->discarded_message_engine_inbox_add_failed_count
      = cf_x_core_message_take_long_value(message);

    server_stats->uptime_seconds
      = cf_x_core_message_take_long_value (message);

    server_stats->connected_client_count
      = cf_x_core_message_take_long_value(message);

    server_stats->max_threads = cf_x_core_message_take_short_value(message);

    server_stats->thread_count = cf_x_core_message_take_short_value(message);
  } else {
    cf_x_core_trace("malloc");
  }

  return server_stats;
}

void cf_x_net_server_stats_init(cf_x_net_server_stats_t *server_stats)
{
  server_stats->server_send_message_failure_count = 0;
  server_stats->engine_cant_handle_message_now_count = 0;
  server_stats->engine_handled_message_count = 0;
  server_stats->engine_cant_handle_message_count = 0;
  server_stats->discarded_message_for_nonexistent_client_count = 0;
  server_stats->discarded_message_for_unregistered_engine_count = 0;
  server_stats->discarded_message_engine_inbox_add_failed_count = 0;
}
