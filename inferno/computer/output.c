#include "x/core/standard.h"
#include "inferno/computer/output.h"

void inferno_computer_output_free(inferno_computer_output_t *output) {}

void inferno_computer_output_init(inferno_computer_output_t *output)
{
  memset(output->bits, 0, INFERNO_COMPUTER_OUTPUT_BITS);
}
