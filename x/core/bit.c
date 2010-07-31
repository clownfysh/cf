#include "cf/x/core/bit.h"
#include "cf/x/core/tools.h"

int cf_x_core_bit_compare(void *bit_object_a,
    void *bit_object_b)
{
  cf_x_core_bit_t *bit_a;
  cf_x_core_bit_t *bit_b;
  int compare;

  bit_a = bit_object_a;
  bit_b = bit_object_b;
  if (*bit_a > *bit_b) {
    compare = 1;
  } else if (*bit_a < *bit_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *cf_x_core_bit_copy(void *bit_object)
{
  assert(bit_object);
  cf_x_core_bit_t *bit;
  cf_x_core_bit_t *bit_copy;

  bit = bit_object;
  bit_copy = malloc(sizeof *bit_copy);
  if (bit_copy) {
    *bit_copy = *bit;
  } else {
    cf_x_core_trace("mallloc");
  }

  return bit_copy;
}

void cf_x_core_bit_destroy(void *bit_object)
{
  assert(bit_object);
  free(bit_object);
}
