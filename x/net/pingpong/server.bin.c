#include "cf/x/case/list.h"
#include "cf/x/core/imessage.h"
#include "cf/x/core/log.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/engine/iengine.h"
#include "cf/x/net/pingpong/engine.h"
#include "cf/x/net/pingpong/message.h"
#include "cf/x/net/post/ipost.h"
#include "cf/x/net/post/system.h"
#include "cf/x/net/server/system.h"

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
  cf_x_net_server_system_t *server;
  ping_server_context_t custom_server_context;
  unsigned short server_port;
  cf_x_net_engine_iengine_t iengine;
  cf_x_core_imessage_t messagey;
  cf_x_net_post_ipost_t ipost;
  cf_x_core_log_t *log;

  if (argc < 2) {
    print_use_exit();
  }
  server_port = atoi(argv[1]);

  log = cf_x_core_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  cf_x_net_engine_iengine_init(&iengine, cf_x_pingpong_engine_create, cf_x_pingpong_engine_destroy,
      cf_x_pingpong_engine_get_handler_for_message, cf_x_pingpong_engine_maintain,
      cf_x_pingpong_engine_run, cf_x_pingpong_engine_start, cf_x_pingpong_engine_stop);

  cf_x_core_imessage_init(&messagey, cf_x_core_message_destroy,
      cf_x_core_message_get_client_socket, cf_x_core_message_get_engine_id,
      cf_x_core_message_get_type);

  cf_x_net_post_ipost_init(&ipost, cf_x_net_post_system_compare, cf_x_net_post_system_create,
      cf_x_net_post_system_create_decoy, cf_x_net_post_system_destroy, cf_x_net_post_system_destroy_decoy,
      cf_x_net_post_system_get_last_receive_activity_time, cf_x_net_post_system_get_socket,
      cf_x_net_post_system_get_stats, cf_x_net_post_system_receive_message,
      cf_x_net_post_system_receive_messages, cf_x_net_post_system_send_message,
      cf_x_net_post_system_send_messages, cf_x_net_post_system_is_socket_closed);

  server = cf_x_net_server_system_create("ping", server_port, server_port,
      PING_SERVER_MAX_THREADS, &messagey, &ipost,
      CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, CF_X_NET_SERVER_SYSTEM_NO_CONFIG_SYSTEM, log);
  if (!server) {
    cf_x_core_trace_exit("x_net_server_create");
  }

  if (!cf_x_net_server_system_register_engine(server, CF_X_NET_ENGINE_PING,
          &custom_server_context, &iengine, 2, 2, CF_X_NET_MAINTAIN_1_MINUTE,
          cf_x_PINGPONG_MESSAGE_TYPE_COUNT)) {
    cf_x_core_trace_exit("x_net_server_register_engine");
  }

  if (!cf_x_net_server_system_start(server)) {
    cf_x_core_trace_exit("x_net_server_start");
  }

  cf_x_net_server_system_destroy(server);
  cf_x_core_log_destroy(log);

  return 0;
}
