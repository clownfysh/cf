#include "x/audit/log.h"
#include "x/container/list.h"
#include "x/core/messagey.h"
#include "x/core/tools.h"
#include "x/core/types.h"
#include "x/net/enginey.h"
#include "x/net/post.h"
#include "x/net/postey.h"
#include "x/net/server.h"
#include "x/pingpong/engine.h"
#include "x/pingpong/message.h"

#define PING_SERVER_MAX_THREADS 32

struct ping_server_context_t {
  void *server_wide_shared_database;
};
typedef struct ping_server_context_t ping_server_context_t;

static void print_use_exit();

void print_use_exit()
{
  printf("\n    use: ping_server server_port\n\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  x_net_server_t *server;
  ping_server_context_t custom_server_context;
  unsigned short server_port;
  x_net_enginey_t enginey;
  x_core_messagey_t messagey;
  x_net_postey_t postey;
  x_audit_log_t *log;

  if (argc < 2) {
    print_use_exit();
  }
  server_port = atoi(argv[1]);

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  x_net_enginey_init(&enginey, x_pingpong_engine_create, x_pingpong_engine_destroy,
      x_pingpong_engine_get_handler_for_message, x_pingpong_engine_maintain,
      x_pingpong_engine_run, x_pingpong_engine_start, x_pingpong_engine_stop);

  x_core_messagey_init(&messagey, x_core_message_destroy,
      x_core_message_get_client_socket, x_core_message_get_engine_id,
      x_core_message_get_type);

  x_net_postey_init(&postey, x_net_post_compare, x_net_post_create,
      x_net_post_create_decoy, x_net_post_destroy, x_net_post_destroy_decoy,
      x_net_post_get_last_receive_activity_time, x_net_post_get_socket,
      x_net_post_get_stats, x_net_post_receive_message,
      x_net_post_receive_messages, x_net_post_send_message,
      x_net_post_send_messages, x_net_post_is_socket_closed);

  server = x_net_server_create("ping", server_port, server_port,
      PING_SERVER_MAX_THREADS, &messagey, &postey,
      X_NET_ENGINE_NO_GET_NAME_FUNCTION, X_NET_SERVER_NO_CONFIG_SYSTEM, log);
  if (!server) {
    x_core_trace_exit("x_net_server_create");
  }

  if (!x_net_server_register_engine(server, X_NET_ENGINE_PING,
          &custom_server_context, &enginey, 2, 2, X_NET_MAINTAIN_1_MINUTE,
          X_PINGPONG_MESSAGE_TYPE_COUNT)) {
    x_core_trace_exit("x_net_server_register_engine");
  }

  if (!x_net_server_start(server)) {
    x_core_trace_exit("x_net_server_start");
  }

  x_net_server_destroy(server);
  x_audit_log_destroy(log);

  return 0;
}
