#include "cf/x/core/basic/short.h"
#include "cf/x/core/message.h"
#include "cf/x/core/tools.h"

cf_x_core_bool_t cf_x_core_basic_short_add_to_message(void *short_object,
    cf_x_core_message_t *message)
{
  return cf_x_core_message_add_short(message, short_object);
}

int cf_x_core_basic_short_compare(void *short_a_object, void *short_b_object)
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

void *cf_x_core_basic_short_copy(void *short_object)
{
  assert(short_object);
  short *short_value;
  short *short_copy;

  short_value = short_object;
  short_copy = malloc(sizeof *short_copy);
  if (short_copy) {
    *short_copy = *short_value;
  } else {
    cf_x_core_trace("malloc");
  }

  return short_copy;
}

void *cf_x_core_basic_short_take_from_message(cf_x_core_message_t *message)
{
  return cf_x_core_message_take_short(message);
}

void cf_x_core_basic_short_destroy(void *short_object)
{
  assert(short_object);
  free(short_object);
}

void cf_x_core_basic_short_print(void *short_object)
{
  short *short_value;

  short_value = short_object;
  printf("%d", *short_value);
}
