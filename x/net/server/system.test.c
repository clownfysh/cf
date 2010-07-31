#include "cf/x/core/log.h"
#include "cf/x/config/tools.h"
#include "cf/x/case/list.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/engine/enginey.h"
#include "cf/x/net/http/message.h"
#include "cf/x/net/http/post.h"
#include "cf/x/net/post/system.h"
#include "cf/x/net/server/system.h"
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
  cf_x_config_disable_if_running_batch_tests(argc, argv);

  cf_x_core_test_server_context_t custom_server_context;

  cf_x_net_server_system_t *server;
  unsigned short port;
  cf_x_net_engine_enginey_t enginey;
  cf_x_core_imessage_t messagey;
  cf_x_net_post_ipost_t postey;

  cf_x_net_server_system_t *hyperserver;
  unsigned short hyperport;
  cf_x_net_engine_enginey_t hyperenginey;
  cf_x_core_imessage_t hypermessagey;
  cf_x_net_post_ipost_t hyperpostey;

  cf_x_core_log_t *log;

  log = cf_x_core_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  port = 20000;

  cf_x_net_engine_enginey_init(&enginey, cf_x_net_testengine_create,
      cf_x_net_testengine_destroy, cf_x_net_testengine_get_handler_for_message,
      cf_x_net_testengine_maintain, cf_x_net_testengine_run,
      cf_x_net_testengine_start, cf_x_net_testengine_stop);

  cf_x_core_imessage_init(&messagey, cf_x_core_message_destroy,
      cf_x_core_message_get_client_socket, cf_x_core_message_get_engine_id,
      cf_x_core_message_get_type);

  cf_x_net_post_ipost_init(&postey, cf_x_net_post_system_compare, cf_x_net_post_system_create,
      cf_x_net_post_system_create_decoy, cf_x_net_post_system_destroy, cf_x_net_post_system_destroy_decoy,
      cf_x_net_post_system_get_last_receive_activity_time, cf_x_net_post_system_get_socket,
      cf_x_net_post_system_get_stats, cf_x_net_post_system_receive_message,
      cf_x_net_post_system_receive_messages, cf_x_net_post_system_send_message,
      cf_x_net_post_system_send_messages, cf_x_net_post_system_is_socket_closed);

  server = cf_x_net_server_system_create("standard", port, port, TEST_SERVER_MAX_THREADS,
      &messagey, &postey, CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION,
      CF_X_NET_SERVER_SYSTEM_NO_CONFIG_SYSTEM, log);
  assert(server);

  if (!cf_x_net_server_system_register_engine(server, CF_X_NET_ENGINE_TEST,
          &custom_server_context, &enginey, 2, 2, CF_X_NET_MAINTAIN_CONSTANTLY,
          CF_X_NET_TESTMESSAGE_TYPE_COUNT)) {
    cf_x_core_trace_exit("x_net_server_register_engine");
  }

  hyperport = 50000;

  cf_x_net_engine_enginey_init(&hyperenginey, cf_x_net_testhyperengine_create,
      cf_x_net_testhyperengine_destroy,
      cf_x_net_testhyperengine_get_handler_for_message,
      cf_x_net_testhyperengine_maintain, cf_x_net_testhyperengine_run,
      cf_x_net_testhyperengine_start, cf_x_net_testhyperengine_stop);

  cf_x_core_imessage_init(&hypermessagey, cf_x_net_http_message_destroy,
      cf_x_net_http_message_get_client_socket, cf_x_net_http_message_get_engine_id,
      cf_x_net_http_message_get_type);

  cf_x_net_post_ipost_init(&hyperpostey, cf_x_net_http_post_compare,
      cf_x_net_http_post_create, cf_x_net_http_post_create_decoy,
      cf_x_net_http_post_destroy, cf_x_net_http_post_destroy_decoy,
      cf_x_net_http_post_get_last_receive_activity_time,
      cf_x_net_http_post_get_socket, cf_x_net_http_post_get_stats,
      cf_x_net_http_post_receive_message, cf_x_net_http_post_receive_messages,
      cf_x_net_http_post_send_message, cf_x_net_http_post_send_messages,
      cf_x_net_http_post_is_socket_closed);

  hyperserver = cf_x_net_server_system_create("http", hyperport, hyperport,
      TEST_HYPERSERVER_MAX_THREADS, &hypermessagey, &hyperpostey,
      CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, CF_X_NET_SERVER_SYSTEM_NO_CONFIG_SYSTEM, log);
  assert(hyperserver);

  if (!cf_x_net_server_system_start(server)) {
    cf_x_core_trace_exit("x_net_server_start");
  }

  if (!cf_x_net_server_system_start(hyperserver)) {
    cf_x_core_trace_exit("x_net_server_start");
  }

  cf_x_net_server_system_destroy(server);
  cf_x_net_server_system_destroy(hyperserver);
  cf_x_core_log_destroy(log);

  return 0;
}
