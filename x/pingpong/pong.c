#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/pingpong/pong.h"

struct x_pingpong_pong_t {
  char *string;
};

x_core_bool_t x_pingpong_pong_add_to_message(void *pong_object,
    x_core_message_t *message)
{
  assert(pong_object);
  assert(message);
  x_core_bool_t success;
  x_pingpong_pong_t *pong;

  pong = pong_object;

  if (x_core_message_add_string(message, pong->string)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
    x_core_trace("x_core_message_add_string");
  }

  return success;
}

x_pingpong_pong_t *x_pingpong_pong_create(char *string)
{
  assert(string);
  x_pingpong_pong_t *pong;

  pong = malloc(sizeof *pong);
  if (pong) {
    pong->string = x_core_string_copy(string);
    if (!pong->string) {
      x_core_trace("x_core_string_copy");
      free(pong);
      pong = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return pong;
}

x_pingpong_pong_t *x_pingpong_pong_create_from_message(x_core_message_t *message)
{
  assert(message);
  char *string;
  x_pingpong_pong_t *pong;

  string = x_core_message_take_string(message);
  if (string) {
    pong = x_pingpong_pong_create(string);
    if (!pong) {
      x_core_trace("x_pingpong_pong_create");
    }
    free(string);
  } else {
    x_core_trace("x_core_message_take_string");
    pong = NULL;
  }

  return pong;
}

void x_pingpong_pong_destroy(x_pingpong_pong_t *pong)
{
  assert(pong);

  free(pong->string);
  free(pong);
}
