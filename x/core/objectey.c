#include "x/core/standard.h"
#include "x/core/objectey.h"

void x_core_objectey_init(x_core_objectey_t *objectey,
    x_core_compare_f compare, x_core_copy_f copy,
    x_core_destroy_f destroy,
    x_core_get_as_string_f get_as_string)
{
  assert(objectey);

  objectey->compare = compare;
  objectey->copy = copy;
  objectey->destroy = destroy;
  objectey->get_as_string = get_as_string;
}
