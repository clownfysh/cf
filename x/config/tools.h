#ifndef cf_x_config_tools_h
#define cf_x_config_tools_h

#include "cf/x/core/bool.h"

void cf_x_config_disable_if_running_batch_tests(int argc, char *argv[]);

cf_x_core_bool_t cf_x_config_running_batch_tests(int argc, char *argv[]);

#endif
