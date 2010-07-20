#include "x/core/tools.h"
#include "x/file/basic.h"
#include "x/file/tools.h"

x_core_bool_t x_file_count_lines_in_file(char *filename,
    unsigned long *line_count)
{
  assert(filename);
  assert(line_count);
  x_core_bool_t success;
  x_file_basic_t *file;

  file = x_file_basic_create(filename, X_FILE_MODE_OPEN_FOR_READ);
  if (file) {
    success = x_file_basic_get_line_count(file, line_count);
    x_file_basic_destroy(file);
  } else {
    success = x_core_bool_false;
    x_core_trace("x_file_basic_create");
  }

  return success;
}
