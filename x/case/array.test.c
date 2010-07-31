#include "cf/x/case/array.h"
#include "cf/x/core/standard.h"
#include "cf/x/core/string.h"

int main(int argc, char *argv[])
{
  cf_x_case_array_t *array;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;

  array = cf_x_case_array_create(4, cf_x_core_string_compare,
      cf_x_core_string_copy, cf_x_core_string_destroy);

  cf_x_case_array_add(array, 0, strdup("puppy"));
  cf_x_case_array_add(array, 1, strdup("rooster"));
  cf_x_case_array_add(array, 2, strdup("cow"));
  cf_x_case_array_add(array, 3, strdup("spots"));

  for (index = 0; index < 4; index++) {
    object = cf_x_case_array_find(array, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = cf_x_case_array_find_random(array);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  cf_x_case_array_iterate_start(array);
  while ((object = cf_x_case_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  cf_x_case_array_iterate_start(array);
  while ((object = cf_x_case_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  cf_x_case_array_destroy(array);

  cf_x_case_array_t *array2;
  array2 = cf_x_case_array_create(1, cf_x_core_string_compare,
      cf_x_core_string_copy, cf_x_core_string_destroy);
  cf_x_case_array_add(array2, 0, malloc(7));
  cf_x_case_array_remove(array2, 0);
  cf_x_case_array_destroy(array2);

  return 0;
}
