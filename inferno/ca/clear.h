#ifndef inferno_ca_clear_h
#define inferno_ca_clear_h

#include "inferno/ca/system.h"

inferno_ca_t inferno_ca_clear_calculate_new_cell_state(inferno_ca_system_t *system,
    unsigned long cell_index);

void *inferno_ca_clear_create_context(void *name_object);

unsigned long inferno_ca_clear_get_relative_cell_index(inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void inferno_ca_clear_destroy_context(void *context_object);

void inferno_ca_clear_end_time_step(inferno_ca_system_t *system);

void inferno_ca_clear_init_systemey(inferno_ca_systemey_t *systemey,
    void *name_object);

void inferno_ca_clear_start_time_step(inferno_ca_system_t *system);

#endif
