#ifndef cf_chage_atom_h
#define cf_chage_atom_h

#include "cf/chage/sing/system.h"
#include "cf/chage/tools.h"
#include "cf/x/core/tools.h"

struct cf_chage_atom_t;
typedef struct cf_chage_atom_t cf_chage_atom_t;

cf_chage_atom_t *cf_chage_atom_create(cf_chage_atom_t *container);

void cf_chage_atom_destroy(cf_chage_atom_t *atom);

cf_chage_atom_t *cf_chage_atom_get_contained(cf_chage_atom_t *atom,
    position_t position);

cf_chage_atom_t *cf_chage_atom_get_container(cf_chage_atom_t *atom);

cf_x_core_bool_t cf_chage_atom_spark(cf_chage_atom_t *atom);

cf_x_core_bool_t cf_chage_atom_spark_distance(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired);

#endif
