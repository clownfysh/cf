#ifndef cf_inferno_clink_concept_h
#define cf_inferno_clink_concept_h

#include "cf/x/core/object.h"

struct cf_inferno_clink_concept_t;
typedef struct cf_inferno_clink_concept_t cf_inferno_clink_concept_t;

cf_inferno_clink_concept_t *cf_inferno_clink_concept_create(void *concept,
    unsigned long max_links, cf_x_core_compare_f compare,
    cf_x_core_destroy_f destroy);

void cf_inferno_clink_concept_destroy(cf_inferno_clink_concept_t *concept);

void *cf_inferno_clink_concept_get_linked_object(cf_inferno_clink_concept_t *concept,
    unsigned long link_index);

void *cf_inferno_clink_concept_get_object(cf_inferno_clink_concept_t *concept);

void cf_inferno_clink_concept_note_object(cf_inferno_clink_concept_t *concept, void *object);

void cf_inferno_clink_concept_print(cf_inferno_clink_concept_t *concept,
    cf_x_core_get_as_string_f get_as_string);

#endif
