#include "cf/x/core/standard.h"
#include "cf/sage/computer/output.h"

void cf_sage_computer_output_free(cf_sage_computer_output_t *output) {}

void cf_sage_computer_output_init(cf_sage_computer_output_t *output)
{
  memset(output->bits, 0, CF_SAGE_COMPUTER_OUTPUT_BITS);
}
