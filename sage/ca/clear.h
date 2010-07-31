#ifndef cf_sage_ca_clear_h
#define cf_sage_ca_clear_h

#include "cf/sage/ca/system.h"

cf_sage_ca_t cf_sage_ca_clear_calculate_new_cell_state(cf_sage_ca_system_t *system,
    unsigned long cell_index);

void *cf_sage_ca_clear_create_context(void *name_object);

unsigned long cf_sage_ca_clear_get_relative_cell_index(cf_sage_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_sage_ca_clear_destroy_context(void *context_object);

void cf_sage_ca_clear_end_time_step(cf_sage_ca_system_t *system);

void cf_sage_ca_clear_init_systemey(cf_sage_ca_systemey_t *systemey,
    void *name_object);

void cf_sage_ca_clear_start_time_step(cf_sage_ca_system_t *system);

#endif
