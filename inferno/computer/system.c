#include "cf/inferno/computer/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_computer_system_t {
  unsigned long order;
  cf_inferno_computer_logic_f logic;
  cf_inferno_computer_output_t *computations;
};

static void compute_with_logic(cf_inferno_computer_system_t *system);

static void free_computations(cf_inferno_computer_system_t *system);

static void init_computations(cf_inferno_computer_system_t *system);

void compute_with_logic(cf_inferno_computer_system_t *system)
{
  assert(system);
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    system->logic(each_computation, system->computations + each_computation);
  }
}

cf_inferno_computer_output_t *cf_inferno_computer_system_compute(cf_inferno_computer_system_t *system,
    unsigned long input)
{
  return system->computations + input;
}

cf_inferno_computer_system_t *cf_inferno_computer_system_create(unsigned long order,
    cf_inferno_computer_logic_f logic)
{
  cf_inferno_computer_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->order = order;
    system->logic = logic;
    system->computations = malloc(order * sizeof(cf_inferno_computer_output_t));
    if (system->computations) {
      init_computations(system);
      compute_with_logic(system);
    } else {
      cf_x_core_trace("malloc");
      free(system);
      system = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return system;
}

cf_inferno_computer_system_t *cf_inferno_computer_system_create_from_file(char *filename)
{
  cf_x_core_trace_exit("TODO: implement");
  return NULL;
}

void cf_inferno_computer_system_destroy(void *system_object)
{
  assert(system_object);
  cf_inferno_computer_system_t *system;

  system = system_object;

  free_computations(system);
  free(system->computations);
  free(system);
}

char *cf_inferno_computer_system_get_as_string(void *system_object)
{
  assert(system_object);
  cf_inferno_computer_system_t *system;
  unsigned long string_size;
  char *string;
  unsigned long each_computation;
  unsigned long each_bit;
  unsigned long string_position;
  cf_inferno_computer_output_t *output;
  cf_x_core_bit_t bit;

  system = system_object;

  string_size = (CF_INFERNO_COMPUTER_OUTPUT_BITS + 1) * system->order;
  string = malloc(string_size + 1);
  if (string) {
    string_position = 0;
    for (each_computation = 0; each_computation < system->order;
         each_computation++) {
      for (each_bit = 0; each_bit < CF_INFERNO_COMPUTER_OUTPUT_BITS; each_bit++) {
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
    cf_x_core_trace("malloc");
  }

  return string;
}

void cf_inferno_computer_system_print(cf_inferno_computer_system_t *system)
{
  assert(system);
  char *string;

  string = cf_inferno_computer_system_get_as_string(system);
  if (string) {
    printf("%s", string);
    free(string);
  } else {
    cf_x_core_trace("inferno_computer_system_get_as_string");
  }
}

cf_x_core_bool_t cf_inferno_computer_system_save_as_file(cf_inferno_computer_system_t *system,
    char *filename)
{
  cf_x_core_trace_exit("TODO: implement");
  return cf_x_core_bool_false;
}

void free_computations(cf_inferno_computer_system_t *system)
{
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    cf_inferno_computer_output_free(system->computations + each_computation);
  }
}

void init_computations(cf_inferno_computer_system_t *system)
{
  unsigned long each_computation;

  for (each_computation = 0; each_computation < system->order;
       each_computation++) {
    cf_inferno_computer_output_init(system->computations + each_computation);
  }
}
