#ifndef inferno_ca_systemey_h
#define inferno_ca_systemey_h

#include "inferno/ca/cell.h"
#include "inferno/ca/declarations.h"
#include "x/core/color.h"

#define INFERNO_CA_NO_CREATE_CONTEXT_FUNCTION NULL
#define INFERNO_CA_NO_DESTROY_CONTEXT_FUNCTION NULL
#define INFERNO_CA_NO_GET_CELL_COLOR_FUNCTION NULL
#define INFERNO_CA_NO_END_TIME_STEP_FUNCTION NULL
#define INFERNO_CA_NO_START_TIME_STEP_FUNCTION NULL

typedef inferno_ca_t (*inferno_ca_system_calculate_new_cell_state_f)
(inferno_ca_system_t *system, unsigned long cell_index);

typedef void *(*inferno_ca_system_create_context_f)(void *name_object);

typedef void (*inferno_ca_system_destroy_context_f)(void *context_object);

typedef void (*inferno_ca_system_end_time_step_f)(inferno_ca_system_t *system);

typedef void (*inferno_ca_system_get_cell_color_f)(inferno_ca_t *cell,
    x_core_color_t *color);

typedef unsigned long (*inferno_ca_system_get_relative_cell_index_f)
(inferno_ca_system_t *system, unsigned long cell_index,
    unsigned long relationship);

typedef void (*inferno_ca_system_start_time_step_f)(inferno_ca_system_t *system);

struct inferno_ca_systemey_t {
  void *name_object;

  inferno_ca_system_calculate_new_cell_state_f calculate_new_cell_state;
  inferno_ca_system_create_context_f create_context;
  inferno_ca_system_destroy_context_f destroy_context;
  inferno_ca_system_end_time_step_f end_time_step;
  inferno_ca_system_get_cell_color_f get_cell_color;
  inferno_ca_system_get_relative_cell_index_f get_relative_cell_index;
  inferno_ca_system_start_time_step_f start_time_step;
};
typedef struct inferno_ca_systemey_t inferno_ca_systemey_t;

void inferno_ca_systemey_init(inferno_ca_systemey_t *systemey,
    void *name_object,
    inferno_ca_system_calculate_new_cell_state_f calculate_new_cell_state,
    inferno_ca_system_create_context_f create_context,
    inferno_ca_system_destroy_context_f destroy_context,
    inferno_ca_system_end_time_step_f end_time_step,
    inferno_ca_system_get_cell_color_f get_cell_color,
    inferno_ca_system_get_relative_cell_index_f get_relative_cell_index,
    inferno_ca_system_start_time_step_f start_time_step);

#endif
