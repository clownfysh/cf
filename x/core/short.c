#include "x/core/message.h"
#include "x/core/short.h"
#include "x/core/tools.h"

x_core_bool_t x_core_short_add_to_message(void *short_object,
    x_core_message_t *message)
{
  return x_core_message_add_short(message, short_object);
}

int x_core_short_compare(void *short_a_object, void *short_b_object)
{
  short *short_a;
  short *short_b;
  int compare;

  short_a = short_a_object;
  short_b = short_b_object;

  if (*short_a > *short_b) {
    compare = 1;
  } else if (*short_a < *short_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *x_core_short_copy(void *short_object)
{
  assert(short_object);
  short *short_value;
  short *short_copy;

  short_value = short_object;
  short_copy = malloc(sizeof *short_copy);
  if (short_copy) {
    *short_copy = *short_value;
  } else {
    x_core_trace("malloc");
  }

  return short_copy;
}

void *x_core_short_take_from_message(x_core_message_t *message)
{
  return x_core_message_take_short(message);
}

void x_core_short_destroy(void *short_object)
{
  assert(short_object);
  free(short_object);
}

void x_core_short_print(void *short_object)
{
  short *short_value;

  short_value = short_object;
  printf("%d", *short_value);
}
