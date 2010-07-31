#include "x/config/file.h"
#include "x/case/set.h"
#include "x/core/nameobject.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/file/basic.h"

struct x_config_file_t {
  /* TODO: reimplement with x_case_map_t, like config_options? */
  x_case_set_t *strings;
  x_case_set_t *string_lists;
  x_core_objectey_t nameobject_objectey;
};

static x_core_bool_t find_as_string(x_config_file_t *file, char *name,
    char **value);

static void x_config_file_create_rollback(x_config_file_t *conf);

static x_core_bool_t line_is_a_comment(char *line);

static void parse_list_value(x_config_file_t *file, char *name, char *value);

static void parse_string_value(x_config_file_t *file, char *name, char *value);

static void read_file(x_config_file_t *file, char *filename);

static x_core_bool_t value_is_a_list(char *name);

x_core_bool_t find_as_string(x_config_file_t *file, char *name,
    char **value)
{
  assert(file);
  assert(name);
  assert(value);
  x_core_nameobject_t *nameobject_decoy;
  x_core_nameobject_t *nameobject;
  x_core_bool_t found_it;

  *value = NULL;

  nameobject_decoy = x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = x_case_set_find(file->strings, nameobject_decoy);
    x_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      found_it = x_core_bool_true;
      *value = x_core_nameobject_get_object(nameobject);
    } else {
      found_it = x_core_bool_false;
    }
  } else {
    found_it = x_core_bool_false;
    x_core_trace("x_core_nameobject_create_decoy");
  }

  return found_it;
}

x_config_file_t *x_config_file_create(char *filename)
{
  x_config_file_t *file;
  x_core_bool_t so_far_so_good;

  file = malloc(sizeof *file);
  if (file) {
    so_far_so_good = x_core_bool_true;
  } else {
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    x_core_nameobject_init_objectey(&file->nameobject_objectey);
    file->strings = x_case_set_create(&file->nameobject_objectey);
    if (!file->strings) {
      so_far_so_good = x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    file->string_lists = x_case_set_create(&file->nameobject_objectey);
    if (!file->string_lists) {
      so_far_so_good = x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (x_file_basic_exists(filename)) {
      read_file(file, filename);
    }
  }

  if (!so_far_so_good && file) {
    x_config_file_create_rollback(file);
    file = NULL;
  }

  return file;
}

void x_config_file_create_rollback(x_config_file_t *file)
{
  assert(file);

  if (file->strings) {
    x_case_set_destroy(file->strings);
  }
  if (file->string_lists) {
    x_case_set_destroy(file->string_lists);
  }
  free(file);

}

void x_config_file_destroy(x_config_file_t *file)
{
  assert(file);
  assert(file->strings);
  assert(file->string_lists);

  x_case_set_destroy(file->strings);
  x_case_set_destroy(file->string_lists);
  free(file);

}

x_core_bool_t x_config_file_find(x_config_file_t *file, char *name)
{
  assert(file);
  assert(name);
  x_core_nameobject_t *nameobject_decoy;
  x_core_bool_t found_it;

  nameobject_decoy = x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    if (x_case_set_find(file->strings, nameobject_decoy)) {
      found_it = x_core_bool_true;
    } else {
      found_it = x_core_bool_false;
    }
    x_core_nameobject_destroy_decoy(nameobject_decoy);
  } else {
    found_it = x_core_bool_false;
    x_core_trace("x_core_nameobject_create_decoy");
  }

  return found_it;
}

x_core_bool_t x_config_file_find_as_double(x_config_file_t *file, char *name,
    double *value, double default_value)
{
  assert(file);
  assert(name);
  assert(value);
  x_core_bool_t found;
  char *value_string;

  if (find_as_string(file, name, &value_string)) {
    found = x_core_bool_true;
    *value = atof(value_string);
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_file_find_as_string(x_config_file_t *file, char *name,
    char **value, char *default_value)
{
  assert(file);
  assert(name);
  assert(value);
  assert(default_value);
  x_core_bool_t found_it;

  if (find_as_string(file, name, value)) {
    found_it = x_core_bool_true;
  } else {
    found_it = x_core_bool_false;
    *value = default_value;
  }

  return found_it;
}

x_core_bool_t x_config_file_find_as_unsigned_long(x_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(file);
  assert(name);
  assert(value);
  char *value_string;
  x_core_bool_t found;

  if (find_as_string(file, name, &value_string)) {
    found = x_core_bool_true;
    *value = atol(value_string);
  } else {
    found = x_core_bool_false;
    *value = default_value;
  }

  return found;
}

x_core_bool_t x_config_file_find_as_unsigned_short(x_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(file);
  assert(name);
  assert(value);
  unsigned long value_unsigned_long;
  x_core_bool_t success;

  success = x_config_file_find_as_unsigned_long
    (file, name, &value_unsigned_long, default_value);
  if (success) {
    *value = (unsigned short) value_unsigned_long;
  }

  return success;
}

x_core_bool_t x_config_file_find_list_as_strings(x_config_file_t *file,
    char *name, x_case_list_t **list)
{
  assert(file);
  assert(name);
  x_core_nameobject_t *nameobject_decoy;
  x_core_nameobject_t *nameobject;
  x_core_bool_t success;

  *list = NULL;

  nameobject_decoy = x_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = x_case_set_find(file->string_lists, nameobject_decoy);
    x_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      success = x_core_bool_true;
      *list = x_core_nameobject_get_object(nameobject);
    } else {
      success = x_core_bool_false;
    }
  } else {
    success = x_core_bool_false;
    x_core_trace("x_core_nameobject_create_decoy");
  }

  return success;
}

x_core_bool_t line_is_a_comment(char *line)
{
  assert(line);
  x_core_bool_t is_a_comment;

  if (strlen(line) > 0) {
    if ('#' == *line) {
      is_a_comment = x_core_bool_true;
    } else {
      is_a_comment = x_core_bool_false;
    }
  } else {
    is_a_comment = x_core_bool_true;
  }

  return is_a_comment;
}

void parse_list_value(x_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  x_core_nameobject_t *nameobject;
  x_case_list_t *list;
  char *string;
  char *string_copy;
  char *strtok_context;

  list = x_case_list_create(x_core_string_compare, x_core_string_copy,
      x_core_string_destroy);
  if (list) {
    string = strtok_r(value, ",", &strtok_context);
    while (string) {
      string_copy = strdup(string);
      if (string_copy) {
        if (!x_case_list_add_last(list, string_copy)) {
          x_core_trace("x_case_list_add_last");
        }
      } else {
        x_core_trace("strdup");
      }
      string = strtok_r(NULL, ",", &strtok_context);
    }
    nameobject = x_core_nameobject_create(name, list, X_CORE_NO_COPY_FUNCTION,
        x_case_list_destroy, x_case_list_get_as_string);
    if (nameobject) {
      if (!x_case_set_add(file->string_lists, nameobject)) {
        x_core_trace("x_case_set_add");
        x_core_nameobject_destroy(nameobject);
      }
    } else {
      x_core_trace("x_core_nameobject_create");
    }
  } else {
    x_core_trace("x_case_list_create");
  }

}

void parse_string_value(x_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  x_core_nameobject_t *nameobject;

  nameobject = x_core_nameobject_create(name, value, x_core_string_copy,
      x_core_string_destroy, x_core_string_get_as_string);
  if (nameobject) {
    if (!x_case_set_add(file->strings, nameobject)) {
      x_core_trace("x_case_set_add");
    }
  } else {
    x_core_trace("x_core_nameobject_create");
  }

}

void read_file(x_config_file_t *file, char *filename)
{
  assert(file);
  assert(filename);
  x_file_basic_t *file_basic;
  x_case_list_t *lines;
  char *line;
  char *name;
  char *value;
  char *strtok_context;

  file_basic = x_file_basic_create(filename, X_FILE_MODE_OPEN_FOR_READ);
  if (file_basic) {
    lines = x_file_basic_get_as_line_list(file_basic);
    if (lines) {
      x_case_list_iterate_start(lines);
      while ((line = x_case_list_iterate_next(lines))) {
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
      x_case_list_destroy(lines);
    }
    x_file_basic_destroy(file_basic);
  } else {
    x_core_trace("x_core_file_create");
  }
}

x_core_bool_t value_is_a_list(char *name)
{
  assert(name);
  x_core_bool_t is_a_list;
  unsigned short name_size;
  unsigned short start_compare_position;

  name_size = strlen(name);

  if (name_size >= 6) {
    start_compare_position = name_size - 6;
    if (0 == strcmp("[list]", name + start_compare_position)) {
      is_a_list = x_core_bool_true;
    } else {
      is_a_list = x_core_bool_false;
    }
  } else {
    is_a_list = x_core_bool_false;
  }

  return is_a_list;
}
