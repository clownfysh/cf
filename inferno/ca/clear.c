#include "inferno/ca/clear.h"
#include "h/core/tools.h"

struct clear_context_t {
  unsigned long total;
};
typedef struct clear_context_t clear_context_t;

inferno_ca_t inferno_ca_clear_calculate_new_cell_state(inferno_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  void *name_object;
  unsigned short *name;
  unsigned long neighbor_0_index;
  unsigned long neighbor_1_index;
  unsigned long neighbor_2_index;
  inferno_ca_t *neighbor_0_cell;
  inferno_ca_t *neighbor_1_cell;
  inferno_ca_t *neighbor_2_cell;
  unsigned long neighbor_0_value;
  unsigned long neighbor_1_value;
  unsigned long neighbor_2_value;
  unsigned short neighborhood;
  unsigned long new_cell_value;
  inferno_ca_t new_cell_state;
  clear_context_t *clear_context;

  name_object = inferno_ca_system_get_name(system);
  name = name_object;
  clear_context = inferno_ca_system_get_context(system);

  neighbor_0_index = inferno_ca_clear_get_relative_cell_index
    (system, cell_index, 0);
  neighbor_1_index = inferno_ca_clear_get_relative_cell_index
    (system, cell_index, 1);
  neighbor_2_index = inferno_ca_clear_get_relative_cell_index
    (system, cell_index, 2);

  neighbor_0_cell = inferno_ca_system_get_current_cell(system, neighbor_0_index);
  neighbor_1_cell = inferno_ca_system_get_current_cell(system, neighbor_1_index);
  neighbor_2_cell = inferno_ca_system_get_current_cell(system, neighbor_2_index);

  neighbor_0_value = neighbor_0_cell->value;
  neighbor_1_value = neighbor_1_cell->value;
  neighbor_2_value = neighbor_2_cell->value;

  neighborhood = (neighbor_0_value * 4) + (neighbor_1_value * 2)
    + neighbor_2_value;

  clear_context->total += neighbor_1_value;

  new_cell_value
    = h_core_get_bit(clear_context->total, (neighborhood * *name) % 256)
    ^ h_core_get_bit(*name, neighborhood);

  /*
  clear_context->total += new_cell_value;
  */

  new_cell_state.value = new_cell_value;
  inferno_ca_init(&new_cell_state, new_cell_value, INFERNO_CA_NO_RULE);

  return new_cell_state;
}

void *inferno_ca_clear_create_context(void *name_object)
{
  assert(name_object);
  clear_context_t *clear_context;

  clear_context = malloc(sizeof *clear_context);
  if (clear_context) {
    clear_context->total = 0;
  } else {
    h_core_trace("malloc");
  }

  return clear_context;
}

unsigned long inferno_ca_clear_get_relative_cell_index(inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship)
{
  assert(system);
  unsigned long cell_count;
  unsigned long relative_cell_index;

  cell_count = inferno_ca_system_get_cell_count(system);
  relative_cell_index
    = h_core_wrap_index(cell_index + (relationship - 1), cell_count);

  return relative_cell_index;
}

void inferno_ca_clear_destroy_context(void *context_object)
{
  assert(context_object);
  clear_context_t *clear_context;

  clear_context = context_object;

  free(clear_context);
}

void inferno_ca_clear_end_time_step(inferno_ca_system_t *system)
{
  assert(system);
  clear_context_t *clear_context;

  clear_context = inferno_ca_system_get_context(system);
}

void inferno_ca_clear_init_systemey(inferno_ca_systemey_t *systemey, void *name_object)
{
  inferno_ca_systemey_init(systemey, name_object,
      inferno_ca_clear_calculate_new_cell_state, inferno_ca_clear_create_context,
      inferno_ca_clear_destroy_context, inferno_ca_clear_end_time_step,
      inferno_ca_system_get_cell_color_binary,
      inferno_ca_clear_get_relative_cell_index, inferno_ca_clear_start_time_step);
}

void inferno_ca_clear_start_time_step(inferno_ca_system_t *system)
{
  assert(system);
  clear_context_t *clear_context;

  clear_context = inferno_ca_system_get_context(system);

  /*
  clear_context->total = 0;
  */
}
