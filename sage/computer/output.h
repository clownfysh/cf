#ifndef cf_sage_computer_output_h
#define cf_sage_computer_output_h

#include "cf/x/core/bit.h"

#define CF_SAGE_COMPUTER_OUTPUT_BITS 32

struct cf_sage_computer_output_t {
  cf_x_core_bit_t bits[CF_SAGE_COMPUTER_OUTPUT_BITS];
};
typedef struct cf_sage_computer_output_t cf_sage_computer_output_t;

void cf_sage_computer_output_free(cf_sage_computer_output_t *output);

void cf_sage_computer_output_init(cf_sage_computer_output_t *output);

#endif
