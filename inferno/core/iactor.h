#ifndef cf_inferno_core_iactor_h
#define cf_inferno_core_iactor_h

#include "cf/x/core/bitarray.h"

typedef void *(*cf_inferno_core_iactor_create_f)(void *search_system,
    cf_x_core_bitarray_t *solution);

typedef void *(*cf_inferno_core_iactor_create_random_f)(void *search_system);

typedef cf_x_core_bitarray_t *(*cf_inferno_core_iactor_get_solution_f)
(void *actor);

struct cf_inferno_core_iactor_t {
  cf_inferno_core_iactor_create_f create;
  cf_inferno_core_iactor_create_random_f create_random;
  cf_x_core_object_compare_f compare;
  cf_x_core_object_copy_f copy;
  cf_x_core_object_destroy_f destroy;
  cf_x_core_object_get_object_f get_box_cell;
  cf_x_core_object_set_object_f set_box_cell;
};
typedef struct cf_inferno_core_iactor_t cf_inferno_core_iactor_t;

void cf_inferno_core_iactor_free(cf_inferno_core_iactor_t *iactor);

void cf_inferno_core_iactor_init(cf_inferno_core_iactor_t *iactor,
    cf_inferno_core_iactor_create_f create,
    cf_inferno_core_iactor_create_random_f create_random,
    cf_x_core_object_compare_f compare, cf_x_core_object_copy_f copy,
    cf_x_core_object_destroy_f destroy,
    cf_x_core_object_get_object_f get_box_cell,
    cf_x_core_object_set_object_f set_box_cell);

#endif
