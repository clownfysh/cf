#include "cf/inferno/core/actorey.h"
#include "cf/x/core/standard.h"

void cf_inferno_core_actorey_free(cf_inferno_core_actorey_t *actorey) {}

void cf_inferno_core_actorey_init(cf_inferno_core_actorey_t *actorey,
    cf_inferno_core_actorey_create_f create,
    cf_inferno_core_actorey_create_random_f create_random,
    cf_x_core_compare_f compare, cf_x_core_copy_f copy,
    cf_x_core_destroy_f destroy, cf_x_core_get_object_f get_box_cell,
    cf_x_core_set_object_f set_box_cell)
{
  assert(actorey);

  actorey->create = create;
  actorey->create_random = create_random;
  actorey->compare = compare;
  actorey->copy = copy;
  actorey->destroy = destroy;
  actorey->get_box_cell = get_box_cell;
  actorey->set_box_cell = set_box_cell;
}
