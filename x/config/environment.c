#include "x/core/tools.h"
#include "x/config/environment.h"

struct x_config_environment_t {
};

x_config_environment_t *x_config_environment_create()
{
  x_config_environment_t *environment;

  environment = malloc(sizeof *environment);
  if (environment) {

  } else {
    x_core_trace("malloc");
  }

  return environment;
}

void x_config_environment_destroy(x_config_environment_t *environment)
{
  assert(environment);
  free(environment);
}

x_core_bool_t x_config_environment_find(x_config_environment_t *environment,
    char *name)
{
  assert(environment);
  assert(name);

  return x_core_bool_false;
}

x_core_bool_t x_config_environment_find_as_double
(x_config_environment_t *environment, char *name, double *value,
    double default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return x_core_bool_false;
}

x_core_bool_t x_config_environment_find_as_string
(x_config_environment_t *environment, char *name, char **value,
    char *default_value)
{
  assert(environment);
  assert(name);
  assert(value);
  assert(default_value);

  return x_core_bool_false;
}

x_core_bool_t x_config_environment_find_as_unsigned_long
(x_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return x_core_bool_false;
}

x_core_bool_t x_config_environment_find_as_unsigned_short
(x_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return x_core_bool_false;
}

x_core_bool_t x_config_environment_find_list_as_strings
(x_config_environment_t *environment, char *name, x_case_list_t **list)
{
  assert(environment);
  assert(name);
  assert(list);

  return x_core_bool_false;
}
