#ifndef cf_inferno_clink_system_h
#define cf_inferno_clink_system_h

#include "cf/inferno/clink/concept.h"
#include "cf/x/core/bool.h"

struct cf_inferno_clink_system_t;
typedef struct cf_inferno_clink_system_t cf_inferno_clink_system_t;

typedef cf_x_core_bool_t (*cf_inferno_clink_system_think_f)(cf_inferno_clink_system_t *system,
    void *object, void *context);

cf_inferno_clink_system_t *cf_inferno_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, cf_x_core_compare_f compare,
    cf_x_core_destroy_f destroy, void *context);

void cf_inferno_clink_system_destroy(cf_inferno_clink_system_t *system);

cf_inferno_clink_concept_t *cf_inferno_clink_system_get_concept(cf_inferno_clink_system_t *system,
    unsigned long concept_index);

cf_x_core_bool_t cf_inferno_clink_system_get_index(cf_inferno_clink_system_t *system, void *object,
    unsigned long *index);

void *cf_inferno_clink_system_get_linked_object(cf_inferno_clink_system_t *system,
    unsigned long concept_index, unsigned long link_index);

void *cf_inferno_clink_system_get_object(cf_inferno_clink_system_t *system,
    unsigned long concept_index);

void cf_inferno_clink_system_link(cf_inferno_clink_system_t *system, void *object_a,
    void *object_b);

void cf_inferno_clink_system_print(cf_inferno_clink_system_t *system,
    cf_x_core_get_as_string_f get_as_string);

cf_x_core_bool_t cf_inferno_clink_system_think_train(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long max_objects);

cf_x_core_bool_t cf_inferno_clink_system_think_tree(cf_inferno_clink_system_t *system,
    cf_inferno_clink_system_think_f think, unsigned long max_objects,
    unsigned long branch_density);

#endif
