#include "cf/x/core/directory.h"
#include "cf/x/core/tools.h"

struct cf_x_core_directory_t {};

cf_x_core_directory_t *cf_x_core_directory_create()
{
  return NULL;
}

void cf_x_core_directory_destroy(cf_x_core_directory_t *cf_x_core_directory)
{
}

cf_x_core_bool_t cf_x_core_directory_exists(char *directory_name)
{
  assert(directory_name);
  cf_x_core_bool_t exists;
  DIR *dir;

  dir = opendir(directory_name);
  if (dir) {
    exists = cf_x_core_bool_true;
    if (0 != closedir(dir)) {
      cf_x_core_trace("closedir");
    }
  } else {
    exists = cf_x_core_bool_false;
  }

  return exists;
}

cf_x_core_bool_t cf_x_core_directory_make(char *directory_name, mode_t mode)
{
  assert(directory_name);
  cf_x_core_bool_t success;

  if (0 == mkdir(directory_name, mode)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}
