#ifndef chage_atom_h
#define chage_atom_h

#include "chage/sing/system.h"
#include "chage/tools.h"
#include "i/tools.h"

struct chage_atom_t;
typedef struct chage_atom_t chage_atom_t;

chage_atom_t *chage_atom_create(chage_atom_t *container);

void chage_atom_destroy(chage_atom_t *atom);

chage_atom_t *chage_atom_get_contained(chage_atom_t *atom,
    position_t position);

chage_atom_t *chage_atom_get_container(chage_atom_t *atom);

i_bool_t chage_atom_spark(chage_atom_t *atom);

i_bool_t chage_atom_spark_distance(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, i_bool_t *interval_expired);

#endif
