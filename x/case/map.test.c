#include "cf/x/case/map.h"
#include "cf/x/core/basic/long.h"
#include "cf/x/core/objects.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"

int main(int argc, char *argv[])
{
  cf_x_case_map_t *map;

  cf_x_core_objects_init();

  char *a_left = "apple";
  unsigned long a_right = 0;

  char *b_left = "bear";
  unsigned long b_right = 1;

  unsigned long *cf_x_right;

  map = cf_x_case_map_create(&cf_x_core_objects.string_iobject,
      &cf_x_core_objects.long_iobject, CF_X_CASE_MAP_DONT_DESTROY);
  if (!map) {
    cf_x_core_trace_exit("x_case_map_create");
  }

  if (!cf_x_case_map_add(map, a_left, &a_right)) {
    cf_x_core_trace_exit("x_case_map_add");
  }

  if (!cf_x_case_map_add(map, b_left, &b_right)) {
    cf_x_core_trace_exit("x_case_map_add");
  }

  cf_x_case_map_print(map);

  cf_x_right = cf_x_case_map_find(map, b_left);
  if (cf_x_right) {
    printf(":%lu:\n", *cf_x_right);
  } else {
    cf_x_core_trace_exit("x_case_map_find");
  }

  cf_x_case_map_destroy(map);

  return 0;
}
