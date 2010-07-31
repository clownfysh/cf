#include "cf/x/core/tools.h"
#include "cf/x/file/basic.h"
#include "cf/x/file/tools.h"

cf_x_core_bool_t cf_x_file_count_lines_in_file(char *filename,
    unsigned long *line_count)
{
  assert(filename);
  assert(line_count);
  cf_x_core_bool_t success;
  cf_x_file_basic_t *file;

  file = cf_x_file_basic_create(filename, CF_X_FILE_MODE_OPEN_FOR_READ);
  if (file) {
    success = cf_x_file_basic_get_line_count(file, line_count);
    cf_x_file_basic_destroy(file);
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_file_basic_create");
  }

  return success;
}
