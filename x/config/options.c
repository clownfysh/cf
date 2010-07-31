#include "cf/x/config/options.h"
#include "cf/x/case/dumpster.h"
#include "cf/x/case/map.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"

struct cf_x_config_options_t {
  cf_x_case_map_t *options;
  cf_x_case_dumpster_t *dumpster;
};

static void parse_options(cf_x_config_options_t *options, int argc, char *argv[]);

cf_x_config_options_t *cf_x_config_options_create(int argc, char *argv[],
    cf_x_core_objects_t *objects)
{
  cf_x_config_options_t *options;
  cf_x_core_bool_t so_far_so_good;

  options = malloc(sizeof *options);
  if (options) {
    options->dumpster = NULL;
    options->options = cf_x_case_map_create(&objects->string_objectey,
        &objects->string_objectey, CF_X_CASE_MAP_DONT_DESTROY);
    if (options->options) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      cf_x_core_trace("x_case_map_create");
      so_far_so_good = cf_x_core_bool_false;
    }
  } else {
    cf_x_core_trace("malloc");
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    options->dumpster = cf_x_case_dumpster_create(&objects->string_objectey);
    if (!options->dumpster) {
      cf_x_core_trace("x_case_dumpster_create");
    }
  }

  if (so_far_so_good) {
    parse_options(options, argc, argv);
  }

  if (!so_far_so_good && options) {
    cf_x_case_map_destroy(options->options);
    if (options->dumpster) {
      cf_x_case_dumpster_destroy(options->dumpster);
    }
    free(options);
    options = NULL;
  }

  return options;
}

void cf_x_config_options_destroy(cf_x_config_options_t *options)
{
  assert(options);

  cf_x_case_map_destroy(options->options);
  cf_x_case_dumpster_destroy(options->dumpster);
  free(options);
}

cf_x_core_bool_t cf_x_config_options_find(cf_x_config_options_t *options, char *name)
{
  assert(options);
  assert(name);
  cf_x_core_bool_t found;

  if (cf_x_case_map_find(options->options, name)) {
    found = cf_x_core_bool_true;
  } else {
    found = cf_x_core_bool_false;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_options_find_as_double(cf_x_config_options_t *options,
    char *name, double *value, double default_value)
{
  assert(options);
  assert(name);
  assert(value);
  cf_x_core_bool_t found;
  char *value_string;

  value_string = cf_x_case_map_find(options->options, name);
  if (value_string) {
    found = cf_x_core_bool_true;
    *value = atof(value_string);
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_options_find_as_string(cf_x_config_options_t *options,
    char *name, char **value, char *default_value)
{
  assert(options);
  assert(name);
  assert(value);
  assert(default_value);
  cf_x_core_bool_t found;

  *value = cf_x_case_map_find(options->options, name);
  if (*value) {
    found = cf_x_core_bool_true;
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_options_find_as_unsigned_short
(cf_x_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value)
{
  assert(options);
  assert(name);
  assert(value);
  cf_x_core_bool_t found;
  char *value_string;

  value_string = cf_x_case_map_find(options->options, name);
  if (value_string) {
    found = cf_x_core_bool_true;
    *value = atoi(value_string);
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_options_find_as_unsigned_long
(cf_x_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value)
{
  assert(options);
  assert(name);
  assert(value);
  cf_x_core_bool_t found;
  char *value_string;

  value_string = cf_x_case_map_find(options->options, name);
  if (value_string) {
    found = cf_x_core_bool_true;
    *value = atol(value_string);
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_options_find_list_as_strings
(cf_x_config_options_t *options, char *name, cf_x_case_list_t **list)
{
  cf_x_core_trace_exit("TODO: implement");
  return cf_x_core_bool_false;
}

void parse_options(cf_x_config_options_t *options, int argc, char *argv[])
{
  assert(options);
  int each_word;
  char *substring;
  char *name;
  char *value;

  for (each_word = 1; (each_word + 1) < argc; each_word += 2) {
    name = *(argv + each_word);
    substring = cf_x_core_string_substring(name, 0, 2);
    if (substring) {
      if (0 == strcmp("--", substring)) {
        name = strdup(name + 2);
        if (name) {
          if (!cf_x_case_dumpster_add(options->dumpster, name)) {
            cf_x_core_trace("x_case_dumpster_add");
          }
          value = strdup(*(argv + each_word + 1));
          if (!cf_x_case_dumpster_add(options->dumpster, value)) {
            cf_x_core_trace("x_case_dumpster_add");
          }
          if (!cf_x_case_map_add(options->options, name, value)) {
            cf_x_core_trace("x_case_map_add");
            free(name);
          }
        } else {
          cf_x_core_trace("strdup");
        }
      }
      free(substring);
    } else {
      cf_x_core_trace("x_core_string_substring");
    }
  }
}
