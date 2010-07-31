#ifndef cf_sage_ca_malice_h
#define cf_sage_ca_malice_h

#include "cf/sage/ca/system.h"

cf_sage_ca_t cf_sage_ca_malice_calculate_new_cell_state(cf_sage_ca_system_t *system,
    unsigned long cell_index);

unsigned long cf_sage_ca_malice_get_relative_cell_index(cf_sage_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_sage_ca_malice_init_systemey(cf_sage_ca_systemey_t *systemey,
    void *name_object);

#endif
