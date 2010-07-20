#ifndef inferno_ca_k3_h
#define inferno_ca_k3_h

#include "inferno/ca/system.h"

inferno_ca_t inferno_ca_k3_calculate_new_cell_state(inferno_ca_system_t *system,
    unsigned long cell_index);

void *inferno_ca_k3_create_context(void *parameter_object);

void inferno_ca_k3_destroy_context(void *context_object);

void inferno_ca_k3_get_cell_color(inferno_ca_t *cell, h_core_color_t *color);

unsigned long inferno_ca_k3_get_relative_cell_index(inferno_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void inferno_ca_k3_init_systemey(inferno_ca_systemey_t *systemey, void *name_object);

#endif
