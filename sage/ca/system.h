#ifndef cf_sage_ca_system_h
#define cf_sage_ca_system_h

#include "cf/sage/ca/state.h"
#include "cf/sage/ca/systemey.h"
#include "cf/x/case/array.h"
#include "cf/x/core/run.h"

cf_sage_ca_system_t *cf_sage_ca_system_create
(cf_x_case_array_t *initial_state_history,
    unsigned long initial_time_step_count, cf_sage_ca_systemey_t *systemey);

void cf_sage_ca_system_default_get_cell_color(cf_sage_ca_t *cell,
    cf_x_core_color_t *color);

void cf_sage_ca_system_destroy(cf_sage_ca_system_t *system);

unsigned long cf_sage_ca_system_get_age_in_time_steps(cf_sage_ca_system_t *system);

unsigned long cf_sage_ca_system_get_cell_count(cf_sage_ca_system_t *system);

cf_sage_ca_t *cf_sage_ca_system_get_cell(cf_sage_ca_system_t *system,
    unsigned long time_step, unsigned long cell_index);

void cf_sage_ca_system_get_cell_color_binary(cf_sage_ca_t *cell,
    cf_x_core_color_t *color);

void *cf_sage_ca_system_get_context(cf_sage_ca_system_t *system);

cf_sage_ca_t *cf_sage_ca_system_get_current_cell(cf_sage_ca_system_t *system,
    unsigned long cell_index);

cf_sage_ca_state_t *cf_sage_ca_system_get_current_state(cf_sage_ca_system_t *system);

void *cf_sage_ca_system_get_name(cf_sage_ca_system_t *system);

cf_sage_ca_t *cf_sage_ca_system_get_relative_cell(cf_sage_ca_system_t *system,
    long relative_time_step, unsigned long cell_index);

cf_sage_ca_state_t *cf_sage_ca_system_get_state(cf_sage_ca_system_t *system,
    unsigned long time_step);

void cf_sage_ca_system_run(cf_sage_ca_system_t *system, cf_x_core_run_t *run);

cf_x_core_bool_t cf_sage_ca_system_save_snapshot_jpeg(cf_sage_ca_system_t *system,
    char *filename);

cf_x_core_bool_t cf_sage_ca_system_save_snapshot_text(cf_sage_ca_system_t *system,
    char *filename);

void cf_sage_ca_system_set_cell(cf_sage_ca_system_t *system, unsigned long time_step,
    unsigned long cell_index, cf_sage_ca_t *cell);

void cf_sage_ca_system_set_current_cell(cf_sage_ca_system_t *system,
    unsigned long cell_index, cf_sage_ca_t *cell);

void cf_sage_ca_system_set_random_seed(cf_sage_ca_system_t *system,
    unsigned long seed);

void cf_sage_ca_system_set_relative_cell(cf_sage_ca_system_t *system,
    long relative_time_step, unsigned long cell_index, cf_sage_ca_t *cell);

#endif
