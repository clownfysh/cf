#include "x/core/long.h"
#include "x/core/message.h"
#include "x/core/tools.h"

x_core_bool_t x_core_long_add_to_message(void *long_object,
    x_core_message_t *message)
{
  return x_core_message_add_long(message, long_object);
}

int x_core_long_compare(void *long_object_a,
    void *long_object_b)
{
  long *long_a;
  long *long_b;
  int compare;

  long_a = long_object_a;
  long_b = long_object_b;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *x_core_long_copy(void *long_object)
{
  assert(long_object);
  long *long_value;
  long *long_copy;

  long_value = long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    x_core_trace("malloc");
  }

  return long_copy;
}

void *x_core_long_create_from_message(x_core_message_t *message)
{
  return x_core_message_take_long(message);
}

void x_core_long_destroy(void *long_object)
{
  assert(long_object);
  free(long_object);
}

x_core_bool_t x_core_long_equal(void *long_a_object, void *long_b_object)
{
  assert(long_a_object);
  assert(long_b_object);
  unsigned char *long_a = long_a_object;  /*  yeah, I know we're gettin' crazy
                                              with the Cheeze Whiz here...just
                                              experimenting...don't rely on
                                              this code  */
  unsigned char *long_b = long_b_object;
  x_core_bool_t equal = x_core_bool_true;
  unsigned char i;

  for (i = 0; i < 4; i++) {
    if (*(long_a + i) != *(long_b + i)) {
      equal = x_core_bool_false;
      break;
    }
  }

  return equal;
}

char *x_core_long_get_as_string(void *long_object)
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

void x_core_long_init_objectey(x_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = x_core_long_get_as_string;
  objectey->compare = x_core_long_compare;
  objectey->copy = x_core_long_copy;
  objectey->destroy = x_core_long_destroy;
}

unsigned long x_core_long_mod(void *long_object, unsigned long divisor)
{
  assert(long_object);
  unsigned long *l = long_object;
  return *l % divisor;
}

void x_core_long_print(void *long_object)
{
  x_core_print(long_object, x_core_long_get_as_string);
}
