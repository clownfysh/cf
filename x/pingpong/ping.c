#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/pingpong/ping.h"

struct cf_x_pingpong_ping_t {
  char *string;
};

cf_x_core_bool_t cf_x_pingpong_ping_add_to_message(void *ping_object,
    cf_x_core_message_t *message)
{
  assert(ping_object);
  assert(message);
  cf_x_core_bool_t success;
  cf_x_pingpong_ping_t *ping;

  ping = ping_object;

  success = cf_x_core_message_add_string(message, ping->string);

  return success;
}

cf_x_pingpong_ping_t *cf_x_pingpong_ping_create(char *string)
{
  assert(string);
  cf_x_pingpong_ping_t *ping;

  ping = malloc(sizeof *ping);
  if (ping) {
    ping->string = cf_x_core_string_copy(string);
    if (!ping->string) {
      cf_x_core_trace("x_core_string_copy");
      free(ping);
      ping = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return ping;
}

cf_x_pingpong_ping_t *cf_x_pingpong_ping_create_from_message(cf_x_core_message_t *message)
{
  assert(message);
  cf_x_pingpong_ping_t *ping;
  char *string;

  string = cf_x_core_message_take_string(message);
  if (string) {
    ping = cf_x_pingpong_ping_create(string);
    if (!ping) {
      cf_x_core_trace("x_pingpong_ping_create");
    }
    free(string);
  } else {
    cf_x_core_trace("x_core_message_take_string");
    ping = NULL;
  }

  return ping;
}

void cf_x_pingpong_ping_destroy(cf_x_pingpong_ping_t *ping)
{
  assert(ping);

  free(ping->string);
  free(ping);
}

char *cf_x_pingpong_ping_get_string(cf_x_pingpong_ping_t *ping)
{
  return ping->string;
}
