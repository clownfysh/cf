#ifndef cf_sage_ca_state_h
#define cf_sage_ca_state_h

#include "cf/sage/ca/cell.h"

struct cf_sage_ca_state_t;
typedef struct cf_sage_ca_state_t cf_sage_ca_state_t;

int cf_sage_ca_state_compare(void *state_object_a, void *state_object_b);

void *cf_sage_ca_state_copy(void *state_object);

cf_sage_ca_state_t *cf_sage_ca_state_create(unsigned long cell_count);

cf_sage_ca_state_t *cf_sage_ca_state_create_from_cells(cf_sage_ca_t *cells,
    unsigned long cell_count);

void cf_sage_ca_state_destroy(void *state_object);

char *cf_sage_ca_state_get_as_string(void *state_object);

cf_sage_ca_t *cf_sage_ca_state_get_cell(cf_sage_ca_state_t *state,
    unsigned long cell_index);

unsigned long cf_sage_ca_state_get_cell_count(cf_sage_ca_state_t *state);

unsigned long cf_sage_ca_state_get_cell_rule(cf_sage_ca_state_t *state,
    unsigned long cell_index);

unsigned long cf_sage_ca_state_get_cell_value(cf_sage_ca_state_t *state,
    unsigned long cell_index);

void cf_sage_ca_state_print(cf_sage_ca_state_t *state);

void cf_sage_ca_state_set_cell(cf_sage_ca_state_t *state, unsigned long cell_index,
    cf_sage_ca_t *cell);

void cf_sage_ca_state_set_cell_rule(cf_sage_ca_state_t *state,
    unsigned long cell_index, unsigned long cf_sage_ca_rule);

void cf_sage_ca_state_set_cell_value(cf_sage_ca_state_t *state,
    unsigned long cell_index, unsigned long cf_sage_ca_value);

#endif
