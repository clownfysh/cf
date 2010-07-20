#include "x/core/object.h"
#include "x/core/tools.h"

void x_core_print(void *object, x_core_get_as_string_f get_object_as_string)
{
  assert(object);
  assert(get_object_as_string);
  char *object_string;

  object_string = get_object_as_string(object);
  if (object_string) {
    printf("%s\n", object_string);
    free(object_string);
  } else {
    x_core_trace("get_as_string");
  }
}
