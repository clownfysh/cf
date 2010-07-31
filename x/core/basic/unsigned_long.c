#include "cf/x/core/basic/unsigned_long.h"
#include "cf/x/core/tools.h"

int cf_x_core_basic_unsigned_long_compare(void *unsigned_long_a_object,
    void *unsigned_long_b_object)
{
  unsigned long *long_a;
  unsigned long *long_b;
  int compare;

  long_a = unsigned_long_a_object;
  long_b = unsigned_long_b_object;

  if (*long_a > *long_b) {
    compare = 1;
  } else if (*long_a < *long_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

cf_x_core_bool_t cf_x_core_basic_unsigned_long_compare_equal
(void *unsigned_long_a_object, void *unsigned_long_b_object)
{
  assert(unsigned_long_a_object);
  assert(unsigned_long_b_object);
  unsigned long *unsigned_long_a = unsigned_long_a_object;
  unsigned long *unsigned_long_b = unsigned_long_b_object;

  return *unsigned_long_a == *unsigned_long_b;
}

void *cf_x_core_basic_unsigned_long_copy(void *unsigned_long_object)
{
  assert(unsigned_long_object);
  unsigned long *long_value;
  unsigned long *long_copy;

  long_value = unsigned_long_object;
  long_copy = malloc(sizeof *long_copy);
  if (long_copy) {
    *long_copy = *long_value;
  } else {
    cf_x_core_trace("malloc");
  }

  return long_copy;
}

void cf_x_core_basic_unsigned_long_destroy(void *unsigned_long_object)
{
  free(unsigned_long_object);
}

char *cf_x_core_basic_unsigned_long_get_as_string(void *unsigned_long_object)
{
  assert(unsigned_long_object);
  unsigned long *unsigned_long;
  char *string;

  unsigned_long = unsigned_long_object;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%lu", *unsigned_long);
  } else {
    cf_x_core_trace_exit("malloc");
  }

  return string;
}

void cf_x_core_basic_unsigned_long_init_objectey(cf_x_core_objectey_t *objectey)
{
  assert(objectey);
  cf_x_core_objectey_init(objectey, cf_x_core_basic_unsigned_long_compare,
      cf_x_core_basic_unsigned_long_compare_equal,
      cf_x_core_basic_unsigned_long_copy,
      cf_x_core_basic_unsigned_long_destroy,
      cf_x_core_basic_unsigned_long_get_as_string,
      cf_x_core_basic_unsigned_long_mod);
}

unsigned long cf_x_core_basic_unsigned_long_mod(void *unsigned_long_object,
    unsigned long divisor)
{
  assert(unsigned_long_object);
  unsigned long *unsigned_long = unsigned_long_object;
  return *unsigned_long % divisor;
}
