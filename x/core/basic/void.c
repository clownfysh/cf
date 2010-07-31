#include "cf/x/core/basic/void.h"
#include "cf/x/core/tools.h"

int cf_x_core_basic_void_compare(void *object_a, void *object_b)
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

void *cf_x_core_basic_void_copy(void *object)
{
  return object;
}

void cf_x_core_basic_void_destroy(void *object)
{
  free(object);
}

char *cf_x_core_basic_void_get_as_string(void *object)
{
  cf_x_core_trace_exit("TODO: implement/test");

  assert(object);
  char *string;

  string = malloc(10 + 1);
  if (string) {
    snprintf(string, 10 + 1, "%p", object);
  } else {
    cf_x_core_trace_exit("malloc");
  }

  return string;
}

void cf_x_core_basic_void_init_objectey(cf_x_core_objectey_t *objectey)
{
  assert(objectey);

  objectey->get_as_string = cf_x_core_basic_void_get_as_string;
  objectey->compare = cf_x_core_basic_void_compare;
  objectey->copy = cf_x_core_basic_void_copy;
  objectey->destroy = cf_x_core_basic_void_destroy;
}
