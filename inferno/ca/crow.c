#include "cf/inferno/ca/crow.h"
#include "cf/inferno/ca/isystem.h"
#include "cf/x/core/tools.h"

cf_inferno_ca_t cf_inferno_ca_crow_calculate_new_cell_state(cf_inferno_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  void *name_object;
  unsigned short *name;
  unsigned long neighbor_0_index;
  unsigned long neighbor_1_index;
  unsigned long neighbor_2_index;
  unsigned long neighbor_3_index;
  cf_inferno_ca_t *neighbor_0_cell;
  cf_inferno_ca_t *neighbor_1_cell;
  cf_inferno_ca_t *neighbor_2_cell;
  cf_inferno_ca_t *neighbor_3_cell;
  unsigned long neighbor_0_value;
  unsigned long neighbor_1_value;
  unsigned long neighbor_2_value;
  unsigned long neighbor_3_value;
  unsigned short neighborhood;
  unsigned long new_cell_value;
  cf_inferno_ca_t new_cell_state;

  name_object = cf_inferno_ca_system_get_name(system);
  name = name_object;

  neighbor_0_index = cf_inferno_ca_crow_get_relative_cell_index
    (system, cell_index, 0);
  neighbor_1_index = cf_inferno_ca_crow_get_relative_cell_index
    (system, cell_index, 1);
  neighbor_2_index = cf_inferno_ca_crow_get_relative_cell_index
    (system, cell_index, 2);
  neighbor_3_index = cf_inferno_ca_crow_get_relative_cell_index
    (system, cell_index, 3);

  neighbor_0_cell = cf_inferno_ca_system_get_relative_cell
    (system, -1, neighbor_0_index);
  neighbor_1_cell = cf_inferno_ca_system_get_current_cell(system, neighbor_1_index);
  neighbor_2_cell = cf_inferno_ca_system_get_current_cell(system, neighbor_2_index);
  neighbor_3_cell = cf_inferno_ca_system_get_current_cell(system, neighbor_3_index);

  neighbor_0_value = neighbor_0_cell->value;
  neighbor_1_value = neighbor_1_cell->value;
  neighbor_2_value = neighbor_2_cell->value;
  neighbor_3_value = neighbor_3_cell->value;

  neighborhood = (neighbor_0_value * 8) + (neighbor_1_value * 4)
    + (neighbor_2_value * 2) + neighbor_3_value;

  new_cell_value = cf_x_core_get_bit(*name, neighborhood);

  new_cell_state.value = new_cell_value;
  cf_inferno_ca_init(&new_cell_state, new_cell_value, CF_INFERNO_CA_NO_RULE);

  return new_cell_state;
}

unsigned long cf_inferno_ca_crow_get_relative_cell_index(cf_inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship)
{
  assert(system);
  unsigned long cell_count;
  unsigned long relative_cell_index;

  cell_count = cf_inferno_ca_system_get_cell_count(system);

  if (0 == cell_index) {
    switch (relationship) {
      case 0:
        relative_cell_index = cell_index;
        break;
      case 1:
        relative_cell_index = cell_count - 1;
        break;
      case 2:
        relative_cell_index = cell_index;
        break;
      case 3:
        relative_cell_index = cell_index + 1;
        break;
      default:
        cf_x_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  } else if ((cell_count - 1) == cell_index) {
    switch (relationship) {
      case 0:
        relative_cell_index = cell_index;
        break;
      case 1:
        relative_cell_index = cell_index - 1;
        break;
      case 2:
        relative_cell_index = cell_index;
        break;
      case 3:
        relative_cell_index = 0;
        break;
      default:
        cf_x_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  } else {
    switch (relationship) {
      case 0:
        relative_cell_index = cell_index;
        break;
      case 1:
        relative_cell_index = cell_index - 1;
        break;
      case 2:
        relative_cell_index = cell_index;
        break;
      case 3:
        relative_cell_index = cell_index + 1;
        break;
      default:
        cf_x_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  }

  return relative_cell_index;
}

void cf_inferno_ca_crow_init_isystem(cf_inferno_ca_isystem_t *isystem, void *name_object)
{
  cf_inferno_ca_isystem_init(isystem, name_object,
      cf_inferno_ca_crow_calculate_new_cell_state, CF_INFERNO_CA_NO_CREATE_CONTEXT_FUNCTION,
      CF_INFERNO_CA_NO_DESTROY_CONTEXT_FUNCTION, CF_INFERNO_CA_NO_END_TIME_STEP_FUNCTION,
      cf_inferno_ca_system_get_cell_color_binary, cf_inferno_ca_crow_get_relative_cell_index,
      CF_INFERNO_CA_NO_START_TIME_STEP_FUNCTION);
}
