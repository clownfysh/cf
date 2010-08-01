#include "cf/inferno/ca/system.h"
#include "cf/inferno/ca/isystem.h"
#include "cf/x/core/standard.h"

void cf_inferno_ca_isystem_init
(cf_inferno_ca_isystem_t *isystem, void *name_object,
    cf_inferno_ca_system_calculate_new_cell_state_f calculate_new_cell_state,
    cf_inferno_ca_system_create_context_f create_context,
    cf_inferno_ca_system_destroy_context_f destroy_context,
    cf_inferno_ca_system_end_time_step_f end_time_step,
    cf_inferno_ca_system_get_cell_color_f get_cell_color,
    cf_inferno_ca_system_get_relative_cell_index_f get_relative_cell_index,
    cf_inferno_ca_system_start_time_step_f start_time_step)
{
  assert(isystem);
  assert(calculate_new_cell_state);

  isystem->name_object = name_object;
  isystem->calculate_new_cell_state = calculate_new_cell_state;
  isystem->create_context = create_context;
  isystem->destroy_context = destroy_context;
  isystem->end_time_step = end_time_step;

  if (get_cell_color) {
    isystem->get_cell_color = get_cell_color;
  } else {
    isystem->get_cell_color = cf_inferno_ca_system_default_get_cell_color;
  }

  isystem->get_relative_cell_index = get_relative_cell_index;
  isystem->start_time_step = start_time_step;

  assert(isystem->calculate_new_cell_state && isystem->get_cell_color);
}
