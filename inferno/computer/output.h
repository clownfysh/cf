#ifndef inferno_computer_output_h
#define inferno_computer_output_h

#include "x/core/bit.h"

#define INFERNO_COMPUTER_OUTPUT_BITS 32

struct inferno_computer_output_t {
  x_core_bit_t bits[INFERNO_COMPUTER_OUTPUT_BITS];
};
typedef struct inferno_computer_output_t inferno_computer_output_t;

void inferno_computer_output_free(inferno_computer_output_t *output);

void inferno_computer_output_init(inferno_computer_output_t *output);

#endif
