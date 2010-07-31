#ifndef cf_inferno_ca_eca_h
#define cf_inferno_ca_eca_h

#include "cf/inferno/ca/system.h"

cf_inferno_ca_t cf_inferno_ca_eca_calculate_new_cell_state(cf_inferno_ca_system_t *system,
    unsigned long cell_index);

unsigned long cf_inferno_ca_eca_get_relative_cell_index(cf_inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_inferno_ca_eca_init_systemey(cf_inferno_ca_systemey_t *systemey, void *name_object);

#endif
