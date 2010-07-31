#include "x/case/set.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/math/combination.h"

int main(int argc, char *argv[])
{
  x_math_combination_t *combination;
  x_case_set_t *set;
  x_case_set_t *subset;
  x_core_objectey_t set_objectey;

  x_core_objectey_init(&set_objectey, x_core_string_compare,
      x_core_string_copy, X_CORE_NO_DESTROY_FUNCTION, x_core_string_equal,
      x_core_string_get_as_string, x_core_string_mod);
  set = x_case_set_create(&set_objectey);
  if (!set) {
    x_core_trace_exit("x_case_set_create");
  }

  x_case_set_add(set, "rooster");
  x_case_set_add(set, "cow");
  x_case_set_add(set, "pig");
  x_case_set_add(set, "duck");

  combination = x_math_combination_create(set);
  if (!combination) {
    x_core_trace_exit("x_math_combination_create");
  }

  x_math_combination_iterate_start(combination);
  while ((subset = x_math_combination_iterate_next(combination))) {
    x_case_set_print(subset, x_core_string_get_as_string);
    x_case_set_destroy(subset);
  }

  x_math_combination_destroy(combination);
  x_case_set_destroy(set);

  return 0;
}
