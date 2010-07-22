#ifndef x_core_bit_h
#define x_core_bit_h

#include "x/core/object.h"

enum x_core_bit_t {
  x_core_bit_zero = 0,
  x_core_bit_one = 1,
};
typedef enum x_core_bit_t x_core_bit_t;

int x_core_bit_compare(void *bit_object_a, void *bit_object_b);

void *x_core_bit_copy(void *bit_object);

void x_core_bit_destroy(void *bit_object);

#endif