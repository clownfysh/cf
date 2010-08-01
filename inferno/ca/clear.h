#ifndef cf_inferno_ca_clear_h
#define cf_inferno_ca_clear_h

#include "cf/inferno/ca/system.h"

cf_inferno_ca_t cf_inferno_ca_clear_calculate_new_cell_state(cf_inferno_ca_system_t *system,
    unsigned long cell_index);

void *cf_inferno_ca_clear_create_context(void *name_object);

unsigned long cf_inferno_ca_clear_get_relative_cell_index(cf_inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_inferno_ca_clear_destroy_context(void *context_object);

void cf_inferno_ca_clear_end_time_step(cf_inferno_ca_system_t *system);

void cf_inferno_ca_clear_init_isystem(cf_inferno_ca_isystem_t *isystem,
    void *name_object);

void cf_inferno_ca_clear_start_time_step(cf_inferno_ca_system_t *system);

#endif
