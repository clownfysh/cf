#ifndef i_clink_system_h
#define i_clink_system_h

#include "i/clink/concept.h"

struct i_clink_system_t;
typedef struct i_clink_system_t i_clink_system_t;

i_clink_system_t *i_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, i_compare_f compare, i_destroy_f destroy);

void i_clink_system_destroy(i_clink_system_t *system);

i_clink_concept_t *i_clink_system_get_concept(i_clink_system_t *system,
    unsigned long concept_index);

void *i_clink_system_get_linked_object(i_clink_system_t *system,
    unsigned long concept_index, unsigned long object_index);

void *i_clink_system_get_object(i_clink_system_t *system,
    unsigned long concept_index);

void i_clink_system_link(i_clink_system_t *system, void *object_a,
    void *object_b);

void i_clink_system_print(i_clink_system_t *system,
    i_get_as_string_f get_as_string);

#endif
