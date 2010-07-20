#ifndef inferno_ca_state_h
#define inferno_ca_state_h

#include "inferno/ca/cell.h"

struct inferno_ca_state_t;
typedef struct inferno_ca_state_t inferno_ca_state_t;

int inferno_ca_state_compare(void *state_object_a, void *state_object_b);

void *inferno_ca_state_copy(void *state_object);

inferno_ca_state_t *inferno_ca_state_create(unsigned long cell_count);

inferno_ca_state_t *inferno_ca_state_create_from_cells(inferno_ca_t *cells,
    unsigned long cell_count);

void inferno_ca_state_destroy(void *state_object);

char *inferno_ca_state_get_as_string(void *state_object);

inferno_ca_t *inferno_ca_state_get_cell(inferno_ca_state_t *state,
    unsigned long cell_index);

unsigned long inferno_ca_state_get_cell_count(inferno_ca_state_t *state);

unsigned long inferno_ca_state_get_cell_rule(inferno_ca_state_t *state,
    unsigned long cell_index);

unsigned long inferno_ca_state_get_cell_value(inferno_ca_state_t *state,
    unsigned long cell_index);

void inferno_ca_state_print(inferno_ca_state_t *state);

void inferno_ca_state_set_cell(inferno_ca_state_t *state, unsigned long cell_index,
    inferno_ca_t *cell);

void inferno_ca_state_set_cell_rule(inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long inferno_ca_rule);

void inferno_ca_state_set_cell_value(inferno_ca_state_t *state,
    unsigned long cell_index, unsigned long inferno_ca_value);

#endif
