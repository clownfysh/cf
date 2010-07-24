#include "x/clink/system.h"
#include "x/core/tools.h"

#define MAX_CONCEPTS 8
#define MAX_LINKS 8

static int compare(void *char_a_object, void *char_b_object);
static char *get_as_string(void *char_object);
int main(int argc, char *argv[]);
static x_core_bool_t think(x_clink_system_t *clink, void *char_object);

int compare(void *char_a_object, void *char_b_object)
{
  assert(char_a_object);
  assert(char_b_object);
  char *a = char_a_object;
  char *b = char_b_object;
  int compare_result;

  if (*a < *b) {
    compare_result = -1;
  } else if (*a > *b) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

char *get_as_string(void *char_object)
{
  assert(char_object);
  char *string;
  char *c = char_object;

  string = malloc(2);
  if (string) {
    *(string + 0) = *c;
    *(string + 1) = '\0';
  } else {
    x_trace("malloc");
  }

  return string;
}

int main(int argc, char *argv[])
{
  char *s = "It was the best of times, it was the worst of times.";
  /*  char *s = "ab";  */
  assert(strlen(s) > 1);
  x_clink_system_t *clink;
  x_clink_concept_t *concept;
  char *c;

  concept = NULL;

  clink = x_clink_system_create(MAX_CONCEPTS, MAX_LINKS, compare,
      X_CORE_NO_DESTROY_FUNCTION);
  if (clink) {
    c = s + 1;
    while (*c) {
      x_clink_system_link(clink, c - 1, c);
      printf("\n");
      x_clink_system_print(clink, get_as_string);
      c++;
    }

    printf("\nthink::\n");
    x_clink_system_think_train(clink, think, 10);
    printf("\n");

    x_clink_system_destroy(clink);
  } else {
    x_trace("x_clink_system_create");
  }

  return 0;
}

x_core_bool_t think(x_clink_system_t *clink, void *char_object)
{
  assert(clink);
  assert(char_object);
  char *c = char_object;

  printf("%c", *c);

  return x_core_bool_true;
}
