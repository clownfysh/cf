#ifndef cf_inferno_core_actorey_h
#define cf_inferno_core_actorey_h

#include "cf/x/core/bitarray.h"

typedef void *(*cf_inferno_core_actorey_create_f)(void *searcx_system,
    cf_x_core_bitarray_t *solution);

typedef void *(*cf_inferno_core_actorey_create_random_f)(void *searcx_system);

typedef cf_x_core_bitarray_t *(*cf_inferno_core_actorey_get_solution_f)
(void *actor);

struct cf_inferno_core_actorey_t {
  cf_inferno_core_actorey_create_f create;
  cf_inferno_core_actorey_create_random_f create_random;
  cf_x_core_compare_f compare;
  cf_x_core_copy_f copy;
  cf_x_core_destroy_f destroy;
  cf_x_core_get_object_f get_box_cell;
  cf_x_core_set_object_f set_box_cell;
};
typedef struct cf_inferno_core_actorey_t cf_inferno_core_actorey_t;

void cf_inferno_core_actorey_free(cf_inferno_core_actorey_t *actorey);

void cf_inferno_core_actorey_init(cf_inferno_core_actorey_t *actorey,
    cf_inferno_core_actorey_create_f create,
    cf_inferno_core_actorey_create_random_f create_random,
    cf_x_core_compare_f compare, cf_x_core_copy_f copy,
    cf_x_core_destroy_f destroy, cf_x_core_get_object_f get_box_cell,
    cf_x_core_set_object_f set_box_cell);

#endif
