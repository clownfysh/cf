#include "x/container/map.h"
#include "x/core/long.h"
#include "x/core/objects.h"
#include "x/core/string.h"
#include "x/core/tools.h"

int main(int argc, char *argv[])
{
  x_container_map_t *map;
  x_core_objects_t objects;

  x_core_objects_init(&objects);

  char *a_left = "apple";
  unsigned long a_right = 0;

  char *b_left = "bear";
  unsigned long b_right = 1;

  unsigned long *x_right;

  map = x_container_map_create(&objects.string_objectey,
      &objects.long_objectey, X_CONTAINER_MAP_DONT_DESTROY);
  if (!map) {
    x_core_trace_exit("x_container_map_create");
  }

  if (!x_container_map_add(map, a_left, &a_right)) {
    x_core_trace_exit("x_container_map_add");
  }

  if (!x_container_map_add(map, b_left, &b_right)) {
    x_core_trace_exit("x_container_map_add");
  }

  x_container_map_print(map);

  x_right = x_container_map_find(map, b_left);
  if (x_right) {
    printf(":%lu:\n", *x_right);
  } else {
    x_core_trace_exit("x_container_map_find");
  }

  x_container_map_destroy(map);
  x_core_objects_free(&objects);

  return 0;
}
