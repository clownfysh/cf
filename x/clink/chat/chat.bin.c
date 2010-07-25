#include "x/clink/system.h"
#include "x/core/string.h"
#include "x/core/tools.h"

#define BRANCH_DENSITY 256
#define DELIMITERS " ,?!.;:-"
#define MAX_CONCEPTS 1024
#define MAX_LINKS 1024
#define MAX_OBJECTS 256

/*  static char *get_as_string(void *string_object);  */
int main(int argc, char *argv[]);
static x_core_bool_t think(x_clink_system_t *system, void *object,
    void *context);

/*
char *get_as_string(void *string_object)
{
  assert(string_object);
  char *string = string_object;
  return strdup(string);
}
*/

int main(int argc, char *argv[])
{
  x_clink_system_t *clink;
  char read_string[80];
  char *word_a;
  char *word_b;
  char *word_a_copy;
  char *word_b_copy;
  x_core_bool_t stop_requested = x_core_bool_false;
  FILE *file;

  file = fopen("/home/matthew/tsid.txt", "r");

  clink = x_clink_system_create(MAX_CONCEPTS, MAX_LINKS, x_core_string_compare,
      x_core_string_destroy, NULL);
  if (clink) {
    do {
      if (NULL == fgets(read_string, 80, file)) {
        stop_requested = x_core_bool_true;
      }
      printf("# %s\n", read_string);
      word_a = strtok(read_string, DELIMITERS);
      while ((word_b = strtok(NULL, DELIMITERS))) {
        word_a_copy = strdup(word_a);
        word_b_copy = strdup(word_b);
        x_clink_system_link(clink, word_a_copy, word_b_copy);
        /*  printf("%s->%s,", word_a_copy, word_b_copy);  */
        word_a = word_b;
      }
      /*  printf("\n");  */
      /*  x_clink_system_print(clink, get_as_string);  */
      printf("~ ");
      if (!x_clink_system_think_tree(clink, think, MAX_OBJECTS,
              BRANCH_DENSITY)) {
        x_trace("x_clink_system_think_tree");
      }
      printf("\n");

    } while (!stop_requested);
    x_clink_system_destroy(clink);
  } else {
    x_trace("x_clink_system_create");
  }

  return 0;
}

x_core_bool_t think(x_clink_system_t *system, void *object,
    void *context)
{
  assert(system);
  assert(object);
  char *s = object;

  printf("%s ", s);

  return x_core_bool_true;
}
