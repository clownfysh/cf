#ifndef x_net_server_stats_h
#define x_net_server_stats_h

#include "x/core/message.h"

struct x_net_server_stats_t {
  unsigned long engine_count;
  unsigned long server_send_message_failure_count;
  unsigned long engine_cant_handle_message_now_count;
  unsigned long engine_handled_message_count;
  unsigned long engine_cant_handle_message_count;
  unsigned long discarded_message_for_nonexistent_client_count;
  unsigned long discarded_message_for_unregistered_engine_count;
  unsigned long discarded_message_engine_inbox_add_failed_count;
  unsigned long uptime_seconds;
  unsigned long connected_client_count;
  unsigned short max_threads;
  unsigned short thread_count;
};
typedef struct x_net_server_stats_t x_net_server_stats_t;

x_core_bool_t x_net_server_add_stats_to_message
(x_net_server_stats_t *server_stats, x_core_message_t *message);

x_net_server_stats_t *x_net_server_create_stats_from_message
(x_core_message_t *message);

void x_net_server_stats_init(x_net_server_stats_t *server_stats);

#endif
