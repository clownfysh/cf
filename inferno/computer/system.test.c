#include "cf/inferno/computer/system.h"
#include "cf/x/core/tools.h"

/*
  static void and_logic(unsigned long input, cf_inferno_computer_output_t *output);
*/

static void or_logic(unsigned long input, cf_inferno_computer_output_t *output);

void and_logic(unsigned long input, cf_inferno_computer_output_t *output)
{
  assert(output);

  if (3 == input) {
    *(output->bits + 0) = 1;
  }
}

void or_logic(unsigned long input, cf_inferno_computer_output_t *output)
{
  assert(output);

  if (input != 0) {
    *(output->bits + 0) = 1;
  }
}

int main(int argc, char *argv[])
{
  cf_inferno_computer_system_t *cf_inferno_computer_system;
  cf_inferno_computer_output_t *cf_inferno_computer_output;
  unsigned short each_input;

  cf_inferno_computer_system = cf_inferno_computer_system_create(4, or_logic);
  if (!cf_inferno_computer_system) {
    cf_x_core_trace_exit("inferno_computer_system_create");
  }

  cf_inferno_computer_system_print(cf_inferno_computer_system);

  for (each_input = 0; each_input < 4; each_input++) {
    cf_inferno_computer_output
      = cf_inferno_computer_system_compute(cf_inferno_computer_system, each_input);
    printf("%i -> %i\n", each_input, *(cf_inferno_computer_output->bits + 0));
  }

  cf_inferno_computer_system_destroy(cf_inferno_computer_system);

  return 0;
}
