#ifndef x_config_tools_h
#define x_config_tools_h

#include "x/core/bool.h"

void x_config_disable_if_running_batcx_tests(int argc, char *argv[]);

x_core_bool_t x_config_running_batcx_tests(int argc, char *argv[]);

#endif
