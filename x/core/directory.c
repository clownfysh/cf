#include "x/core/directory.h"
#include "x/core/tools.h"

struct x_core_directory_t {};

x_core_directory_t *x_core_directory_create()
{
  return NULL;
}

void x_core_directory_destroy(x_core_directory_t *x_core_directory)
{
}

x_core_bool_t x_core_directory_exists(char *directory_name)
{
  assert(directory_name);
  x_core_bool_t exists;
  DIR *dir;

  dir = opendir(directory_name);
  if (dir) {
    exists = x_core_bool_true;
    if (0 != closedir(dir)) {
      x_core_trace("closedir");
    }
  } else {
    exists = x_core_bool_false;
  }

  return exists;
}

x_core_bool_t x_core_directory_make(char *directory_name, mode_t mode)
{
  assert(directory_name);
  x_core_bool_t success;

  if (0 == mkdir(directory_name, mode)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}
