#include "inferno/computer/system.h"
#include "h/core/tools.h"

struct inferno_computer_system_t {
  unsigned long order;
  inferno_computer_logic_f logic;
  inferno_computer_output_t *computations;
};

static void compute_with_logic(inferno_computer_system_t *system);

static void free_computations(inferno_computer_system_t *system);

static void init_computations(inferno_computer_system_t *system);

void compute_with_logic(inferno_computer_system_t *system)
{
  assert(system);
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    system->logic(each_computation, system->computations + each_computation);
  }
}

inferno_computer_output_t *inferno_computer_system_compute(inferno_computer_system_t *system,
    unsigned long input)
{
  return system->computations + input;
}

inferno_computer_system_t *inferno_computer_system_create(unsigned long order,
    inferno_computer_logic_f logic)
{
  inferno_computer_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->order = order;
    system->logic = logic;
    system->computations = malloc(order * sizeof(inferno_computer_output_t));
    if (system->computations) {
      init_computations(system);
      compute_with_logic(system);
    } else {
      h_core_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return system;
}

inferno_computer_system_t *inferno_computer_system_create_from_file(char *filename)
{
  h_core_trace_exit("TODO: implement");
  return NULL;
}

void inferno_computer_system_destroy(void *system_object)
{
  assert(system_object);
  inferno_computer_system_t *system;

  system = system_object;

  free_computations(system);
  free(system->computations);
  free(system);
}

char *inferno_computer_system_get_as_string(void *system_object)
{
  assert(system_object);
  inferno_computer_system_t *system;
  unsigned long string_size;
  char *string;
  unsigned long each_computation;
  unsigned long each_bit;
  unsigned long string_position;
  inferno_computer_output_t *output;
  h_core_bit_t bit;

  system = system_object;

  string_size = (INFERNO_COMPUTER_OUTPUT_BITS + 1) * system->order;
  string = malloc(string_size + 1);
  if (string) {
    string_position = 0;
    for (each_computation = 0; each_computation < system->order;
         each_computation++) {
      for (each_bit = 0; each_bit < INFERNO_COMPUTER_OUTPUT_BITS; each_bit++) {
        output = system->computations + each_computation;
        bit = *(output->bits + each_bit);
        if (bit) {
          *(string + string_position) = '1';
        } else {
          *(string + string_position) = '0';
        }
        string_position++;
      }
      *(string + string_position) = '\n';
      string_position++;
    }
    *(string + string_position) = '\0';
  } else {
    h_core_trace("malloc");
  }

  return string;
}

void inferno_computer_system_print(inferno_computer_system_t *system)
{
  assert(system);
  char *string;

  string = inferno_computer_system_get_as_string(system);
  if (string) {
    printf("%s", string);
    free(string);
  } else {
    h_core_trace("inferno_computer_system_get_as_string");
  }
}

h_core_bool_t inferno_computer_system_save_as_file(inferno_computer_system_t *system,
    char *filename)
{
  h_core_trace_exit("TODO: implement");
  return h_core_bool_false;
}

void free_computations(inferno_computer_system_t *system)
{
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    inferno_computer_output_free(system->computations + each_computation);
  }
}

void init_computations(inferno_computer_system_t *system)
{
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    inferno_computer_output_init(system->computations + each_computation);
  }
}
