#include "x/core/bit.h"
#include "x/core/tools.h"

int x_core_bit_compare(void *bit_object_a,
    void *bit_object_b)
{
  x_core_bit_t *bit_a;
  x_core_bit_t *bit_b;
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

void *x_core_bit_copy(void *bit_object)
{
  assert(bit_object);
  x_core_bit_t *bit;
  x_core_bit_t *bit_copy;

  bit = bit_object;
  bit_copy = malloc(sizeof *bit_copy);
  if (bit_copy) {
    *bit_copy = *bit;
  } else {
    x_core_trace("mallloc");
  }

  return bit_copy;
}

void x_core_bit_destroy(void *bit_object)
{
  assert(bit_object);
  free(bit_object);
}
