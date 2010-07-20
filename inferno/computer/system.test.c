#include "inferno/computer/system.h"
#include "h/core/tools.h"

/*
  static void and_logic(unsigned long input, inferno_computer_output_t *output);
*/

static void or_logic(unsigned long input, inferno_computer_output_t *output);

void and_logic(unsigned long input, inferno_computer_output_t *output)
{
  assert(output);

  if (3 == input) {
    *(output->bits + 0) = 1;
  }
}

void or_logic(unsigned long input, inferno_computer_output_t *output)
{
  assert(output);

  if (input != 0) {
    *(output->bits + 0) = 1;
  }
}

int main(int argc, char *argv[])
{
  inferno_computer_system_t *inferno_computer_system;
  inferno_computer_output_t *inferno_computer_output;
  unsigned short each_input;

  inferno_computer_system = inferno_computer_system_create(4, or_logic);
  if (!inferno_computer_system) {
    h_core_trace_exit("inferno_computer_system_create");
  }

  inferno_computer_system_print(inferno_computer_system);

  for (each_input = 0; each_input < 4; each_input++) {
    inferno_computer_output = inferno_computer_system_compute(inferno_computer_system, each_input);
    printf("%i -> %i\n", each_input, *(inferno_computer_output->bits + 0));
  }

  inferno_computer_system_destroy(inferno_computer_system);

  return 0;
}
