#include "cf/x/config/tools.h"
#include "cf/x/config/options.h"
#include "cf/x/core/tools.h"

void cf_x_config_disable_if_running_batch_tests(int argc, char *argv[])
{
  if (cf_x_config_running_batch_tests(argc, argv)) {
    printf("  %s disabled in batch test mode\n", *argv);
    exit(0);
  }
}

cf_x_core_bool_t cf_x_config_running_batch_tests(int argc, char *argv[])
{
  cf_x_config_options_t *options;
  cf_x_core_bool_t batch;
  cf_x_core_objects_t objects;

  batch = cf_x_core_bool_false;
  cf_x_core_objects_init(&objects);

  options = cf_x_config_options_create(argc, argv, &objects);
  if (options) {
    if (cf_x_config_options_find(options, "running-batch-tests")) {
      batch = cf_x_core_bool_true;
    }
    cf_x_config_options_destroy(options);
  } else {
    cf_x_core_trace("x_config_options_create");
  }

  cf_x_core_objects_free(&objects);

  return batch;
}
