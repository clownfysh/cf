#include "cf/x/core/long.h"
#include "cf/x/core/message.h"
#include "cf/x/core/tools.h"

cf_x_core_bool_t cf_x_core_long_add_to_message(void *long_object,
    cf_x_core_message_t *message)
{
  return cf_x_core_message_add_long(message, long_object);
}

int cf_x_core_long_compare(void *long_a_object, void *long_b_object)
{
  long *long_a;
  long *long_b;
  int compare;

  long_a = long_a_object;
  long_b = long_b_object;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *cf_x_core_long_copy(void *long_object)
{
  assert(long_object);
  long *long_value;
  long *long_copy;

  long_value = long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    cf_x_core_trace("malloc");
  }

  return long_copy;
}

void *cf_x_core_long_create_from_message(cf_x_core_message_t *message)
{
  return cf_x_core_message_take_long(message);
}

void cf_x_core_long_destroy(void *long_object)
{
  assert(long_object);
  free(long_object);
}

cf_x_core_bool_t cf_x_core_long_equal(void *long_a_object, void *long_b_object)
{
  assert(long_a_object);
  assert(long_b_object);
  unsigned long *long_a = long_a_object;
  unsigned long *long_b = long_b_object;

  return *long_a == *long_b;
}

char *cf_x_core_long_get_as_string(void *long_object)
{
  assert(long_object);
  long *long_value;
  char *string;

  long_value = long_object;

  if (-1 == asprintf(&string, "%li", *long_value)) {
    string = NULL;
  }

  return string;
}

void cf_x_core_long_init_objectey(cf_x_core_objectey_t *objectey)
{
  assert(objectey);
  cf_x_core_objectey_init(objectey, cf_x_core_long_compare, cf_x_core_long_copy,
      cf_x_core_long_destroy, cf_x_core_long_equal, cf_x_core_long_get_as_string,
      cf_x_core_long_mod);
}

unsigned long cf_x_core_long_mod(void *long_object, unsigned long divisor)
{
  assert(long_object);
  unsigned long *l = long_object;
  return *l % divisor;
}

void cf_x_core_long_print(void *long_object)
{
  cf_x_core_print(long_object, cf_x_core_long_get_as_string);
}
