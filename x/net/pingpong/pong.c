#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/pingpong/pong.h"

struct cf_x_pingpong_pong_t {
  char *string;
};

cf_x_core_bool_t cf_x_pingpong_pong_add_to_message(void *pong_object,
    cf_x_core_message_t *message)
{
  assert(pong_object);
  assert(message);
  cf_x_core_bool_t success;
  cf_x_pingpong_pong_t *pong;

  pong = pong_object;

  if (cf_x_core_message_add_string(message, pong->string)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_core_message_add_string");
  }

  return success;
}

cf_x_pingpong_pong_t *cf_x_pingpong_pong_create(char *string)
{
  assert(string);
  cf_x_pingpong_pong_t *pong;

  pong = malloc(sizeof *pong);
  if (pong) {
    pong->string = cf_x_core_string_copy(string);
    if (!pong->string) {
      cf_x_core_trace("x_core_string_copy");
      free(pong);
      pong = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return pong;
}

cf_x_pingpong_pong_t *cf_x_pingpong_pong_create_from_message(cf_x_core_message_t *message)
{
  assert(message);
  char *string;
  cf_x_pingpong_pong_t *pong;

  string = cf_x_core_message_take_string(message);
  if (string) {
    pong = cf_x_pingpong_pong_create(string);
    if (!pong) {
      cf_x_core_trace("x_pingpong_pong_create");
    }
    free(string);
  } else {
    cf_x_core_trace("x_core_message_take_string");
    pong = NULL;
  }

  return pong;
}

void cf_x_pingpong_pong_destroy(cf_x_pingpong_pong_t *pong)
{
  assert(pong);

  free(pong->string);
  free(pong);
}
