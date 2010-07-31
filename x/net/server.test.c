#include "cf/x/audit/log.h"
#include "cf/x/config/tools.h"
#include "cf/x/case/list.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/enginey.h"
#include "cf/x/net/hypermessage.h"
#include "cf/x/net/hyperpost.h"
#include "cf/x/net/post.h"
#include "cf/x/net/server.h"
#include "cf/x/net/testengine.h"
#include "cf/x/net/testhyperengine.h"
#include "cf/x/net/testmessage.h"

#define TEST_SERVER_MAX_THREADS 32
#define TEST_HYPERSERVER_MAX_THREADS 32

struct cf_x_core_test_server_context_t {
  void *server_wide_shared_database;
};
typedef struct cf_x_core_test_server_context_t cf_x_core_test_server_context_t;

int main(int argc, char *argv[])
{
  cf_x_config_disable_if_running_batcx_tests(argc, argv);

  cf_x_core_test_server_context_t custom_server_context;

  cf_x_net_server_t *server;
  unsigned short port;
  cf_x_net_enginey_t enginey;
  cf_x_core_messagey_t messagey;
  cf_x_net_postey_t postey;

  cf_x_net_server_t *hyperserver;
  unsigned short hyperport;
  cf_x_net_enginey_t hyperenginey;
  cf_x_core_messagey_t hypermessagey;
  cf_x_net_postey_t hyperpostey;

  cf_x_audit_log_t *log;

  log = cf_x_audit_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  port = 20000;

  cf_x_net_enginey_init(&enginey, cf_x_net_testengine_create,
      cf_x_net_testengine_destroy, cf_x_net_testengine_get_handler_for_message,
      cf_x_net_testengine_maintain, cf_x_net_testengine_run,
      cf_x_net_testengine_start, cf_x_net_testengine_stop);

  cf_x_core_messagey_init(&messagey, cf_x_core_message_destroy,
      cf_x_core_message_get_client_socket, cf_x_core_message_get_engine_id,
      cf_x_core_message_get_type);

  cf_x_net_postey_init(&postey, cf_x_net_post_compare, cf_x_net_post_create,
      cf_x_net_post_create_decoy, cf_x_net_post_destroy, cf_x_net_post_destroy_decoy,
      cf_x_net_post_get_last_receive_activity_time, cf_x_net_post_get_socket,
      cf_x_net_post_get_stats, cf_x_net_post_receive_message,
      cf_x_net_post_receive_messages, cf_x_net_post_send_message,
      cf_x_net_post_send_messages, cf_x_net_post_is_socket_closed);

  server = cf_x_net_server_create("standard", port, port, TEST_SERVER_MAX_THREADS,
      &messagey, &postey, CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION,
      CF_X_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(server);

  if (!cf_x_net_server_register_engine(server, CF_X_NET_ENGINE_TEST,
          &custom_server_context, &enginey, 2, 2, CF_X_NET_MAINTAIN_CONSTANTLY,
          CF_X_NET_TESTMESSAGE_TYPE_COUNT)) {
    cf_x_core_trace_exit("x_net_server_register_engine");
  }

  hyperport = 50000;

  cf_x_net_enginey_init(&hyperenginey, cf_x_net_testhyperengine_create,
      cf_x_net_testhyperengine_destroy,
      cf_x_net_testhyperengine_get_handler_for_message,
      cf_x_net_testhyperengine_maintain, cf_x_net_testhyperengine_run,
      cf_x_net_testhyperengine_start, cf_x_net_testhyperengine_stop);

  cf_x_core_messagey_init(&hypermessagey, cf_x_net_hypermessage_destroy,
      cf_x_net_hypermessage_get_client_socket, cf_x_net_hypermessage_get_engine_id,
      cf_x_net_hypermessage_get_type);

  cf_x_net_postey_init(&hyperpostey, cf_x_net_hyperpost_compare,
      cf_x_net_hyperpost_create, cf_x_net_hyperpost_create_decoy,
      cf_x_net_hyperpost_destroy, cf_x_net_hyperpost_destroy_decoy,
      cf_x_net_hyperpost_get_last_receive_activity_time,
      cf_x_net_hyperpost_get_socket, cf_x_net_hyperpost_get_stats,
      cf_x_net_hyperpost_receive_message, cf_x_net_hyperpost_receive_messages,
      cf_x_net_hyperpost_send_message, cf_x_net_hyperpost_send_messages,
      cf_x_net_hyperpost_is_socket_closed);

  hyperserver = cf_x_net_server_create("http", hyperport, hyperport,
      TEST_HYPERSERVER_MAX_THREADS, &hypermessagey, &hyperpostey,
      CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, CF_X_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(hyperserver);

  if (!cf_x_net_server_start(server)) {
    cf_x_core_trace_exit("x_net_server_start");
  }

  if (!cf_x_net_server_start(hyperserver)) {
    cf_x_core_trace_exit("x_net_server_start");
  }

  cf_x_net_server_destroy(server);
  cf_x_net_server_destroy(hyperserver);
  cf_x_audit_log_destroy(log);

  return 0;
}
