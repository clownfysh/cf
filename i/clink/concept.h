#ifndef i_clink_concept_h
#define i_clink_concept_h

#include "i/object.h"

struct i_clink_concept_t;
typedef struct i_clink_concept_t i_clink_concept_t;

i_clink_concept_t *i_clink_concept_create(void *object,
    unsigned long max_links, i_compare_f compare, i_destroy_f destroy);

void i_clink_concept_destroy(i_clink_concept_t *concept);

void *i_clink_concept_get_linked_object(i_clink_concept_t *concept,
    unsigned long object_index);

void *i_clink_concept_get_object(i_clink_concept_t *concept);

void i_clink_concept_note_object(i_clink_concept_t *concept, void *object);

void i_clink_concept_print(i_clink_concept_t *concept,
    i_get_as_string_f get_as_string);

#endif
