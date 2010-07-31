#include "cf/x/core/standard.h"
#include "cf/x/core/iobject.h"

void cf_x_core_iobject_init(cf_x_core_iobject_t *iobject,
    cf_x_core_object_compare_f compare,
    cf_x_core_object_compare_equal_f compare_equal,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_object_get_as_string_f get_as_string, cf_x_core_object_mod_f mod)
{
  assert(iobject);

  iobject->compare = compare;
  iobject->compare_equal = compare_equal;
  iobject->copy = copy;
  iobject->destroy = destroy;
  iobject->get_as_string = get_as_string;
  iobject->mod = mod;
}
