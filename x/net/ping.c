#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/net/ping.h"

struct x_net_ping_t {
  char *string;
};

x_core_bool_t x_net_ping_add_to_message(void *ping_object,
    x_core_message_t *message)
{
  assert(ping_object);
  assert(message);
  x_core_bool_t success;
  x_net_ping_t *ping;

  ping = ping_object;

  success = x_core_message_add_string(message, ping->string);

  return success;
}

x_net_ping_t *x_net_ping_create(const char *string)
{
  assert(string);
  x_net_ping_t *ping;

  ping = malloc(sizeof *ping);
  if (ping) {
    ping->string = strdup(string);
    if (!ping->string) {
      x_core_trace("strdup");
      free(ping);
      ping = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return ping;
}

x_net_ping_t *x_net_ping_create_from_message(x_core_message_t *message)
{
  assert(message);
  x_net_ping_t *ping;
  char *string;

  string = x_core_message_take_string(message);
  if (string) {
    ping = x_net_ping_create(string);
    if (!ping) {
      x_core_trace("x_net_ping_create");
    }
    free(string);
  } else {
    x_core_trace("x_core_message_take_string");
    ping = NULL;
  }

  return ping;
}

void x_net_ping_destroy(x_net_ping_t *ping)
{
  assert(ping);

  free(ping->string);
  free(ping);
}

char *x_net_ping_get_string(x_net_ping_t *ping)
{
  return ping->string;
}
