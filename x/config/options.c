#include "x/config/options.h"
#include "x/container/dumpster.h"
#include "x/container/map.h"
#include "x/core/string.h"
#include "x/core/tools.h"

struct x_config_options_t {
  x_container_map_t *options;
  x_container_dumpster_t *dumpster;
};

static void parse_options(x_config_options_t *options, int argc, char *argv[]);

x_config_options_t *x_config_options_create(int argc, char *argv[],
    x_core_objects_t *objects)
{
  x_config_options_t *options;
  x_core_bool_t so_far_so_good;

  options = malloc(sizeof *options);
  if (options) {
    options->dumpster = NULL;
    options->options = x_container_map_create(&objects->string_objectey,
        &objects->string_objectey, X_CONTAINER_MAP_DONT_DESTROY);
    if (options->options) {
      so_far_so_good = x_core_bool_true;
    } else {
      x_core_trace("x_container_map_create");
      so_far_so_good = x_core_bool_false;
    }
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    options->dumpster = x_container_dumpster_create(&objects->string_objectey);
    if (!options->dumpster) {
      x_core_trace("x_container_dumpster_create");
    }
  }

  if (so_far_so_good) {
    parse_options(options, argc, argv);
  }

  if (!so_far_so_good && options) {
    x_container_map_destroy(options->options);
    if (options->dumpster) {
      x_container_dumpster_destroy(options->dumpster);
    }
    free(options);
    options = NULL;
  }

  return options;
}

void x_config_options_destroy(x_config_options_t *options)
{
  assert(options);

  x_container_map_destroy(options->options);
  x_container_dumpster_destroy(options->dumpster);
  free(options);
}

x_core_bool_t x_config_options_find(x_config_options_t *options, char *name)
{
  assert(options);
  assert(name);
  x_core_bool_t found;

  if (x_container_map_find(options->options, name)) {
    found = x_core_bool_true;
  } else {
    found = x_core_bool_false;
  }

  return found;
}

x_core_bool_t x_config_options_find_as_double(x_config_options_t *options,
    char *name, double *value, double default_value)
{
  assert(options);
  assert(name);
  assert(value);
  x_core_bool_t found;
  char *value_string;

  value_string = x_container_map_find(options->options, name);
  if (value_string) {
    found = x_core_bool_true;
    *value = atof(value_string);
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_options_find_as_string(x_config_options_t *options,
    char *name, char **value, char *default_value)
{
  assert(options);
  assert(name);
  assert(value);
  assert(default_value);
  x_core_bool_t found;

  *value = x_container_map_find(options->options, name);
  if (*value) {
    found = x_core_bool_true;
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_options_find_as_unsigned_short
(x_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(options);
  assert(name);
  assert(value);
  x_core_bool_t found;
  char *value_string;

  value_string = x_container_map_find(options->options, name);
  if (value_string) {
    found = x_core_bool_true;
    *value = atoi(value_string);
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_options_find_as_unsigned_long
(x_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(options);
  assert(name);
  assert(value);
  x_core_bool_t found;
  char *value_string;

  value_string = x_container_map_find(options->options, name);
  if (value_string) {
    found = x_core_bool_true;
    *value = atol(value_string);
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_options_find_list_as_strings
(x_config_options_t *options, char *name, x_container_list_t **list)
{
  x_core_trace_exit("TODO: implement");
  return x_core_bool_false;
}

void parse_options(x_config_options_t *options, int argc, char *argv[])
{
  assert(options);
  int eacx_word;
  char *substring;
  char *name;
  char *value;

  for (eacx_word = 1; (eacx_word + 1) < argc; eacx_word += 2) {
    name = *(argv + eacx_word);
    substring = x_core_string_substring(name, 0, 2);
    if (substring) {
      if (0 == strcmp("--", substring)) {
        name = strdup(name + 2);
        if (name) {
          if (!x_container_dumpster_add(options->dumpster, name)) {
            x_core_trace("x_container_dumpster_add");
          }
          value = strdup(*(argv + eacx_word + 1));
          if (!x_container_dumpster_add(options->dumpster, value)) {
            x_core_trace("x_container_dumpster_add");
          }
          if (!x_container_map_add(options->options, name, value)) {
            x_core_trace("x_container_map_add");
            free(name);
          }
        } else {
          x_core_trace("strdup");
        }
      }
      free(substring);
    } else {
      x_core_trace("x_core_string_substring");
    }
  }
}
