#include "x/case/array.h"
#include "x/case/set.h"
#include "x/core/long.h"

#if defined X_CASE_SET_BINARY_TREE_IMPL
#include "x/case/set.impl.binary_tree.c"
#elif defined X_CASE_SET_HASH_TABLE_IMPL
#include "x/case/set.imple.hash_table.c"
#elif defined X_CASE_SET_HASHA_IMPL
#include "x/case/set.impl.hasha.c"
#elif defined X_CASE_SET_MBIN_IMPL
#include "x/case/set.impl.mbin.c"
#elif defined X_CASE_SET_SKIP_LIST_IMPL
#include "x/case/set.impl.skip_list.c"
#else
#include "x/case/set.impl.mbin.c"
#endif

x_core_bool_t x_case_set_absorb(x_case_set_t *set,
    x_case_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  x_core_bool_t success;

  success = x_core_bool_true;

  x_case_set_iterate_start(absorb_these);
  while ((object = x_case_set_iterate_next(absorb_these))) {
    if (!x_case_set_add(set, object)) {
      success = x_core_bool_false;
      x_core_trace("x_case_set_add");
    }
  }

  return success;
}

x_core_bool_t x_case_set_absorb_array(x_case_set_t *set,
    x_case_array_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  x_core_bool_t success;

  success = x_core_bool_true;

  x_case_array_iterate_start(absorb_these);
  while ((object = x_case_array_iterate_next(absorb_these))) {
    if (!x_case_set_add(set, object)) {
      success = x_core_bool_false;
      x_core_trace("x_case_set_add");
    }
  }

  return success;
}

x_core_bool_t x_case_set_absorb_copy(x_case_set_t *set,
    x_case_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(x_case_set_get_objectey(set)->destroy);
  void *object;
  void *object_copy;
  x_core_bool_t success;
  x_core_objectey_t *objectey = x_case_set_get_objectey(set);

  success = x_core_bool_true;

  x_case_set_iterate_start(absorb_these);
  while ((object = x_case_set_iterate_next(absorb_these))) {
    object_copy = objectey->copy(object);
    if (object_copy) {
      if (!x_case_set_add(set, object_copy)) {
        objectey->destroy(object_copy);
      }
    } else {
      success = x_core_bool_false;
      x_core_trace("copy");
    }
  }

  return success;
}

x_core_bool_t x_case_set_absorb_list_copy(x_case_set_t *set,
    x_case_list_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(x_case_set_get_objectey(set)->destroy);
  void *object;
  void *object_copy;
  x_core_bool_t success;
  x_core_objectey_t *objectey = x_case_set_get_objectey(set);

  success = x_core_bool_true;

  x_case_list_iterate_start(absorb_these);
  while ((object = x_case_list_iterate_next(absorb_these))) {
    object_copy = objectey->copy(object);
    if (object_copy) {
      if (!x_case_set_add(set, object_copy)) {
        objectey->destroy(object_copy);
      }
    } else {
      success = x_core_bool_false;
      x_core_trace("copy");
    }
  }

  return success;
}

x_core_bool_t x_case_set_add_to_message(x_case_set_t *set,
    x_core_message_t *message, x_core_message_add_to_message_f add_to_message)
{
  x_core_bool_t success;
  long set_size;
  void *object;

  set_size = x_case_set_get_size(set);
  success = x_core_message_add_long(message, &set_size);
  x_case_set_iterate_start(set);
  while ((object = x_case_set_iterate_next(set))) {
    if (!add_to_message(object, message)) {
      success = x_core_bool_false;
      break;
    }
  }

  return success;
}

void x_case_set_clear(x_case_set_t *set)
{
  assert(set);

  x_case_set_iterate_start(set);
  while (x_case_set_iterate_next(set)) {
    x_case_set_iterate_remove(set);
  }
}

int x_case_set_compare(void *set_object_a,
    void *set_object_b)
{
  assert(set_object_a);
  assert(set_object_b);
  x_case_set_t *set_a;
  x_case_set_t *set_b;
  int compare_result;
  void *object_a;
  void *object_b;
  unsigned long size_a;
  unsigned long size_b;
  x_core_compare_f compare;

  set_a = set_object_a;
  set_b = set_object_b;

  size_a = x_case_set_get_size(set_a);
  size_b = x_case_set_get_size(set_b);

  compare_result = x_core_long_compare(&size_a, &size_b);
  if (0 == compare_result) {
    compare = x_case_set_get_objectey(set_a)->compare;
    x_case_set_iterate_start(set_a);
    x_case_set_iterate_start(set_b);
    while ((object_a = x_case_set_iterate_next(set_a))) {
      object_b = x_case_set_iterate_next(set_b);
      compare_result = compare(object_a, object_b);
      if (0 != compare_result) {
        break;
      }
    }
  }

  return compare_result;
}

x_core_bool_t x_case_set_contains(x_case_set_t *superset,
    x_case_set_t *subset)
{
  assert(superset);
  assert(subset);
  x_core_bool_t contains_it;
  void *object;

  contains_it = x_core_bool_true;

  x_case_set_iterate_start(subset);
  while ((object = x_case_set_iterate_next(subset))) {
    if (!x_case_set_find(superset, object)) {
      contains_it = x_core_bool_false;
      break;
    }
  }

  return contains_it;
}

void *x_case_set_copy(void *set_object)
{
  assert(set_object);
  x_case_set_t *set = set_object;
  x_case_set_t *copy;
  void *original_object;
  void *copied_object;
  x_core_objectey_t *objectey = x_case_set_get_objectey(set);

  copy = x_case_set_create(objectey);
  if (copy) {
    x_case_set_iterate_start(set);
    while ((original_object = x_case_set_iterate_next(set))) {
      copied_object = objectey->copy(original_object);
      if (copied_object) {
        if (!x_case_set_add(copy, copied_object)) {
          x_core_trace("x_case_set_add");
          objectey->destroy(copied_object);
          x_case_set_destroy(copy);
          copy = NULL;
          break;
        }
      } else {
        x_core_trace("copy");
        x_case_set_destroy(copy);
        copy = NULL;
        break;
      }
    }
  } else {
    x_core_trace("x_case_set_create");
  }

  return copy;
}

x_case_set_t *x_case_set_create_from_message
(x_core_objectey_t *objectey, x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message)
{
  assert(objectey);
  assert(message);
  assert(create_from_message);
  x_case_set_t *set;
  long set_size;
  long each_object;
  void *object;

  set = x_case_set_create(objectey);
  if (set) {
    set_size = x_core_message_take_long_value(message);
    for (each_object = 0; each_object < set_size; each_object++) {
      object = create_from_message(message);
      if (!x_case_set_add(set, object)) {
        x_core_trace("x_case_set_add");
      }
    }
  } else {
    x_core_trace("x_case_set_create");
  }

  return set;
}

x_case_set_t *x_case_set_create_from_union(x_case_set_t *set_a,
    x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  x_case_set_t *union_set;
  void *object;

  union_set = x_case_set_copy(set_a);
  if (union_set) {
    x_case_set_iterate_start(set_b);
    while ((object = x_case_set_iterate_next(set_b))) {
      x_case_set_add(union_set, object);
    }
  } else {
    x_core_trace("x_case_set_copy");
  }

  return union_set;
}

x_case_set_t *x_case_set_difference(x_case_set_t *set_a,
    x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  x_case_set_t *difference;

  difference = x_case_set_copy(set_a);
  if (difference) {
    if (!x_case_set_expunge(difference, set_b)) {
      x_core_trace("x_case_set_expunge");
      x_case_set_destroy(difference);
      difference = NULL;
    }
  } else {
    x_core_trace("x_case_set_copy");
  }

  return difference;
}

void x_case_set_dont_destroy_objects(x_case_set_t *set)
{
  x_case_set_get_objectey(set)->destroy = NULL;
}

x_core_bool_t x_case_set_expunge(x_case_set_t *set,
    x_case_set_t *expunge_these)
{
  assert(set);
  assert(expunge_these);
  x_core_bool_t success;
  void *object;

  x_case_set_iterate_start(expunge_these);
  while ((object = x_case_set_iterate_next(expunge_these))) {
    x_case_set_remove(set, object);
  }

  /*
    TODO: return codes in x_core_set interface don't allow us to determine
    success or failure here
  */
  success = x_core_bool_true;

  return success;
}

void *x_case_set_find_copy(x_case_set_t *set, void *decoy_object)
{
  void *found_object;
  void *found_object_copy;

  found_object = x_case_set_find(set, decoy_object);
  if (found_object) {
    found_object_copy = x_case_set_get_objectey(set)->copy(found_object);
    if (!found_object_copy) {
      x_core_trace("copy");
    }
  } else {
    found_object_copy = NULL;
  }

  return found_object_copy;
}

void *x_case_set_find_take(x_case_set_t *set,
    void *decoy_object)
{
  void *object;

  object = x_case_set_find(set, decoy_object);
  if (object) {
    if (!x_case_set_remove(set, decoy_object)) {
      x_core_trace("x_case_set_remove");
    }
  }

  return object;
}

x_case_array_t *x_case_set_get_as_array(x_case_set_t *set)
{
  assert(set);
  x_case_array_t *array;
  void *object;
  unsigned long object_count;
  unsigned long object_index;
  x_core_objectey_t *objectey = x_case_set_get_objectey(set);

  object_count = x_case_set_get_size(set);

  array = x_case_array_create(object_count, objectey->compare,
      objectey->copy, X_CORE_NO_DESTROY_FUNCTION);
  if (array) {
    object_index = 0;
    x_case_set_iterate_start(set);
    while ((object = x_case_set_iterate_next(set))) {
      x_case_array_add(array, object_index, object);
      object_index++;
    }
  } else {
    x_core_trace("x_case_array_create");
  }

  return array;
}

x_case_list_t *x_case_set_get_as_list(x_case_set_t *set)
{
  assert(set);
  x_case_list_t *list;
  void *object;
  x_core_objectey_t *objectey = x_case_set_get_objectey(set);

  list = x_case_list_create(objectey->compare, objectey->copy,
      X_CORE_NO_DESTROY_FUNCTION);
  if (list) {
    x_case_set_iterate_start(set);
    while ((object = x_case_set_iterate_next(set))) {
      x_case_list_add_last(list, object);
    }
  } else {
    x_core_trace("x_case_list_create");
  }

  return list;
}

char *x_case_set_get_as_delimited_string(x_case_set_t *set,
    x_core_get_as_string_f get_as_string, const char *delimiter)
{
  assert(set);
  assert(get_as_string);
  assert(delimiter);
  char *string;
  void *object;
  char *object_string;
  unsigned long object_index;
  unsigned long last_object_index;
  x_core_bool_t success;

  string = NULL;
  last_object_index = x_case_set_get_size(set) - 1;
  success = x_core_bool_true;

  object_index = 0;
  x_case_set_iterate_start(set);
  while ((object = x_case_set_iterate_next(set))) {
    object_string = get_as_string(object);
    if (object_string) {
      string = x_core_string_append(string, object_string);
      free(object_string);
      if (string) {
        if (object_index != last_object_index) {
          string = x_core_string_append(string, delimiter);
          if (!string) {
            success = x_core_bool_false;
            x_core_trace("x_core_string_append");
            break;
          }
        }
      } else {
        success = x_core_bool_false;
        x_core_trace("x_core_string_append");
        break;
      }
    } else {
      success = x_core_bool_false;
      x_core_trace("get_as_string");
      break;
    }
    object_index++;
  }

  if (!success && string) {
    free(string);
    string = NULL;
  }

  return string;
}

x_core_bool_t x_case_set_overlaps(x_case_set_t *set_a,
    x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  x_core_bool_t overlaps;
  void *object;
  unsigned long set_a_size;
  unsigned long set_b_size;
  x_case_set_t *larger_set;
  x_case_set_t *smaller_set;

  set_a_size = x_case_set_get_size(set_a);
  set_b_size = x_case_set_get_size(set_b);
  if (set_a_size > set_b_size) {
    larger_set = set_a;
    smaller_set = set_b;
  } else {
    larger_set = set_b;
    smaller_set = set_a;
  }

  overlaps = x_core_bool_false;
  x_case_set_iterate_start(smaller_set);
  while ((object = x_case_set_iterate_next(smaller_set))) {
    if (x_case_set_find(larger_set, object)) {
      overlaps = x_core_bool_true;
      break;
    }
  }

  return overlaps;
}
