#ifndef x_clink_concept_h
#define x_clink_concept_h

#include "x/core/object.h"

struct x_clink_concept_t;
typedef struct x_clink_concept_t x_clink_concept_t;

x_clink_concept_t *x_clink_concept_create(void *concept,
    unsigned long max_links, x_core_compare_f compare,
    x_core_destroy_f destroy);

void x_clink_concept_destroy(x_clink_concept_t *concept);

void *x_clink_concept_get_linked_object(x_clink_concept_t *concept,
    unsigned long link_index);

void *x_clink_concept_get_object(x_clink_concept_t *concept);

void x_clink_concept_note_object(x_clink_concept_t *concept, void *object);

void x_clink_concept_print(x_clink_concept_t *concept,
    x_core_get_as_string_f get_as_string);

#endif
