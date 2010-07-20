#include "i/object.h"
#include "i/tools.h"

void i_print(void *object, i_get_as_string_f get_object_as_string)
{
  assert(object);
  assert(get_object_as_string);
  char *object_string;

  object_string = get_object_as_string(object);
  if (object_string) {
    printf("%s\n", object_string);
    free(object_string);
  } else {
    i_trace("get_as_string");
  }
}
