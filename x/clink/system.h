#ifndef x_clink_system_h
#define x_clink_system_h

#include "x/clink/concept.h"
#include "x/core/bool.h"

struct x_clink_system_t;
typedef struct x_clink_system_t x_clink_system_t;

typedef x_core_bool_t (*x_clink_system_think_f)(x_clink_system_t *system,
    void *object);

x_clink_system_t *x_clink_system_create(unsigned long max_concepts,
    unsigned long max_links, x_core_compare_f compare,
    x_core_destroy_f destroy);

void x_clink_system_destroy(x_clink_system_t *system);

x_clink_concept_t *x_clink_system_get_concept(x_clink_system_t *system,
    unsigned long concept_index);

x_core_bool_t x_clink_system_get_index(x_clink_system_t *system, void *object,
    unsigned long *index);

void *x_clink_system_get_linked_object(x_clink_system_t *system,
    unsigned long concept_index, unsigned long link_index);

void *x_clink_system_get_object(x_clink_system_t *system,
    unsigned long concept_index);

void x_clink_system_link(x_clink_system_t *system, void *object_a,
    void *object_b);

void x_clink_system_print(x_clink_system_t *system,
    x_core_get_as_string_f get_as_string);

x_core_bool_t x_clink_system_think_train(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_objects);

x_core_bool_t x_clink_system_think_tree(x_clink_system_t *system,
    x_clink_system_think_f think, unsigned long max_objects);

#endif
