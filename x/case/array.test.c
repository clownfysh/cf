#include "x/case/array.h"
#include "x/core/standard.h"
#include "x/core/string.h"

int main(int argc, char *argv[])
{
  x_case_array_t *array;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;

  array = x_case_array_create(4, x_core_string_compare,
      x_core_string_copy, x_core_string_destroy);

  x_case_array_add(array, 0, strdup("puppy"));
  x_case_array_add(array, 1, strdup("rooster"));
  x_case_array_add(array, 2, strdup("cow"));
  x_case_array_add(array, 3, strdup("spots"));

  for (index = 0; index < 4; index++) {
    object = x_case_array_find(array, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = x_case_array_find_random(array);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  x_case_array_iterate_start(array);
  while ((object = x_case_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  x_case_array_iterate_start(array);
  while ((object = x_case_array_iterate_next(array))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  x_case_array_destroy(array);

  x_case_array_t *array2;
  array2 = x_case_array_create(1, x_core_string_compare,
      x_core_string_copy, x_core_string_destroy);
  x_case_array_add(array2, 0, malloc(7));
  x_case_array_remove(array2, 0);
  x_case_array_destroy(array2);

  return 0;
}
