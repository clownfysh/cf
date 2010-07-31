#include "cf/x/config/options.h"
#include "cf/x/core/tools.h"

#define DEFAULT_MODE "modex"
#define DEFAULT_OPTION "optionx"

int main(int argc, char *argv[])
{
  cf_x_config_options_t *options;
  char *value;

  cf_x_core_objects_init();

  options = cf_x_config_options_create(argc, argv, &cf_x_core_objects);
  if (!options) {
    cf_x_core_trace_exit("x_config_options_create");
  }

  if (cf_x_config_options_find_as_string(options, "mode", &value, DEFAULT_MODE)) {
    printf(":%s:\n", value);
  } else {
  }

  if (cf_x_config_options_find_as_string
      (options, "option", &value, DEFAULT_OPTION)) {
    printf(":%s:\n", value);
  } else {
  }

  cf_x_config_options_destroy(options);

  return 0;
}
