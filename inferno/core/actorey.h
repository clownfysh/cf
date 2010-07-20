#ifndef inferno_core_actorey_h
#define inferno_core_actorey_h

#include "h/core/bitarray.h"

typedef void *(*inferno_core_actorey_create_f)(void *search_system,
    h_core_bitarray_t *solution);

typedef void *(*inferno_core_actorey_create_random_f)(void *search_system);

typedef h_core_bitarray_t *(*inferno_core_actorey_get_solution_f)
(void *actor);

struct inferno_core_actorey_t {
  inferno_core_actorey_create_f create;
  inferno_core_actorey_create_random_f create_random;
  h_core_compare_f compare;
  h_core_copy_f copy;
  h_core_destroy_f destroy;
  h_core_get_object_f get_box_cell;
  h_core_set_object_f set_box_cell;
};
typedef struct inferno_core_actorey_t inferno_core_actorey_t;

void inferno_core_actorey_free(inferno_core_actorey_t *actorey);

void inferno_core_actorey_init(inferno_core_actorey_t *actorey,
    inferno_core_actorey_create_f create,
    inferno_core_actorey_create_random_f create_random,
    h_core_compare_f compare, h_core_copy_f copy,
    h_core_destroy_f destroy, h_core_get_object_f get_box_cell,
    h_core_set_object_f set_box_cell);

#endif
