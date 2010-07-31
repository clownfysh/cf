#include "x/core/standard.h"
#include "x/case/list.h"
#include "x/core/string.h"

int main(int argc, char *argv[])
{
  x_case_list_t *list;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;
  x_case_list_t *strings;
  char *string;
  char *string_to_split;

  list = x_case_list_create(x_core_string_compare, x_core_string_copy,
      x_core_string_destroy);

  x_case_list_add_first(list, strdup("puppy"));
  x_case_list_add_first(list, strdup("rooster"));
  x_case_list_add_first(list, strdup("cow"));
  x_case_list_add_last(list, strdup("spots"));

  for (index = 0; index < 5; index++) {
    object = x_case_list_find_at(list, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = x_case_list_find_random(list);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  x_case_list_iterate_start(list);
  while ((object = x_case_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  x_case_list_iterate_start(list);
  while ((object = x_case_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  x_case_list_destroy(list);

  x_case_list_t *list2;
  list2 = x_case_list_create(x_core_string_compare, x_core_string_copy,
      x_core_string_destroy);
  x_case_list_add_first(list2, malloc(7));
  x_case_list_remove_first(list2);
  x_case_list_destroy(list2);

  string_to_split = malloc(sizeof("here I am"));
  strcpy(string_to_split, "here I am");

  strings = x_case_list_create_strings_from_string(string_to_split, " ");
  x_case_list_iterate_start(strings);
  while ((string = x_case_list_iterate_next(strings))) {
    printf(":%s:\n", string);
  }
  x_case_list_destroy(strings);

  free(string_to_split);

  return 0;
}
