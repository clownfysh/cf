#ifndef inferno_ca_system_h
#define inferno_ca_system_h

#include "inferno/ca/state.h"
#include "inferno/ca/systemey.h"
#include "h/container/array.h"
#include "h/core/run.h"

inferno_ca_system_t *inferno_ca_system_create
(h_container_array_t *initial_state_history,
    unsigned long initial_time_step_count, inferno_ca_systemey_t *systemey);

void inferno_ca_system_default_get_cell_color(inferno_ca_t *cell,
    h_core_color_t *color);

void inferno_ca_system_destroy(inferno_ca_system_t *system);

unsigned long inferno_ca_system_get_age_in_time_steps(inferno_ca_system_t *system);

unsigned long inferno_ca_system_get_cell_count(inferno_ca_system_t *system);

inferno_ca_t *inferno_ca_system_get_cell(inferno_ca_system_t *system,
    unsigned long time_step, unsigned long cell_index);

void inferno_ca_system_get_cell_color_binary(inferno_ca_t *cell,
    h_core_color_t *color);

void *inferno_ca_system_get_context(inferno_ca_system_t *system);

inferno_ca_t *inferno_ca_system_get_current_cell(inferno_ca_system_t *system,
    unsigned long cell_index);

inferno_ca_state_t *inferno_ca_system_get_current_state(inferno_ca_system_t *system);

void *inferno_ca_system_get_name(inferno_ca_system_t *system);

inferno_ca_t *inferno_ca_system_get_relative_cell(inferno_ca_system_t *system,
    long relative_time_step, unsigned long cell_index);

inferno_ca_state_t *inferno_ca_system_get_state(inferno_ca_system_t *system,
    unsigned long time_step);

void inferno_ca_system_run(inferno_ca_system_t *system, h_core_run_t *run);

h_core_bool_t inferno_ca_system_save_snapshot_jpeg(inferno_ca_system_t *system,
    char *filename);

h_core_bool_t inferno_ca_system_save_snapshot_text(inferno_ca_system_t *system,
    char *filename);

void inferno_ca_system_set_cell(inferno_ca_system_t *system, unsigned long time_step,
    unsigned long cell_index, inferno_ca_t *cell);

void inferno_ca_system_set_current_cell(inferno_ca_system_t *system,
    unsigned long cell_index, inferno_ca_t *cell);

void inferno_ca_system_set_random_seed(inferno_ca_system_t *system,
    unsigned long seed);

void inferno_ca_system_set_relative_cell(inferno_ca_system_t *system,
    long relative_time_step, unsigned long cell_index, inferno_ca_t *cell);

#endif
