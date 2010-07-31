#include "x/case/array.h"
#include "x/core/bitarray.h"
#include "x/core/tools.h"
#include "x/math/combination.h"

struct x_math_combination_t {
  x_case_set_t *elements_set;
  x_case_array_t *elements;
  unsigned long element_count;
  x_core_bitarray_t *flags;

  x_core_bool_t iterate_first;

  x_core_objectey_t *set_objectey;
};

static x_case_set_t *create_set_based_on_flags
(x_math_combination_t *combination);

x_case_set_t *create_set_based_on_flags(x_math_combination_t *combination)
{
  assert(combination);
  x_case_set_t *set;
  unsigned long each_element;
  void *object;
  void *object_copy;

  set = x_case_set_create(combination->set_objectey);
  if (set) {
    for (each_element = 0; each_element < combination->element_count;
         each_element++) {
      if (1 == x_core_bitarray_get_bit(combination->flags, each_element)) {
        object = x_case_array_find(combination->elements, each_element);
        object_copy = combination->set_objectey->copy(object);
        if (object_copy) {
          if (!x_case_set_add(set, object_copy)) {
            x_core_trace("x_case_set_add");
          }
        } else {
          x_core_trace("combination->copy");
        }
      }
    }
  } else {
    x_core_trace("x_case_set_create");
  }

  return set;
}

x_math_combination_t *x_math_combination_create(x_case_set_t *set)
{
  assert(set);
  assert(x_case_set_get_size(set) > 0);
  x_math_combination_t *combination;
  x_core_bool_t so_far_so_good;

  combination = malloc(sizeof *combination);
  if (combination) {
    combination->flags = NULL;
    combination->iterate_first = x_core_bool_false;
    combination->element_count = x_case_set_get_size(set);
    combination->set_objectey = x_case_set_get_objectey(set);
    combination->elements_set = x_case_set_copy(set);
    if (combination->elements_set) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_case_set_copy");
    }

  } else {
    so_far_so_good = x_core_bool_false;

    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    combination->elements = x_case_set_get_as_array
      (combination->elements_set);
    if (!combination->elements) {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_case_array_create_reference_from_set");
    }
  }

  if (so_far_so_good) {
    combination->flags = x_core_bitarray_create
      (combination->element_count);
    if (!combination->flags) {
      so_far_so_good = x_core_bool_false;
    }
  }

  if (!so_far_so_good && combination) {
    if (combination->elements_set) {
      x_case_set_destroy(combination->elements_set);
    }
    if (combination->elements) {
      x_case_array_destroy(combination->elements);
    }
    if (combination->flags) {
      x_core_bitarray_destroy(combination->flags);
    }
  }

  return combination;
}

void x_math_combination_destroy(x_math_combination_t *combination)
{
  assert(combination);

  x_case_set_destroy(combination->elements_set);
  x_case_array_destroy(combination->elements);
  x_core_bitarray_destroy(combination->flags);
  free(combination);
}

x_case_set_t *x_math_combination_iterate_next
(x_math_combination_t *combination)
{
  assert(combination);
  x_case_set_t *set;

  if (combination->iterate_first) {
    set = create_set_based_on_flags(combination);
    if (!set) {
      x_core_trace("create_set_based_on_flags");
    }
    combination->iterate_first = x_core_bool_false;
  } else {
    if (x_core_bitarray_increment(combination->flags)) {
      set = create_set_based_on_flags(combination);
      if (!set) {
        x_core_trace("create_set_based_on_flags");
      }
    } else {
      set = NULL;
    }
  }

  return set;
}

void x_math_combination_iterate_start(x_math_combination_t *combination)
{
  assert(combination);

  x_core_bitarray_unset_all(combination->flags);
  x_core_bitarray_set_bit(combination->flags, 0, 1);
  combination->iterate_first = x_core_bool_true;
}
