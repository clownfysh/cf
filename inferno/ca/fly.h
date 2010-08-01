#ifndef cf_inferno_ca_fly_h
#define cf_inferno_ca_fly_h

#include "cf/inferno/ca/system.h"

cf_inferno_ca_t cf_inferno_ca_fly_calculate_new_cell_state(cf_inferno_ca_system_t *system,
    unsigned long cell_index);

unsigned long cf_inferno_ca_fly_get_relative_cell_index(cf_inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_inferno_ca_fly_init_isystem(cf_inferno_ca_isystem_t *isystem, void *name_object);

#endif
