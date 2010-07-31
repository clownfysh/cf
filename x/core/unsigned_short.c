#include "cf/x/core/tools.h"
#include "cf/x/core/unsigned_short.h"

int cf_x_core_unsigned_short_compare(void *unsigned_short_a_object,
    void *unsigned_short_b_object)
{
  unsigned short *short_a;
  unsigned short *short_b;
  int compare;

  short_a = unsigned_short_a_object;
  short_b = unsigned_short_b_object;

  if (*short_a > *short_b) {
    compare = 1;
  } else if (*short_a < *short_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *cf_x_core_unsigned_short_copy
(void *unsigned_short_object)
{
  assert(unsigned_short_object);
  unsigned short *short_value;
  unsigned short *short_copy;

  short_value = unsigned_short_object;
  short_copy = malloc(sizeof *short_copy);
  if (short_copy) {
    *short_copy = *short_value;
  } else {
    cf_x_core_trace("malloc");
  }

  return short_copy;
}

void cf_x_core_unsigned_short_destroy(void *unsigned_short_object)
{
  assert(unsigned_short_object);
  free(unsigned_short_object);
}

void cf_x_core_unsigned_short_print(void *unsigned_short_object)
{
  unsigned short *short_value;

  short_value = unsigned_short_object;
  printf("%d", *short_value);
}
