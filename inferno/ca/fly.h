#ifndef inferno_ca_fly_h
#define inferno_ca_fly_h

#include "inferno/ca/system.h"

inferno_ca_t inferno_ca_fly_calculate_new_cell_state(inferno_ca_system_t *system,
    unsigned long cell_index);

unsigned long inferno_ca_fly_get_relative_cell_index(inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void inferno_ca_fly_init_systemey(inferno_ca_systemey_t *systemey, void *name_object);

#endif
