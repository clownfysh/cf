#ifndef inferno_core_actorey_h
#define inferno_core_actorey_h

#include "x/core/bitarray.h"

typedef void *(*inferno_core_actorey_create_f)(void *searcx_system,
    x_core_bitarray_t *solution);

typedef void *(*inferno_core_actorey_create_random_f)(void *searcx_system);

typedef x_core_bitarray_t *(*inferno_core_actorey_get_solution_f)
(void *actor);

struct inferno_core_actorey_t {
  inferno_core_actorey_create_f create;
  inferno_core_actorey_create_random_f create_random;
  x_core_compare_f compare;
  x_core_copy_f copy;
  x_core_destroy_f destroy;
  x_core_get_object_f get_box_cell;
  x_core_set_object_f set_box_cell;
};
typedef struct inferno_core_actorey_t inferno_core_actorey_t;

void inferno_core_actorey_free(inferno_core_actorey_t *actorey);

void inferno_core_actorey_init(inferno_core_actorey_t *actorey,
    inferno_core_actorey_create_f create,
    inferno_core_actorey_create_random_f create_random,
    x_core_compare_f compare, x_core_copy_f copy,
    x_core_destroy_f destroy, x_core_get_object_f get_box_cell,
    x_core_set_object_f set_box_cell);

#endif
