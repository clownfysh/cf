#include "cf/x/case/array.h"
#include "cf/x/case/set.h"
#include "cf/x/core/long.h"

#if defined CF_X_CASE_SET_BINARY_TREE_IMPL
#include "cf/x/case/set.impl.binary_tree.c"
#elif defined CF_X_CASE_SET_HASH_TABLE_IMPL
#include "cf/x/case/set.imple.hash_table.c"
#elif defined CF_X_CASE_SET_HASHA_IMPL
#include "cf/x/case/set.impl.hasha.c"
#elif defined CF_X_CASE_SET_MBIN_IMPL
#include "cf/x/case/set.impl.mbin.c"
#elif defined CF_X_CASE_SET_SKIP_LIST_IMPL
#include "cf/x/case/set.impl.skip_list.c"
#else
#include "cf/x/case/set.impl.mbin.c"
#endif

cf_x_core_bool_t cf_x_case_set_absorb(cf_x_case_set_t *set,
    cf_x_case_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  cf_x_case_set_iterate_start(absorb_these);
  while ((object = cf_x_case_set_iterate_next(absorb_these))) {
    if (!cf_x_case_set_add(set, object)) {
      success = cf_x_core_bool_false;
      cf_x_core_trace("x_case_set_add");
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_case_set_absorb_array(cf_x_case_set_t *set,
    cf_x_case_array_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  void *object;
  cf_x_core_bool_t success;

  success = cf_x_core_bool_true;

  cf_x_case_array_iterate_start(absorb_these);
  while ((object = cf_x_case_array_iterate_next(absorb_these))) {
    if (!cf_x_case_set_add(set, object)) {
      success = cf_x_core_bool_false;
      cf_x_core_trace("x_case_set_add");
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_case_set_absorb_copy(cf_x_case_set_t *set,
    cf_x_case_set_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(cf_x_case_set_get_objectey(set)->destroy);
  void *object;
  void *object_copy;
  cf_x_core_bool_t success;
  cf_x_core_objectey_t *objectey = cf_x_case_set_get_objectey(set);

  success = cf_x_core_bool_true;

  cf_x_case_set_iterate_start(absorb_these);
  while ((object = cf_x_case_set_iterate_next(absorb_these))) {
    object_copy = objectey->copy(object);
    if (object_copy) {
      if (!cf_x_case_set_add(set, object_copy)) {
        objectey->destroy(object_copy);
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("copy");
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_case_set_absorb_list_copy(cf_x_case_set_t *set,
    cf_x_case_list_t *absorb_these)
{
  assert(set);
  assert(absorb_these);
  assert(cf_x_case_set_get_objectey(set)->destroy);
  void *object;
  void *object_copy;
  cf_x_core_bool_t success;
  cf_x_core_objectey_t *objectey = cf_x_case_set_get_objectey(set);

  success = cf_x_core_bool_true;

  cf_x_case_list_iterate_start(absorb_these);
  while ((object = cf_x_case_list_iterate_next(absorb_these))) {
    object_copy = objectey->copy(object);
    if (object_copy) {
      if (!cf_x_case_set_add(set, object_copy)) {
        objectey->destroy(object_copy);
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("copy");
    }
  }

  return success;
}

cf_x_core_bool_t cf_x_case_set_add_to_message(cf_x_case_set_t *set,
    cf_x_core_message_t *message, cf_x_core_message_add_to_message_f add_to_message)
{
  cf_x_core_bool_t success;
  long set_size;
  void *object;

  set_size = cf_x_case_set_get_size(set);
  success = cf_x_core_message_add_long(message, &set_size);
  cf_x_case_set_iterate_start(set);
  while ((object = cf_x_case_set_iterate_next(set))) {
    if (!add_to_message(object, message)) {
      success = cf_x_core_bool_false;
      break;
    }
  }

  return success;
}

void cf_x_case_set_clear(cf_x_case_set_t *set)
{
  assert(set);

  cf_x_case_set_iterate_start(set);
  while (cf_x_case_set_iterate_next(set)) {
    cf_x_case_set_iterate_remove(set);
  }
}

int cf_x_case_set_compare(void *set_object_a,
    void *set_object_b)
{
  assert(set_object_a);
  assert(set_object_b);
  cf_x_case_set_t *set_a;
  cf_x_case_set_t *set_b;
  int compare_result;
  void *object_a;
  void *object_b;
  unsigned long size_a;
  unsigned long size_b;
  cf_x_core_compare_f compare;

  set_a = set_object_a;
  set_b = set_object_b;

  size_a = cf_x_case_set_get_size(set_a);
  size_b = cf_x_case_set_get_size(set_b);

  compare_result = cf_x_core_long_compare(&size_a, &size_b);
  if (0 == compare_result) {
    compare = cf_x_case_set_get_objectey(set_a)->compare;
    cf_x_case_set_iterate_start(set_a);
    cf_x_case_set_iterate_start(set_b);
    while ((object_a = cf_x_case_set_iterate_next(set_a))) {
      object_b = cf_x_case_set_iterate_next(set_b);
      compare_result = compare(object_a, object_b);
      if (0 != compare_result) {
        break;
      }
    }
  }

  return compare_result;
}

cf_x_core_bool_t cf_x_case_set_contains(cf_x_case_set_t *superset,
    cf_x_case_set_t *subset)
{
  assert(superset);
  assert(subset);
  cf_x_core_bool_t contains_it;
  void *object;

  contains_it = cf_x_core_bool_true;

  cf_x_case_set_iterate_start(subset);
  while ((object = cf_x_case_set_iterate_next(subset))) {
    if (!cf_x_case_set_find(superset, object)) {
      contains_it = cf_x_core_bool_false;
      break;
    }
  }

  return contains_it;
}

void *cf_x_case_set_copy(void *set_object)
{
  assert(set_object);
  cf_x_case_set_t *set = set_object;
  cf_x_case_set_t *copy;
  void *original_object;
  void *copied_object;
  cf_x_core_objectey_t *objectey = cf_x_case_set_get_objectey(set);

  copy = cf_x_case_set_create(objectey);
  if (copy) {
    cf_x_case_set_iterate_start(set);
    while ((original_object = cf_x_case_set_iterate_next(set))) {
      copied_object = objectey->copy(original_object);
      if (copied_object) {
        if (!cf_x_case_set_add(copy, copied_object)) {
          cf_x_core_trace("x_case_set_add");
          objectey->destroy(copied_object);
          cf_x_case_set_destroy(copy);
          copy = NULL;
          break;
        }
      } else {
        cf_x_core_trace("copy");
        cf_x_case_set_destroy(copy);
        copy = NULL;
        break;
      }
    }
  } else {
    cf_x_core_trace("x_case_set_create");
  }

  return copy;
}

cf_x_case_set_t *cf_x_case_set_create_from_message
(cf_x_core_objectey_t *objectey, cf_x_core_message_t *message,
    cf_x_core_message_create_from_message_f create_from_message)
{
  assert(objectey);
  assert(message);
  assert(create_from_message);
  cf_x_case_set_t *set;
  long set_size;
  long each_object;
  void *object;

  set = cf_x_case_set_create(objectey);
  if (set) {
    set_size = cf_x_core_message_take_long_value(message);
    for (each_object = 0; each_object < set_size; each_object++) {
      object = create_from_message(message);
      if (!cf_x_case_set_add(set, object)) {
        cf_x_core_trace("x_case_set_add");
      }
    }
  } else {
    cf_x_core_trace("x_case_set_create");
  }

  return set;
}

cf_x_case_set_t *cf_x_case_set_create_from_union(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  cf_x_case_set_t *union_set;
  void *object;

  union_set = cf_x_case_set_copy(set_a);
  if (union_set) {
    cf_x_case_set_iterate_start(set_b);
    while ((object = cf_x_case_set_iterate_next(set_b))) {
      cf_x_case_set_add(union_set, object);
    }
  } else {
    cf_x_core_trace("x_case_set_copy");
  }

  return union_set;
}

cf_x_case_set_t *cf_x_case_set_difference(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  cf_x_case_set_t *difference;

  difference = cf_x_case_set_copy(set_a);
  if (difference) {
    if (!cf_x_case_set_expunge(difference, set_b)) {
      cf_x_core_trace("x_case_set_expunge");
      cf_x_case_set_destroy(difference);
      difference = NULL;
    }
  } else {
    cf_x_core_trace("x_case_set_copy");
  }

  return difference;
}

void cf_x_case_set_dont_destroy_objects(cf_x_case_set_t *set)
{
  cf_x_case_set_get_objectey(set)->destroy = NULL;
}

cf_x_core_bool_t cf_x_case_set_expunge(cf_x_case_set_t *set,
    cf_x_case_set_t *expunge_these)
{
  assert(set);
  assert(expunge_these);
  cf_x_core_bool_t success;
  void *object;

  cf_x_case_set_iterate_start(expunge_these);
  while ((object = cf_x_case_set_iterate_next(expunge_these))) {
    cf_x_case_set_remove(set, object);
  }

  /*
    TODO: return codes in cf_x_core_set interface don't allow us to determine
    success or failure here
  */
  success = cf_x_core_bool_true;

  return success;
}

void *cf_x_case_set_find_copy(cf_x_case_set_t *set, void *decoy_object)
{
  void *found_object;
  void *found_object_copy;

  found_object = cf_x_case_set_find(set, decoy_object);
  if (found_object) {
    found_object_copy = cf_x_case_set_get_objectey(set)->copy(found_object);
    if (!found_object_copy) {
      cf_x_core_trace("copy");
    }
  } else {
    found_object_copy = NULL;
  }

  return found_object_copy;
}

void *cf_x_case_set_find_take(cf_x_case_set_t *set,
    void *decoy_object)
{
  void *object;

  object = cf_x_case_set_find(set, decoy_object);
  if (object) {
    if (!cf_x_case_set_remove(set, decoy_object)) {
      cf_x_core_trace("x_case_set_remove");
    }
  }

  return object;
}

cf_x_case_array_t *cf_x_case_set_get_as_array(cf_x_case_set_t *set)
{
  assert(set);
  cf_x_case_array_t *array;
  void *object;
  unsigned long object_count;
  unsigned long object_index;
  cf_x_core_objectey_t *objectey = cf_x_case_set_get_objectey(set);

  object_count = cf_x_case_set_get_size(set);

  array = cf_x_case_array_create(object_count, objectey->compare,
      objectey->copy, CF_X_CORE_NO_DESTROY_FUNCTION);
  if (array) {
    object_index = 0;
    cf_x_case_set_iterate_start(set);
    while ((object = cf_x_case_set_iterate_next(set))) {
      cf_x_case_array_add(array, object_index, object);
      object_index++;
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return array;
}

cf_x_case_list_t *cf_x_case_set_get_as_list(cf_x_case_set_t *set)
{
  assert(set);
  cf_x_case_list_t *list;
  void *object;
  cf_x_core_objectey_t *objectey = cf_x_case_set_get_objectey(set);

  list = cf_x_case_list_create(objectey->compare, objectey->copy,
      CF_X_CORE_NO_DESTROY_FUNCTION);
  if (list) {
    cf_x_case_set_iterate_start(set);
    while ((object = cf_x_case_set_iterate_next(set))) {
      cf_x_case_list_add_last(list, object);
    }
  } else {
    cf_x_core_trace("x_case_list_create");
  }

  return list;
}

char *cf_x_case_set_get_as_delimited_string(cf_x_case_set_t *set,
    cf_x_core_get_as_string_f get_as_string, const char *delimiter)
{
  assert(set);
  assert(get_as_string);
  assert(delimiter);
  char *string;
  void *object;
  char *object_string;
  unsigned long object_index;
  unsigned long last_object_index;
  cf_x_core_bool_t success;

  string = NULL;
  last_object_index = cf_x_case_set_get_size(set) - 1;
  success = cf_x_core_bool_true;

  object_index = 0;
  cf_x_case_set_iterate_start(set);
  while ((object = cf_x_case_set_iterate_next(set))) {
    object_string = get_as_string(object);
    if (object_string) {
      string = cf_x_core_string_append(string, object_string);
      free(object_string);
      if (string) {
        if (object_index != last_object_index) {
          string = cf_x_core_string_append(string, delimiter);
          if (!string) {
            success = cf_x_core_bool_false;
            cf_x_core_trace("x_core_string_append");
            break;
          }
        }
      } else {
        success = cf_x_core_bool_false;
        cf_x_core_trace("x_core_string_append");
        break;
      }
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("get_as_string");
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

cf_x_core_bool_t cf_x_case_set_overlaps(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b)
{
  assert(set_a);
  assert(set_b);
  cf_x_core_bool_t overlaps;
  void *object;
  unsigned long set_a_size;
  unsigned long set_b_size;
  cf_x_case_set_t *larger_set;
  cf_x_case_set_t *smaller_set;

  set_a_size = cf_x_case_set_get_size(set_a);
  set_b_size = cf_x_case_set_get_size(set_b);
  if (set_a_size > set_b_size) {
    larger_set = set_a;
    smaller_set = set_b;
  } else {
    larger_set = set_b;
    smaller_set = set_a;
  }

  overlaps = cf_x_core_bool_false;
  cf_x_case_set_iterate_start(smaller_set);
  while ((object = cf_x_case_set_iterate_next(smaller_set))) {
    if (cf_x_case_set_find(larger_set, object)) {
      overlaps = cf_x_core_bool_true;
      break;
    }
  }

  return overlaps;
}
