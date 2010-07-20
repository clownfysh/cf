#include "x/config/tools.h"
#include "x/core/constants.h"
#include "x/core/message.h"
#include "x/core/tools.h"
#include "x/net/client.h"
#include "x/net/ping.h"
#include "x/net/testengine.h"
#include "x/net/testmessage.h"

#define X_NET_CLIENT_TEST_SLEEP_MICROSECONDS X_CORE_STANDARD_SLEEP_MICROSECONDS

struct test_client_context_t {
  x_core_bool_t pong_received;
};
typedef struct test_client_context_t test_client_context_t;

static void handle_pong(void *custom_client_context,
    x_core_message_t *message);

void handle_pong(void *custom_client_context, x_core_message_t *message)
{
  assert(custom_client_context);
  assert(message);
  test_client_context_t *client_context;
  char *pong_string;

  client_context = custom_client_context;

  client_context->pong_received = x_core_bool_true;
  pong_string = x_core_message_take_string(message);
  printf("%s\n", pong_string);

  free(pong_string);
  assert(client_context->pong_received);
}

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  x_net_client_t *client;
  x_core_message_t *ping_message;
  test_client_context_t test_client_context;
  x_net_ping_t *ping;
  x_audit_log_t *log;

  test_client_context.pong_received = x_core_bool_false;

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  client = x_net_client_create("127.0.0.1", 20000, 20000,
      X_NET_ENGINE_NO_GET_NAME_FUNCTION, &test_client_context, log);
  if (!client) {
    x_core_trace_exit("x_net_client_create()");
  }
  x_net_client_register_engine(client, X_NET_ENGINE_TEST,
      X_NET_TESTMESSAGE_TYPE_COUNT);
  x_net_client_register_message_handler(client, X_NET_ENGINE_TEST,
      X_NET_TESTMESSAGE_PONG, handle_pong);

  ping = x_net_ping_create("penta");
  if (!ping) {
    x_core_trace_exit("x_net_ping_create");
  }

  ping_message = x_net_testmessage_create(X_CORE_MESSAGEY_NO_CLIENT_SOCKET,
      X_NET_TESTMESSAGE_PING);
  if (!ping_message) {
    x_core_trace_exit("x_net_testmessage_create");
  }

  if (!x_net_ping_add_to_message(ping, ping_message)) {
    x_core_trace_exit("x_net_ping_add_to_message");
  }

  x_net_ping_destroy(ping);

  if (x_net_client_send_message(client, ping_message)) {
    printf("client sent ping message\n");
  } else {
    x_core_message_destroy(ping_message);
    x_core_trace_exit("x_net_client_send_message");
  }

  do {
    x_net_client_process_messages(client);
    usleep(X_NET_CLIENT_TEST_SLEEP_MICROSECONDS);
  } while (!test_client_context.pong_received);

  x_net_client_destroy(client);
  x_audit_log_destroy(log);

  return 0;
}
