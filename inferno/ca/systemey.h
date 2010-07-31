#ifndef cf_inferno_ca_systemey_h
#define cf_inferno_ca_systemey_h

#include "cf/inferno/ca/cell.h"
#include "cf/inferno/ca/declarations.h"
#include "cf/x/core/color.h"

#define CF_INFERNO_CA_NO_CREATE_CONTEXT_FUNCTION NULL
#define CF_INFERNO_CA_NO_DESTROY_CONTEXT_FUNCTION NULL
#define CF_INFERNO_CA_NO_GET_CELL_COLOR_FUNCTION NULL
#define CF_INFERNO_CA_NO_END_TIME_STEP_FUNCTION NULL
#define CF_INFERNO_CA_NO_START_TIME_STEP_FUNCTION NULL

typedef cf_inferno_ca_t (*cf_inferno_ca_system_calculate_new_cell_state_f)
(cf_inferno_ca_system_t *system, unsigned long cell_index);

typedef void *(*cf_inferno_ca_system_create_context_f)(void *name_object);

typedef void (*cf_inferno_ca_system_destroy_context_f)(void *context_object);

typedef void (*cf_inferno_ca_system_end_time_step_f)(cf_inferno_ca_system_t *system);

typedef void (*cf_inferno_ca_system_get_cell_color_f)(cf_inferno_ca_t *cell,
    cf_x_core_color_t *color);

typedef unsigned long (*cf_inferno_ca_system_get_relative_cell_index_f)
(cf_inferno_ca_system_t *system, unsigned long cell_index,
    unsigned long relationship);

typedef void (*cf_inferno_ca_system_start_time_step_f)(cf_inferno_ca_system_t *system);

struct cf_inferno_ca_systemey_t {
  void *name_object;

  cf_inferno_ca_system_calculate_new_cell_state_f calculate_new_cell_state;
  cf_inferno_ca_system_create_context_f create_context;
  cf_inferno_ca_system_destroy_context_f destroy_context;
  cf_inferno_ca_system_end_time_step_f end_time_step;
  cf_inferno_ca_system_get_cell_color_f get_cell_color;
  cf_inferno_ca_system_get_relative_cell_index_f get_relative_cell_index;
  cf_inferno_ca_system_start_time_step_f start_time_step;
};
typedef struct cf_inferno_ca_systemey_t cf_inferno_ca_systemey_t;

void cf_inferno_ca_systemey_init(cf_inferno_ca_systemey_t *systemey,
    void *name_object,
    cf_inferno_ca_system_calculate_new_cell_state_f calculate_new_cell_state,
    cf_inferno_ca_system_create_context_f create_context,
    cf_inferno_ca_system_destroy_context_f destroy_context,
    cf_inferno_ca_system_end_time_step_f end_time_step,
    cf_inferno_ca_system_get_cell_color_f get_cell_color,
    cf_inferno_ca_system_get_relative_cell_index_f get_relative_cell_index,
    cf_inferno_ca_system_start_time_step_f start_time_step);

#endif
