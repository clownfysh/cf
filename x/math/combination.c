#include "cf/x/case/array.h"
#include "cf/x/core/bitarray.h"
#include "cf/x/core/tools.h"
#include "cf/x/math/combination.h"

struct cf_x_math_combination_t {
  cf_x_case_set_t *elements_set;
  cf_x_case_array_t *elements;
  unsigned long element_count;
  cf_x_core_bitarray_t *flags;

  cf_x_core_bool_t iterate_first;

  cf_x_core_objectey_t *set_objectey;
};

static cf_x_case_set_t *create_set_based_on_flags
(cf_x_math_combination_t *combination);

cf_x_case_set_t *create_set_based_on_flags(cf_x_math_combination_t *combination)
{
  assert(combination);
  cf_x_case_set_t *set;
  unsigned long each_element;
  void *object;
  void *object_copy;

  set = cf_x_case_set_create(combination->set_objectey);
  if (set) {
    for (each_element = 0; each_element < combination->element_count;
         each_element++) {
      if (1 == cf_x_core_bitarray_get_bit(combination->flags, each_element)) {
        object = cf_x_case_array_find(combination->elements, each_element);
        object_copy = combination->set_objectey->copy(object);
        if (object_copy) {
          if (!cf_x_case_set_add(set, object_copy)) {
            cf_x_core_trace("x_case_set_add");
          }
        } else {
          cf_x_core_trace("combination->copy");
        }
      }
    }
  } else {
    cf_x_core_trace("x_case_set_create");
  }

  return set;
}

cf_x_math_combination_t *cf_x_math_combination_create(cf_x_case_set_t *set)
{
  assert(set);
  assert(cf_x_case_set_get_size(set) > 0);
  cf_x_math_combination_t *combination;
  cf_x_core_bool_t so_far_so_good;

  combination = malloc(sizeof *combination);
  if (combination) {
    combination->flags = NULL;
    combination->iterate_first = cf_x_core_bool_false;
    combination->element_count = cf_x_case_set_get_size(set);
    combination->set_objectey = cf_x_case_set_get_objectey(set);
    combination->elements_set = cf_x_case_set_copy(set);
    if (combination->elements_set) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_case_set_copy");
    }

  } else {
    so_far_so_good = cf_x_core_bool_false;

    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    combination->elements = cf_x_case_set_get_as_array
      (combination->elements_set);
    if (!combination->elements) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_case_array_create_reference_from_set");
    }
  }

  if (so_far_so_good) {
    combination->flags = cf_x_core_bitarray_create
      (combination->element_count);
    if (!combination->flags) {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (!so_far_so_good && combination) {
    if (combination->elements_set) {
      cf_x_case_set_destroy(combination->elements_set);
    }
    if (combination->elements) {
      cf_x_case_array_destroy(combination->elements);
    }
    if (combination->flags) {
      cf_x_core_bitarray_destroy(combination->flags);
    }
  }

  return combination;
}

void cf_x_math_combination_destroy(cf_x_math_combination_t *combination)
{
  assert(combination);

  cf_x_case_set_destroy(combination->elements_set);
  cf_x_case_array_destroy(combination->elements);
  cf_x_core_bitarray_destroy(combination->flags);
  free(combination);
}

cf_x_case_set_t *cf_x_math_combination_iterate_next
(cf_x_math_combination_t *combination)
{
  assert(combination);
  cf_x_case_set_t *set;

  if (combination->iterate_first) {
    set = create_set_based_on_flags(combination);
    if (!set) {
      cf_x_core_trace("create_set_based_on_flags");
    }
    combination->iterate_first = cf_x_core_bool_false;
  } else {
    if (cf_x_core_bitarray_increment(combination->flags)) {
      set = create_set_based_on_flags(combination);
      if (!set) {
        cf_x_core_trace("create_set_based_on_flags");
      }
    } else {
      set = NULL;
    }
  }

  return set;
}

void cf_x_math_combination_iterate_start(cf_x_math_combination_t *combination)
{
  assert(combination);

  cf_x_core_bitarray_unset_all(combination->flags);
  cf_x_core_bitarray_set_bit(combination->flags, 0, 1);
  combination->iterate_first = cf_x_core_bool_true;
}
