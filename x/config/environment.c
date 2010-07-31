#include "cf/x/core/tools.h"
#include "cf/x/config/environment.h"

struct cf_x_config_environment_t {
};

cf_x_config_environment_t *cf_x_config_environment_create()
{
  cf_x_config_environment_t *environment;

  environment = malloc(sizeof *environment);
  if (environment) {

  } else {
    cf_x_core_trace("malloc");
  }

  return environment;
}

void cf_x_config_environment_destroy(cf_x_config_environment_t *environment)
{
  assert(environment);
  free(environment);
}

cf_x_core_bool_t cf_x_config_environment_find(cf_x_config_environment_t *environment,
    char *name)
{
  assert(environment);
  assert(name);

  return cf_x_core_bool_false;
}

cf_x_core_bool_t cf_x_config_environment_find_as_double
(cf_x_config_environment_t *environment, char *name, double *value,
    double default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return cf_x_core_bool_false;
}

cf_x_core_bool_t cf_x_config_environment_find_as_string
(cf_x_config_environment_t *environment, char *name, char **value,
    char *default_value)
{
  assert(environment);
  assert(name);
  assert(value);
  assert(default_value);

  return cf_x_core_bool_false;
}

cf_x_core_bool_t cf_x_config_environment_find_as_unsigned_long
(cf_x_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return cf_x_core_bool_false;
}

cf_x_core_bool_t cf_x_config_environment_find_as_unsigned_short
(cf_x_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(environment);
  assert(name);
  assert(value);

  return cf_x_core_bool_false;
}

cf_x_core_bool_t cf_x_config_environment_find_list_as_strings
(cf_x_config_environment_t *environment, char *name, cf_x_case_list_t **list)
{
  assert(environment);
  assert(name);
  assert(list);

  return cf_x_core_bool_false;
}
