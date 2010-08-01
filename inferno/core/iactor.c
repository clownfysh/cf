#include "cf/inferno/core/iactor.h"
#include "cf/x/core/standard.h"

void cf_inferno_core_iactor_free(cf_inferno_core_iactor_t *iactor) {}

void cf_inferno_core_iactor_init(cf_inferno_core_iactor_t *iactor,
    cf_inferno_core_iactor_create_f create,
    cf_inferno_core_iactor_create_random_f create_random,
    cf_x_core_object_compare_f compare, cf_x_core_object_copy_f copy,
    cf_x_core_object_destroy_f destroy, cf_x_core_object_get_object_f get_box_cell,
    cf_x_core_object_set_object_f set_box_cell)
{
  assert(iactor);

  iactor->create = create;
  iactor->create_random = create_random;
  iactor->compare = compare;
  iactor->copy = copy;
  iactor->destroy = destroy;
  iactor->get_box_cell = get_box_cell;
  iactor->set_box_cell = set_box_cell;
}
