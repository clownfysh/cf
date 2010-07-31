#include "cf/x/config/environment.h"
#include "cf/x/config/file.h"
#include "cf/x/config/options.h"
#include "cf/x/config/system.h"
#include "cf/x/core/tools.h"

struct cf_x_config_system_t {
  cf_x_config_file_t *file;
  cf_x_config_environment_t *environment;
  cf_x_config_options_t *options;
};

cf_x_config_system_t *cf_x_config_system_create(int argc, char *argv[],
    char *filename, cf_x_core_objects_t *objects)
{
  assert(argv);
  assert(objects);
  cf_x_config_system_t *system;
  cf_x_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    system->environment = NULL;
    system->options = NULL;
    if (filename) {
      system->file = cf_x_config_file_create(filename);
      if (system->file) {
        so_far_so_good = cf_x_core_bool_true;
      } else {
        so_far_so_good = cf_x_core_bool_false;
        cf_x_core_trace("x_config_file_create");
      }
    } else {
      so_far_so_good = cf_x_core_bool_true;
      system->file = NULL;
    }
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->environment = cf_x_config_environment_create();
    if (!system->environment) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_config_environment_create");
    }
  }

  if (so_far_so_good) {
    system->options = cf_x_config_options_create(argc, argv, objects);
    if (!system->options) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_config_options_create");
    }
  }

  if (!so_far_so_good && system) {
    if (system->file) {
      cf_x_config_file_destroy(system->file);
    }
    if (system->environment) {
      cf_x_config_environment_destroy(system->environment);
    }
    if (system->options) {
      cf_x_config_options_destroy(system->options);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void cf_x_config_system_destroy(cf_x_config_system_t *system)
{
  assert(system);

  if (system->file) {
    cf_x_config_file_destroy(system->file);
  }
  cf_x_config_environment_destroy(system->environment);
  cf_x_config_options_destroy(system->options);
  free(system);
}

cf_x_core_bool_t cf_x_config_system_find(cf_x_config_system_t *system, char *name)
{
  assert(system);
  assert(name);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find(system->options, name)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find(system->environment, name)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file && cf_x_config_file_find(system->file, name)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_system_find_as_double(cf_x_config_system_t *system,
    char *name, double *value, double default_value)
{
  assert(system);
  assert(name);
  assert(value);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find_as_double
      (system->options, name, value, default_value)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find_as_double
        (system->environment, name, value, default_value)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file && cf_x_config_file_find_as_double
          (system->file, name, value, default_value)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_system_find_as_string(cf_x_config_system_t *system,
    char *name, char **value, char *default_value)
{
  assert(system);
  assert(name);
  assert(value);
  assert(default_value);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find_as_string
      (system->options, name, value, default_value)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find_as_string
        (system->environment, name, value, default_value)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file && cf_x_config_file_find_as_string
          (system->file, name, value, default_value)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_system_find_as_unsigned_long(cf_x_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(system);
  assert(name);
  assert(value);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find_as_unsigned_long
      (system->options, name, value, default_value)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find_as_unsigned_long
        (system->environment, name, value, default_value)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file && cf_x_config_file_find_as_unsigned_long
          (system->file, name, value, default_value)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_system_find_as_unsigned_short(cf_x_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(system);
  assert(name);
  assert(value);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find_as_unsigned_short
      (system->options, name, value, default_value)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find_as_unsigned_short
        (system->environment, name, value, default_value)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file && cf_x_config_file_find_as_unsigned_short
          (system->file, name, value, default_value)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_system_find_list_as_strings(cf_x_config_system_t *system,
    char *name, cf_x_case_list_t **list)
{
  assert(system);
  assert(name);
  cf_x_core_bool_t found_it;

  found_it = cf_x_core_bool_false;

  if (cf_x_config_options_find_list_as_strings(system->options, name, list)) {
    found_it = cf_x_core_bool_true;
  } else {
    if (cf_x_config_environment_find_list_as_strings
        (system->environment, name, list)) {
      found_it = cf_x_core_bool_true;
    } else {
      if (system->file
          && cf_x_config_file_find_list_as_strings(system->file, name, list)) {
        found_it = cf_x_core_bool_true;
      }
    }
  }

  return found_it;
}
