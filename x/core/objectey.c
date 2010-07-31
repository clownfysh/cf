#include "cf/x/core/standard.h"
#include "cf/x/core/objectey.h"

void cf_x_core_objectey_init(cf_x_core_objectey_t *objectey,
    cf_x_core_object_compare_f compare, cf_x_core_object_copy_f copy,
    cf_x_core_object_destroy_f destroy, cf_x_core_object_compare_equal_f equal,
    cf_x_core_object_get_as_string_f get_as_string, cf_x_core_object_mod_f mod)
{
  assert(objectey);

  objectey->compare = compare;
  objectey->copy = copy;
  objectey->destroy = destroy;
  objectey->equal = equal;
  objectey->get_as_string = get_as_string;
  objectey->mod = mod;
}
