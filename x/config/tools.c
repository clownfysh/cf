#include "x/config/tools.h"
#include "x/config/options.h"
#include "x/core/tools.h"

void x_config_disable_if_running_batcx_tests(int argc, char *argv[])
{
  if (x_config_running_batcx_tests(argc, argv)) {
    printf("  %s disabled in batch test mode\n", *argv);
    exit(0);
  }
}

x_core_bool_t x_config_running_batcx_tests(int argc, char *argv[])
{
  x_config_options_t *options;
  x_core_bool_t batch;
  x_core_objects_t objects;

  batch = x_core_bool_false;
  x_core_objects_init(&objects);

  options = x_config_options_create(argc, argv, &objects);
  if (options) {
    if (x_config_options_find(options, "running-batch-tests")) {
      batch = x_core_bool_true;
    }
    x_config_options_destroy(options);
  } else {
    x_core_trace("x_config_options_create");
  }

  x_core_objects_free(&objects);

  return batch;
}
