#include "x/container/set.h"
#include "x/container/mbin.h"

typedef x_container_mbin_t x_container_set_t;

x_core_bool_t x_container_set_add(x_container_set_t *set, void *object)
{
  return x_container_mbin_add(set, object);
}

x_core_bool_t x_container_set_add_replace(x_container_set_t *set, void *object)
{
  return x_core_bool_false;
}

x_container_set_t *x_container_set_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy)
{
  return x_container_mbin_create(x_core_mod_f mod, x_core_equal_f equal,
      x_core_destroy_f destroy, X_CONTAINER_MBIN_SET_TYPE_SET);
}

void x_container_set_destroy(void *set_object)
{
  x_container_mbin_destroy(set_object);
}

void *x_container_set_find(x_container_set_t *set, void *decoy_object)
{
  return x_container_mbin_find(set, decoy_object);
}

void *x_container_set_find_any(x_container_set_t *set)
{
  return NULL;
}

void x_container_set_print(x_container_set_t *set,
    x_core_get_as_string_f get_object_as_string)
{
  assert(set);
  assert(get_object_as_string);
  x_implement();
}

x_core_bool_t x_container_set_remove(x_container_set_t *set, void *object)
{
  x_container_mbin_remove(set, object);
  return x_core_bool_true;
}
