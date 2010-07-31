#include "cf/x/config/file.h"
#include "cf/x/case/set.h"
#include "cf/x/core/nameobject.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/file/basic.h"

struct cf_x_config_file_t {
  /* TODO: reimplement with cf_x_case_map_t, like config_options? */
  cf_x_case_set_t *strings;
  cf_x_case_set_t *string_lists;
  cf_x_core_objectey_t nameobject_objectey;
};

static cf_x_core_bool_t find_as_string(cf_x_config_file_t *file, char *name,
    char **value);

static void cf_x_config_file_create_rollback(cf_x_config_file_t *conf);

static cf_x_core_bool_t line_is_a_comment(char *line);

static void parse_list_value(cf_x_config_file_t *file, char *name, char *value);

static void parse_string_value(cf_x_config_file_t *file, char *name, char *value);

static void read_file(cf_x_config_file_t *file, char *filename);

static cf_x_core_bool_t value_is_a_list(char *name);

cf_x_core_bool_t find_as_string(cf_x_config_file_t *file, char *name,
    char **value)
{
  assert(file);
  assert(name);
  assert(value);
  cf_x_core_nameobject_t *nameobject_decoy;
  cf_x_core_nameobject_t *nameobject;
  cf_x_core_bool_t found_it;

  *value = NULL;

  nameobject_decoy = cf_x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = cf_x_case_set_find(file->strings, nameobject_decoy);
    cf_x_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      found_it = cf_x_core_bool_true;
      *value = cf_x_core_nameobject_get_object(nameobject);
    } else {
      found_it = cf_x_core_bool_false;
    }
  } else {
    found_it = cf_x_core_bool_false;
    cf_x_core_trace("x_core_nameobject_create_decoy");
  }

  return found_it;
}

cf_x_config_file_t *cf_x_config_file_create(char *filename)
{
  cf_x_config_file_t *file;
  cf_x_core_bool_t so_far_so_good;

  file = malloc(sizeof *file);
  if (file) {
    so_far_so_good = cf_x_core_bool_true;
  } else {
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    cf_x_core_nameobject_init_objectey(&file->nameobject_objectey);
    file->strings = cf_x_case_set_create(&file->nameobject_objectey);
    if (!file->strings) {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    file->string_lists = cf_x_case_set_create(&file->nameobject_objectey);
    if (!file->string_lists) {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (cf_x_file_basic_exists(filename)) {
      read_file(file, filename);
    }
  }

  if (!so_far_so_good && file) {
    cf_x_config_file_create_rollback(file);
    file = NULL;
  }

  return file;
}

void cf_x_config_file_create_rollback(cf_x_config_file_t *file)
{
  assert(file);

  if (file->strings) {
    cf_x_case_set_destroy(file->strings);
  }
  if (file->string_lists) {
    cf_x_case_set_destroy(file->string_lists);
  }
  free(file);

}

void cf_x_config_file_destroy(cf_x_config_file_t *file)
{
  assert(file);
  assert(file->strings);
  assert(file->string_lists);

  cf_x_case_set_destroy(file->strings);
  cf_x_case_set_destroy(file->string_lists);
  free(file);

}

cf_x_core_bool_t cf_x_config_file_find(cf_x_config_file_t *file, char *name)
{
  assert(file);
  assert(name);
  cf_x_core_nameobject_t *nameobject_decoy;
  cf_x_core_bool_t found_it;

  nameobject_decoy = cf_x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    if (cf_x_case_set_find(file->strings, nameobject_decoy)) {
      found_it = cf_x_core_bool_true;
    } else {
      found_it = cf_x_core_bool_false;
    }
    cf_x_core_nameobject_destroy_decoy(nameobject_decoy);
  } else {
    found_it = cf_x_core_bool_false;
    cf_x_core_trace("x_core_nameobject_create_decoy");
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_file_find_as_double(cf_x_config_file_t *file, char *name,
    double *value, double default_value)
{
  assert(file);
  assert(name);
  assert(value);
  cf_x_core_bool_t found;
  char *value_string;

  if (find_as_string(file, name, &value_string)) {
    found = cf_x_core_bool_true;
    *value = atof(value_string);
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_file_find_as_string(cf_x_config_file_t *file, char *name,
    char **value, char *default_value)
{
  assert(file);
  assert(name);
  assert(value);
  assert(default_value);
  cf_x_core_bool_t found_it;

  if (find_as_string(file, name, value)) {
    found_it = cf_x_core_bool_true;
  } else {
    found_it = cf_x_core_bool_false;
    *value = default_value;
  }

  return found_it;
}

cf_x_core_bool_t cf_x_config_file_find_as_unsigned_long(cf_x_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(file);
  assert(name);
  assert(value);
  char *value_string;
  cf_x_core_bool_t found;

  if (find_as_string(file, name, &value_string)) {
    found = cf_x_core_bool_true;
    *value = atol(value_string);
  } else {
    found = cf_x_core_bool_false;
    *value = default_value;
  }

  return found;
}

cf_x_core_bool_t cf_x_config_file_find_as_unsigned_short(cf_x_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(file);
  assert(name);
  assert(value);
  unsigned long value_unsigned_long;
  cf_x_core_bool_t success;

  success = cf_x_config_file_find_as_unsigned_long
    (file, name, &value_unsigned_long, default_value);
  if (success) {
    *value = (unsigned short) value_unsigned_long;
  }

  return success;
}

cf_x_core_bool_t cf_x_config_file_find_list_as_strings(cf_x_config_file_t *file,
    char *name, cf_x_case_list_t **list)
{
  assert(file);
  assert(name);
  cf_x_core_nameobject_t *nameobject_decoy;
  cf_x_core_nameobject_t *nameobject;
  cf_x_core_bool_t success;

  *list = NULL;

  nameobject_decoy = cf_x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = cf_x_case_set_find(file->string_lists, nameobject_decoy);
    cf_x_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      success = cf_x_core_bool_true;
      *list = cf_x_core_nameobject_get_object(nameobject);
    } else {
      success = cf_x_core_bool_false;
    }
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("x_core_nameobject_create_decoy");
  }

  return success;
}

cf_x_core_bool_t line_is_a_comment(char *line)
{
  assert(line);
  cf_x_core_bool_t is_a_comment;

  if (strlen(line) > 0) {
    if ('#' == *line) {
      is_a_comment = cf_x_core_bool_true;
    } else {
      is_a_comment = cf_x_core_bool_false;
    }
  } else {
    is_a_comment = cf_x_core_bool_true;
  }

  return is_a_comment;
}

void parse_list_value(cf_x_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  cf_x_core_nameobject_t *nameobject;
  cf_x_case_list_t *list;
  char *string;
  char *string_copy;
  char *strtok_context;

  list = cf_x_case_list_create(cf_x_core_string_compare, cf_x_core_string_copy,
      cf_x_core_string_destroy);
  if (list) {
    string = strtok_r(value, ",", &strtok_context);
    while (string) {
      string_copy = strdup(string);
      if (string_copy) {
        if (!cf_x_case_list_add_last(list, string_copy)) {
          cf_x_core_trace("x_case_list_add_last");
        }
      } else {
        cf_x_core_trace("strdup");
      }
      string = strtok_r(NULL, ",", &strtok_context);
    }
    nameobject = cf_x_core_nameobject_create(name, list, CF_X_CORE_NO_COPY_FUNCTION,
        cf_x_case_list_destroy, cf_x_case_list_get_as_string);
    if (nameobject) {
      if (!cf_x_case_set_add(file->string_lists, nameobject)) {
        cf_x_core_trace("x_case_set_add");
        cf_x_core_nameobject_destroy(nameobject);
      }
    } else {
      cf_x_core_trace("x_core_nameobject_create");
    }
  } else {
    cf_x_core_trace("x_case_list_create");
  }

}

void parse_string_value(cf_x_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  cf_x_core_nameobject_t *nameobject;

  nameobject = cf_x_core_nameobject_create(name, value, cf_x_core_string_copy,
      cf_x_core_string_destroy, cf_x_core_string_get_as_string);
  if (nameobject) {
    if (!cf_x_case_set_add(file->strings, nameobject)) {
      cf_x_core_trace("x_case_set_add");
    }
  } else {
    cf_x_core_trace("x_core_nameobject_create");
  }

}

void read_file(cf_x_config_file_t *file, char *filename)
{
  assert(file);
  assert(filename);
  cf_x_file_basic_t *file_basic;
  cf_x_case_list_t *lines;
  char *line;
  char *name;
  char *value;
  char *strtok_context;

  file_basic = cf_x_file_basic_create(filename, CF_X_FILE_MODE_OPEN_FOR_READ);
  if (file_basic) {
    lines = cf_x_file_basic_get_as_line_list(file_basic);
    if (lines) {
      cf_x_case_list_iterate_start(lines);
      while ((line = cf_x_case_list_iterate_next(lines))) {
        if (!line_is_a_comment(line)) {
          name = strtok_r(line, ":", &strtok_context);
          if (name) {
            value = strtok_r(NULL, "\n", &strtok_context);
            if (!value) {
              value = "";
            }
            if (value_is_a_list(name)) {
              parse_list_value(file, name, value);
            } else {
              parse_string_value(file, name, value);
            }
          }
        }
      }
      cf_x_case_list_destroy(lines);
    }
    cf_x_file_basic_destroy(file_basic);
  } else {
    cf_x_core_trace("x_core_file_create");
  }
}

cf_x_core_bool_t value_is_a_list(char *name)
{
  assert(name);
  cf_x_core_bool_t is_a_list;
  unsigned short name_size;
  unsigned short start_compare_position;

  name_size = strlen(name);

  if (name_size >= 6) {
    start_compare_position = name_size - 6;
    if (0 == strcmp("[list]", name + start_compare_position)) {
      is_a_list = cf_x_core_bool_true;
    } else {
      is_a_list = cf_x_core_bool_false;
    }
  } else {
    is_a_list = cf_x_core_bool_false;
  }

  return is_a_list;
}
