#include "inferno/core/actorey.h"
#include "x/core/standard.h"

void inferno_core_actorey_free(inferno_core_actorey_t *actorey) {}

void inferno_core_actorey_init(inferno_core_actorey_t *actorey,
    inferno_core_actorey_create_f create,
    inferno_core_actorey_create_random_f create_random,
    x_core_compare_f compare, x_core_copy_f copy,
    x_core_destroy_f destroy, x_core_get_object_f get_box_cell,
    x_core_set_object_f set_box_cell)
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
