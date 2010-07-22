#include "x/core/tools.h"
#include "x/core/void.h"

int x_core_void_compare(void *object_a, void *object_b)
{
  int compare;

  if (object_a > object_b) {
    compare = 1;
  } else if (object_a < object_b) {
    compare = -1;
  } else {
    compare = 0;
  }

  return compare;
}

void *x_core_void_copy(void *object)
{
  return object;
}

void x_core_void_destroy(void *object)
{
  free(object);
}

char *x_core_void_get_as_string(void *object)
{
  x_core_trace_exit("TODO: implement/test");

  assert(object);
  char *string;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%p", object);
  } else {
    x_core_trace_exit("malloc");
  }

  return string;
}

void x_core_void_init_objectey(x_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = x_core_void_get_as_string;
  objectey->compare = x_core_void_compare;
  objectey->copy = x_core_void_copy;
  objectey->destroy = x_core_void_destroy;
}