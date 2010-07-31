#include "cf/x/core/standard.h"
#include "cf/inferno/computer/output.h"

void cf_inferno_computer_output_free(cf_inferno_computer_output_t *output) {}

void cf_inferno_computer_output_init(cf_inferno_computer_output_t *output)
{
  memset(output->bits, 0, CF_INFERNO_COMPUTER_OUTPUT_BITS);
}
