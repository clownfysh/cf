#ifndef cf_inferno_computer_output_h
#define cf_inferno_computer_output_h

#include "cf/x/core/bit.h"

#define CF_INFERNO_COMPUTER_OUTPUT_BITS 32

struct cf_inferno_computer_output_t {
  cf_x_core_bit_t bits[CF_INFERNO_COMPUTER_OUTPUT_BITS];
};
typedef struct cf_inferno_computer_output_t cf_inferno_computer_output_t;

void cf_inferno_computer_output_free(cf_inferno_computer_output_t *output);

void cf_inferno_computer_output_init(cf_inferno_computer_output_t *output);

#endif
