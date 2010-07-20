#include "x/config/options.h"
#include "x/core/tools.h"

#define DEFAULT_MODE "modex"
#define DEFAULT_OPTION "optionx"

int main(int argc, char *argv[])
{
  x_core_objects_t objects;
  x_config_options_t *options;
  char *value;

  x_core_objects_init(&objects);

  options = x_config_options_create(argc, argv, &objects);
  if (!options) {
    x_core_trace_exit("x_config_options_create");
  }

  if (x_config_options_find_as_string(options, "mode", &value, DEFAULT_MODE)) {
    printf(":%s:\n", value);
  } else {
  }

  if (x_config_options_find_as_string
      (options, "option", &value, DEFAULT_OPTION)) {
    printf(":%s:\n", value);
  } else {
  }

  x_config_options_destroy(options);
  x_core_objects_free(&objects);

  return 0;
}
