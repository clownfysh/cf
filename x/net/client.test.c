#include "cf/x/config/tools.h"
#include "cf/x/core/constants.h"
#include "cf/x/core/message.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/client.h"
#include "cf/x/net/ping.h"
#include "cf/x/net/testengine.h"
#include "cf/x/net/testmessage.h"

#define CF_X_NET_CLIENT_TEST_SLEEP_MICROSECONDS CF_X_CORE_STANDARD_SLEEP_MICROSECONDS

struct test_client_context_t {
  cf_x_core_bool_t pong_received;
};
typedef struct test_client_context_t test_client_context_t;

static void handle_pong(void *custom_client_context,
    cf_x_core_message_t *message);

void handle_pong(void *custom_client_context, cf_x_core_message_t *message)
{
  assert(custom_client_context);
  assert(message);
  test_client_context_t *client_context;
  char *pong_string;

  client_context = custom_client_context;

  client_context->pong_received = cf_x_core_bool_true;
  pong_string = cf_x_core_message_take_string(message);
  printf("%s\n", pong_string);

  free(pong_string);
  assert(client_context->pong_received);
}

int main(int argc, char *argv[])
{
  cf_x_config_disable_if_running_batcx_tests(argc, argv);

  cf_x_net_client_t *client;
  cf_x_core_message_t *ping_message;
  test_client_context_t test_client_context;
  cf_x_net_ping_t *ping;
  cf_x_audit_log_t *log;

  test_client_context.pong_received = cf_x_core_bool_false;

  log = cf_x_audit_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  client = cf_x_net_client_create("127.0.0.1", 20000, 20000,
      CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION, &test_client_context, log);
  if (!client) {
    cf_x_core_trace_exit("x_net_client_create()");
  }
  cf_x_net_client_register_engine(client, CF_X_NET_ENGINE_TEST,
      CF_X_NET_TESTMESSAGE_TYPE_COUNT);
  cf_x_net_client_register_message_handler(client, CF_X_NET_ENGINE_TEST,
      CF_X_NET_TESTMESSAGE_PONG, handle_pong);

  ping = cf_x_net_ping_create("penta");
  if (!ping) {
    cf_x_core_trace_exit("x_net_ping_create");
  }

  ping_message = cf_x_net_testmessage_create(CF_X_CORE_MESSAGEY_NO_CLIENT_SOCKET,
      CF_X_NET_TESTMESSAGE_PING);
  if (!ping_message) {
    cf_x_core_trace_exit("x_net_testmessage_create");
  }

  if (!cf_x_net_ping_add_to_message(ping, ping_message)) {
    cf_x_core_trace_exit("x_net_ping_add_to_message");
  }

  cf_x_net_ping_destroy(ping);

  if (cf_x_net_client_send_message(client, ping_message)) {
    printf("client sent ping message\n");
  } else {
    cf_x_core_message_destroy(ping_message);
    cf_x_core_trace_exit("x_net_client_send_message");
  }

  do {
    cf_x_net_client_process_messages(client);
    usleep(CF_X_NET_CLIENT_TEST_SLEEP_MICROSECONDS);
  } while (!test_client_context.pong_received);

  cf_x_net_client_destroy(client);
  cf_x_audit_log_destroy(log);

  return 0;
}
