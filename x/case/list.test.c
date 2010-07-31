#include "cf/x/core/standard.h"
#include "cf/x/case/list.h"
#include "cf/x/core/string.h"

int main(int argc, char *argv[])
{
  cf_x_case_list_t *list;
  void *object;
  char *object_string;
  unsigned long index;
  unsigned long iteration;
  cf_x_case_list_t *strings;
  char *string;
  char *string_to_split;

  list = cf_x_case_list_create(cf_x_core_string_compare, cf_x_core_string_copy,
      cf_x_core_string_destroy);

  cf_x_case_list_add_first(list, strdup("puppy"));
  cf_x_case_list_add_first(list, strdup("rooster"));
  cf_x_case_list_add_first(list, strdup("cow"));
  cf_x_case_list_add_last(list, strdup("spots"));

  for (index = 0; index < 5; index++) {
    object = cf_x_case_list_find_at(list, index);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  for (iteration = 0; iteration < 16; iteration++) {
    object = cf_x_case_list_find_random(list);
    if (object) {
      object_string = (char *) object;
      printf("%s\n", object_string);
    }
  }

  printf("\n");

  cf_x_case_list_iterate_start(list);
  while ((object = cf_x_case_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  printf("\n");

  cf_x_case_list_iterate_start(list);
  while ((object = cf_x_case_list_iterate_next(list))) {
    object_string = (char *) object;
    printf("%s\n", object_string);
  }

  cf_x_case_list_destroy(list);

  cf_x_case_list_t *list2;
  list2 = cf_x_case_list_create(cf_x_core_string_compare, cf_x_core_string_copy,
      cf_x_core_string_destroy);
  cf_x_case_list_add_first(list2, malloc(7));
  cf_x_case_list_remove_first(list2);
  cf_x_case_list_destroy(list2);

  string_to_split = malloc(sizeof("here I am"));
  strcpy(string_to_split, "here I am");

  strings = cf_x_case_list_create_strings_from_string(string_to_split, " ");
  cf_x_case_list_iterate_start(strings);
  while ((string = cf_x_case_list_iterate_next(strings))) {
    printf(":%s:\n", string);
  }
  cf_x_case_list_destroy(strings);

  free(string_to_split);

  return 0;
}
