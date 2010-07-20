#include "x/core/tools.h"
#include "x/core/unsigned_long.h"

int x_core_unsigned_long_compare(void *unsigned_long_object_a,
    void *unsigned_long_object_b)
{
  unsigned long *long_a;
  unsigned long *long_b;
  int compare;

  long_a = unsigned_long_object_a;
  long_b = unsigned_long_object_b;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *x_core_unsigned_long_copy
(void *unsigned_long_object)
{
  assert(unsigned_long_object);
  unsigned long *long_value;
  unsigned long *long_copy;

  long_value = unsigned_long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    x_core_trace("malloc");
  }

  return long_copy;
}

void x_core_unsigned_long_destroy(void *unsigned_long_object)
{
  free(unsigned_long_object);
}

char *x_core_unsigned_long_get_as_string(void *unsigned_long_object)
{
  assert(unsigned_long_object);
  unsigned long *unsigned_long;
  char *string;

  unsigned_long = unsigned_long_object;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%lu", *unsigned_long);
  } else {
    x_core_trace_exit("malloc");
  }

  return string;
}

void x_core_unsigned_long_init_objectey(x_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = x_core_unsigned_long_get_as_string;
  objectey->compare = x_core_unsigned_long_compare;
  objectey->copy = x_core_unsigned_long_copy;
  objectey->destroy = x_core_unsigned_long_destroy;
}
