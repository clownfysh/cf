#include "x/audit/log.h"
#include "x/config/tools.h"
#include "x/case/list.h"
#include "x/core/tools.h"
#include "x/net/enginey.h"
#include "x/net/hypermessage.h"
#include "x/net/hyperpost.h"
#include "x/net/post.h"
#include "x/net/server.h"
#include "x/net/testengine.h"
#include "x/net/testhyperengine.h"
#include "x/net/testmessage.h"

#define TEST_SERVER_MAX_THREADS 32
#define TEST_HYPERSERVER_MAX_THREADS 32

struct x_core_test_server_context_t {
  void *server_wide_shared_database;
};
typedef struct x_core_test_server_context_t x_core_test_server_context_t;

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  x_core_test_server_context_t custom_server_context;

  x_net_server_t *server;
  unsigned short port;
  x_net_enginey_t enginey;
  x_core_messagey_t messagey;
  x_net_postey_t postey;

  x_net_server_t *hyperserver;
  unsigned short hyperport;
  x_net_enginey_t hyperenginey;
  x_core_messagey_t hypermessagey;
  x_net_postey_t hyperpostey;

  x_audit_log_t *log;

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  port = 20000;

  x_net_enginey_init(&enginey, x_net_testengine_create,
      x_net_testengine_destroy, x_net_testengine_get_handler_for_message,
      x_net_testengine_maintain, x_net_testengine_run,
      x_net_testengine_start, x_net_testengine_stop);

  x_core_messagey_init(&messagey, x_core_message_destroy,
      x_core_message_get_client_socket, x_core_message_get_engine_id,
      x_core_message_get_type);

  x_net_postey_init(&postey, x_net_post_compare, x_net_post_create,
      x_net_post_create_decoy, x_net_post_destroy, x_net_post_destroy_decoy,
      x_net_post_get_last_receive_activity_time, x_net_post_get_socket,
      x_net_post_get_stats, x_net_post_receive_message,
      x_net_post_receive_messages, x_net_post_send_message,
      x_net_post_send_messages, x_net_post_is_socket_closed);

  server = x_net_server_create("standard", port, port, TEST_SERVER_MAX_THREADS,
      &messagey, &postey, X_NET_ENGINE_NO_GET_NAME_FUNCTION,
      X_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(server);

  if (!x_net_server_register_engine(server, X_NET_ENGINE_TEST,
          &custom_server_context, &enginey, 2, 2, X_NET_MAINTAIN_CONSTANTLY,
          X_NET_TESTMESSAGE_TYPE_COUNT)) {
    x_core_trace_exit("x_net_server_register_engine");
  }

  hyperport = 50000;

  x_net_enginey_init(&hyperenginey, x_net_testhyperengine_create,
      x_net_testhyperengine_destroy,
      x_net_testhyperengine_get_handler_for_message,
      x_net_testhyperengine_maintain, x_net_testhyperengine_run,
      x_net_testhyperengine_start, x_net_testhyperengine_stop);

  x_core_messagey_init(&hypermessagey, x_net_hypermessage_destroy,
      x_net_hypermessage_get_client_socket, x_net_hypermessage_get_engine_id,
      x_net_hypermessage_get_type);

  x_net_postey_init(&hyperpostey, x_net_hyperpost_compare,
      x_net_hyperpost_create, x_net_hyperpost_create_decoy,
      x_net_hyperpost_destroy, x_net_hyperpost_destroy_decoy,
      x_net_hyperpost_get_last_receive_activity_time,
      x_net_hyperpost_get_socket, x_net_hyperpost_get_stats,
      x_net_hyperpost_receive_message, x_net_hyperpost_receive_messages,
      x_net_hyperpost_send_message, x_net_hyperpost_send_messages,
      x_net_hyperpost_is_socket_closed);

  hyperserver = x_net_server_create("http", hyperport, hyperport,
      TEST_HYPERSERVER_MAX_THREADS, &hypermessagey, &hyperpostey,
      X_NET_ENGINE_NO_GET_NAME_FUNCTION, X_NET_SERVER_NO_CONFIG_SYSTEM, log);
  assert(hyperserver);

  if (!x_net_server_start(server)) {
    x_core_trace_exit("x_net_server_start");
  }

  if (!x_net_server_start(hyperserver)) {
    x_core_trace_exit("x_net_server_start");
  }

  x_net_server_destroy(server);
  x_net_server_destroy(hyperserver);
  x_audit_log_destroy(log);

  return 0;
}
