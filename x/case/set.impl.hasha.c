#include "cf/x/case/set.h"
#include "cf/x/case/hasha.h"

cf_x_core_bool_t cf_x_case_set_add(cf_x_case_set_t *set, void *object)
{
  return cf_x_case_hasha_add((cf_x_case_hasha_t *) set, object);
}

cf_x_core_bool_t cf_x_case_set_add_replace(cf_x_case_set_t *set, void *object)
{
  return cf_x_core_bool_false;
}

cf_x_case_set_t *cf_x_case_set_create(cf_x_core_objectey_t *objectey)
{
  return (cf_x_case_set_t *) cf_x_case_hasha_create(objectey,
      CF_X_CASE_HASHA_SET_TYPE_SET);
}

void cf_x_case_set_destroy(void *set_object)
{
  cf_x_case_hasha_destroy(set_object);
}

void *cf_x_case_set_find(cf_x_case_set_t *set, void *decoy_object)
{
  return cf_x_case_hasha_find((cf_x_case_hasha_t *) set, decoy_object);
}

void *cf_x_case_set_find_any(cf_x_case_set_t *set)
{
  return NULL;
}

cf_x_core_objectey_t *cf_x_case_set_get_objectey(cf_x_case_set_t *set)
{
  return cf_x_case_hasha_get_objectey((cf_x_case_hasha_t *) set);
}

unsigned long cf_x_case_set_get_size(cf_x_case_set_t *set)
{
  return cf_x_case_hasha_get_size((cf_x_case_hasha_t *) set);
}

void *cf_x_case_set_iterate_next(cf_x_case_set_t *set)
{
  return cf_x_case_hasha_iterate_next((cf_x_case_hasha_t *) set);
}

void cf_x_case_set_iterate_remove(cf_x_case_set_t *set)
{
  cf_x_case_hasha_iterate_remove((cf_x_case_hasha_t *) set);
}

void cf_x_case_set_iterate_start(cf_x_case_set_t *set)
{
  cf_x_case_hasha_iterate_start((cf_x_case_hasha_t *) set);
}

void cf_x_case_set_print(cf_x_case_set_t *set,
    cf_x_core_object_get_as_string_f get_object_as_string)
{
  assert(set);
  assert(get_object_as_string);
  void *object;
  char *string;

  cf_x_case_set_iterate_start(set);
  while ((object = cf_x_case_set_iterate_next(set))) {
    string = get_object_as_string(object);
    if (string) {
      printf("%s,", string);
      free(string);
    } else {
      cf_x_trace("get_object_as_string");
    }
  }
  printf("\n");
}

cf_x_core_bool_t cf_x_case_set_remove(cf_x_case_set_t *set, void *object)
{
  cf_x_case_hasha_remove((cf_x_case_hasha_t *) set, object);
  return cf_x_core_bool_true;
}
