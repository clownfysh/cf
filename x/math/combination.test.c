#include "cf/x/case/set.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/math/combination.h"

int main(int argc, char *argv[])
{
  cf_x_math_combination_t *combination;
  cf_x_case_set_t *set;
  cf_x_case_set_t *subset;
  cf_x_core_iobject_t set_iobject;

  cf_x_core_iobject_init(&set_iobject, cf_x_core_string_compare,
      cf_x_core_string_compare_equal, cf_x_core_string_copy,
      CF_X_CORE_OBJECT_NO_DESTROY_F, cf_x_core_string_get_as_string,
      cf_x_core_string_mod);
  set = cf_x_case_set_create(&set_iobject);
  if (!set) {
    cf_x_core_trace_exit("x_case_set_create");
  }

  cf_x_case_set_add(set, "rise");
  cf_x_case_set_add(set, "run");
  cf_x_case_set_add(set, "earth");
  cf_x_case_set_add(set, "sky");
  cf_x_case_set_add(set, "dot");
  cf_x_case_set_add(set, "oh");
  cf_x_case_set_add(set, "bird");
  cf_x_case_set_add(set, "fish");

  combination = cf_x_math_combination_create(set);
  if (!combination) {
    cf_x_core_trace_exit("x_math_combination_create");
  }

  cf_x_math_combination_iterate_start(combination);
  while ((subset = cf_x_math_combination_iterate_next(combination))) {
    cf_x_case_set_print(subset, cf_x_core_string_get_as_string);
    cf_x_case_set_destroy(subset);
  }

  cf_x_math_combination_destroy(combination);
  cf_x_case_set_destroy(set);

  return 0;
}
