#include "x/case/set.h"
#include "x/case/hasha.h"

x_core_bool_t x_case_set_add(x_case_set_t *set, void *object)
{
  return x_case_hasha_add((x_case_hasha_t *) set, object);
}

x_core_bool_t x_case_set_add_replace(x_case_set_t *set, void *object)
{
  return x_core_bool_false;
}

x_case_set_t *x_case_set_create(x_core_objectey_t *objectey)
{
  return (x_case_set_t *) x_case_hasha_create(objectey,
      X_CASE_HASHA_SET_TYPE_SET);
}

void x_case_set_destroy(void *set_object)
{
  x_case_hasha_destroy(set_object);
}

void *x_case_set_find(x_case_set_t *set, void *decoy_object)
{
  return x_case_hasha_find((x_case_hasha_t *) set, decoy_object);
}

void *x_case_set_find_any(x_case_set_t *set)
{
  return NULL;
}

x_core_objectey_t *x_case_set_get_objectey(x_case_set_t *set)
{
  return x_case_hasha_get_objectey((x_case_hasha_t *) set);
}

unsigned long x_case_set_get_size(x_case_set_t *set)
{
  return x_case_hasha_get_size((x_case_hasha_t *) set);
}

void *x_case_set_iterate_next(x_case_set_t *set)
{
  return x_case_hasha_iterate_next((x_case_hasha_t *) set);
}

void x_case_set_iterate_remove(x_case_set_t *set)
{
  x_case_hasha_iterate_remove((x_case_hasha_t *) set);
}

void x_case_set_iterate_start(x_case_set_t *set)
{
  x_case_hasha_iterate_start((x_case_hasha_t *) set);
}

void x_case_set_print(x_case_set_t *set,
    x_core_get_as_string_f get_object_as_string)
{
  assert(set);
  assert(get_object_as_string);
  void *object;
  char *string;

  x_case_set_iterate_start(set);
  while ((object = x_case_set_iterate_next(set))) {
    string = get_object_as_string(object);
    if (string) {
      printf("%s,", string);
      free(string);
    } else {
      x_trace("get_object_as_string");
    }
  }
  printf("\n");
}

x_core_bool_t x_case_set_remove(x_case_set_t *set, void *object)
{
  x_case_hasha_remove((x_case_hasha_t *) set, object);
  return x_core_bool_true;
}
