#include "x/core/long.h"
#include "x/core/message.h"
#include "x/core/short.h"
#include "x/core/tools.h"
#include "x/net/server_stats.h"

x_core_bool_t x_net_server_add_stats_to_message
(x_net_server_stats_t *server_stats, x_core_message_t *message)
{
  assert(message);
  assert(server_stats);
  x_core_bool_t success;

  success = x_core_bool_true;

  if (!x_core_long_add_to_message(&server_stats->engine_count, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->server_send_message_failure_count, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->engine_handled_message_count, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->discarded_message_for_nonexistent_client_count,
          message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->discarded_message_for_unregistered_engine_count,
          message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message
      (&server_stats->discarded_message_engine_inbox_add_failed_count,
          message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message(&server_stats->uptime_seconds, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_long_add_to_message(&server_stats->connected_client_count,
          message)) {
    success = x_core_bool_false;
  }

  if (!x_core_short_add_to_message(&server_stats->max_threads, message)) {
    success = x_core_bool_false;
  }

  if (!x_core_short_add_to_message(&server_stats->thread_count, message)) {
    success = x_core_bool_false;
  }

  return success;
}

x_net_server_stats_t *x_net_server_create_stats_from_message
(x_core_message_t *message)
{
  assert(message);
  x_net_server_stats_t *server_stats;

  server_stats = malloc(sizeof *server_stats);
  if (server_stats) {
    server_stats->engine_count = x_core_message_take_long_value(message);

    server_stats->server_send_message_failure_count
      = x_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_now_count
      = x_core_message_take_long_value(message);

    server_stats->engine_handled_message_count
      = x_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_count
      = x_core_message_take_long_value(message);

    server_stats->discarded_message_for_nonexistent_client_count
      = x_core_message_take_long_value(message);

    server_stats->discarded_message_for_unregistered_engine_count
      = x_core_message_take_long_value(message);

    server_stats->discarded_message_engine_inbox_add_failed_count
      = x_core_message_take_long_value(message);

    server_stats->uptime_seconds
      = x_core_message_take_long_value (message);

    server_stats->connected_client_count
      = x_core_message_take_long_value(message);

    server_stats->max_threads = x_core_message_take_short_value(message);

    server_stats->thread_count = x_core_message_take_short_value(message);
  } else {
    x_core_trace("malloc");
  }

  return server_stats;
}

void x_net_server_stats_init(x_net_server_stats_t *server_stats)
{
  server_stats->server_send_message_failure_count = 0;
  server_stats->engine_cant_handle_message_now_count = 0;
  server_stats->engine_handled_message_count = 0;
  server_stats->engine_cant_handle_message_count = 0;
  server_stats->discarded_message_for_nonexistent_client_count = 0;
  server_stats->discarded_message_for_unregistered_engine_count = 0;
  server_stats->discarded_message_engine_inbox_add_failed_count = 0;
}
