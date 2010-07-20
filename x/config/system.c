#include "x/config/environment.h"
#include "x/config/file.h"
#include "x/config/options.h"
#include "x/config/system.h"
#include "x/core/tools.h"

struct x_config_system_t {
  x_config_file_t *file;
  x_config_environment_t *environment;
  x_config_options_t *options;
};

x_config_system_t *x_config_system_create(int argc, char *argv[],
    char *filename, x_core_objects_t *objects)
{
  assert(argv);
  assert(objects);
  x_config_system_t *system;
  x_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    system->environment = NULL;
    system->options = NULL;
    if (filename) {
      system->file = x_config_file_create(filename);
      if (system->file) {
        so_far_so_good = x_core_bool_true;
      } else {
        so_far_so_good = x_core_bool_false;
        x_core_trace("x_config_file_create");
      }
    } else {
      so_far_so_good = x_core_bool_true;
      system->file = NULL;
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    system->environment = x_config_environment_create();
    if (!system->environment) {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_config_environment_create");
    }
  }

  if (so_far_so_good) {
    system->options = x_config_options_create(argc, argv, objects);
    if (!system->options) {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_config_options_create");
    }
  }

  if (!so_far_so_good && system) {
    if (system->file) {
      x_config_file_destroy(system->file);
    }
    if (system->environment) {
      x_config_environment_destroy(system->environment);
    }
    if (system->options) {
      x_config_options_destroy(system->options);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void x_config_system_destroy(x_config_system_t *system)
{
  assert(system);

  if (system->file) {
    x_config_file_destroy(system->file);
  }
  x_config_environment_destroy(system->environment);
  x_config_options_destroy(system->options);
  free(system);
}

x_core_bool_t x_config_system_find(x_config_system_t *system, char *name)
{
  assert(system);
  assert(name);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find(system->options, name)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find(system->environment, name)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file && x_config_file_find(system->file, name)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}

x_core_bool_t x_config_system_find_as_double(x_config_system_t *system,
    char *name, double *value, double default_value)
{
  assert(system);
  assert(name);
  assert(value);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find_as_double
      (system->options, name, value, default_value)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find_as_double
        (system->environment, name, value, default_value)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file && x_config_file_find_as_double
          (system->file, name, value, default_value)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}

x_core_bool_t x_config_system_find_as_string(x_config_system_t *system,
    char *name, char **value, char *default_value)
{
  assert(system);
  assert(name);
  assert(value);
  assert(default_value);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find_as_string
      (system->options, name, value, default_value)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find_as_string
        (system->environment, name, value, default_value)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file && x_config_file_find_as_string
          (system->file, name, value, default_value)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}

x_core_bool_t x_config_system_find_as_unsigned_long(x_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(system);
  assert(name);
  assert(value);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find_as_unsigned_long
      (system->options, name, value, default_value)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find_as_unsigned_long
        (system->environment, name, value, default_value)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file && x_config_file_find_as_unsigned_long
          (system->file, name, value, default_value)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}

x_core_bool_t x_config_system_find_as_unsigned_short(x_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(system);
  assert(name);
  assert(value);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find_as_unsigned_short
      (system->options, name, value, default_value)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find_as_unsigned_short
        (system->environment, name, value, default_value)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file && x_config_file_find_as_unsigned_short
          (system->file, name, value, default_value)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}

x_core_bool_t x_config_system_find_list_as_strings(x_config_system_t *system,
    char *name, x_container_list_t **list)
{
  assert(system);
  assert(name);
  x_core_bool_t found_it;

  found_it = x_core_bool_false;

  if (x_config_options_find_list_as_strings(system->options, name, list)) {
    found_it = x_core_bool_true;
  } else {
    if (x_config_environment_find_list_as_strings
        (system->environment, name, list)) {
      found_it = x_core_bool_true;
    } else {
      if (system->file
          && x_config_file_find_list_as_strings(system->file, name, list)) {
        found_it = x_core_bool_true;
      }
    }
  }

  return found_it;
}
