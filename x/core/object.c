#include "cf/x/core/object.h"
#include "cf/x/core/tools.h"

void cf_x_core_object_print(void *object,
    cf_x_core_object_get_as_string_f get_object_as_string)
{
  assert(object);
  assert(get_object_as_string);
  char *object_string;

  object_string = get_object_as_string(object);
  if (object_string) {
    printf("%s\n", object_string);
    free(object_string);
  } else {
    cf_x_core_trace("get_as_string");
  }
}
