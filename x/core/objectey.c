#include "x/core/standard.h"
#include "x/core/objectey.h"

void x_core_objectey_init(x_core_objectey_t *objectey,
    x_core_compare_f compare, x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_equal_f equal, x_core_get_as_string_f get_as_string,
    x_core_mod_f mod)
{
  assert(objectey);

  objectey->compare = compare;
  objectey->copy = copy;
  objectey->destroy = destroy;
  objectey->equal = equal;
  objectey->get_as_string = get_as_string;
  objectey->mod = mod;
}
