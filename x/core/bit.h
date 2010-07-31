#ifndef cf_x_core_bit_h
#define cf_x_core_bit_h

#include "cf/x/core/object.h"

enum cf_x_core_bit_t {
  cf_x_core_bit_zero = 0,
  cf_x_core_bit_one = 1,
};
typedef enum cf_x_core_bit_t cf_x_core_bit_t;

int cf_x_core_bit_compare(void *bit_a_object, void *bit_b_object);

void *cf_x_core_bit_copy(void *bit_object);

void cf_x_core_bit_destroy(void *bit_object);

#endif
