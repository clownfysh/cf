#ifndef cf_inferno_ca_k3_h
#define cf_inferno_ca_k3_h

#include "cf/inferno/ca/system.h"

cf_inferno_ca_t cf_inferno_ca_k3_calculate_new_cell_state(cf_inferno_ca_system_t *system,
    unsigned long cell_index);

void *cf_inferno_ca_k3_create_context(void *parameter_object);

void cf_inferno_ca_k3_destroy_context(void *context_object);

void cf_inferno_ca_k3_get_cell_color(cf_inferno_ca_t *cell, cf_x_core_color_t *color);

unsigned long cf_inferno_ca_k3_get_relative_cell_index(cf_inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void cf_inferno_ca_k3_init_isystem(cf_inferno_ca_isystem_t *isystem, void *name_object);

#endif
