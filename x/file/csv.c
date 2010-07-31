#include "cf/x/case/array.h"
#include "cf/x/case/list.h"
#include "cf/x/case/map.h"
#include "cf/x/core/objects.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/file/basic.h"
#include "cf/x/file/csv.h"

struct cf_x_file_csv_t {
  unsigned long object_count;
  unsigned long field_count;

  cf_x_case_map_t *name_to_index;
  cf_x_case_array_t *index_to_name;
  cf_x_case_array_t *objects;
};

static cf_x_case_array_t *create_index_to_name_array(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file);

static cf_x_case_map_t *create_name_to_index_map(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file);

static cf_x_case_array_t *create_objects_array(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file, unsigned long start_object,
    unsigned long end_object);

static char *get_value_by_index_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

static char *get_value_by_name_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

cf_x_file_csv_t *cf_x_file_csv_create(char *filename)
{
  return cf_x_file_csv_create_extended(filename, 0, 0);
}

cf_x_file_csv_t *cf_x_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object)
{
  assert(filename);
  cf_x_file_csv_t *csv;
  cf_x_core_bool_t so_far_so_good;
  cf_x_file_basic_t *file;

  file = NULL;

  csv = malloc(sizeof *csv);
  if (csv) {
    csv->name_to_index = NULL;
    csv->index_to_name = NULL;
    csv->objects = NULL;
    cf_x_core_objects_init();
    file = cf_x_file_basic_create(filename, CF_X_FILE_MODE_OPEN_FOR_READ);
    if (file) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      cf_x_core_trace("x_file_basic_create");
      so_far_so_good = cf_x_core_bool_false;
    }
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    csv->name_to_index = create_name_to_index_map(csv, file);
    if (!csv->name_to_index) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("create_name_to_index_map");
    }
  }

  if (so_far_so_good) {
    csv->index_to_name = create_index_to_name_array(csv, file);
    if (!csv->index_to_name) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("create_index_to_name_array");
    }
  }

  if (so_far_so_good) {
    csv->objects = create_objects_array(csv, file, start_object, end_object);
    if (!csv->objects) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("create_objects_array");
    }
  }

  if (!so_far_so_good && csv) {
    if (csv->name_to_index) {
      cf_x_case_map_destroy(csv->name_to_index);
    }
    if (csv->index_to_name) {
      cf_x_case_array_destroy(csv->index_to_name);
    }
    if (csv->objects) {
      cf_x_case_array_destroy(csv->objects);
    }
  }

  if (file) {
    cf_x_file_basic_destroy(file);
  }

  return csv;
}

void cf_x_file_csv_destroy(void *csv_object)
{
  assert(csv_object);
  cf_x_file_csv_t *csv;

  csv = csv_object;

  cf_x_case_map_destroy(csv->name_to_index);
  cf_x_case_array_destroy(csv->index_to_name);
  cf_x_case_array_destroy(csv->objects);
  free(csv);
}

cf_x_case_array_t *cf_x_file_csv_get_field_by_index_as_array
(cf_x_file_csv_t *csv, unsigned long field_index)
{
  assert(csv);
  cf_x_case_array_t *array;
  unsigned long each_object;
  char *value;
  char *value_copy;

  array = cf_x_case_array_create(csv->object_count, cf_x_core_string_compare,
      cf_x_core_string_copy, cf_x_core_string_destroy);
  if (array) {
    for (each_object = 0; each_object < csv->object_count; each_object++) {
      value = get_value_by_index_as_string(csv, each_object, field_index);
      value_copy = strdup(value);
      if (value_copy) {
        cf_x_case_array_add(array, each_object, value_copy);
      } else {
        cf_x_core_trace("strdup");
        cf_x_case_array_destroy(array);
        array = NULL;
        break;
      }
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return array;
}

cf_x_case_array_t *cf_x_file_csv_get_field_by_name_as_array
(cf_x_file_csv_t *csv, char *field_name)
{
  assert(csv);
  assert(field_name);
  cf_x_case_array_t *array;
  unsigned long each_object;
  char *value;
  char *value_copy;

  array = cf_x_case_array_create(csv->object_count, cf_x_core_string_compare,
      cf_x_core_string_copy, cf_x_core_string_destroy);
  if (array) {
    for (each_object = 0; each_object < csv->object_count; each_object++) {
      value = get_value_by_name_as_string(csv, each_object, field_name);
      value_copy = strdup(value);
      if (value_copy) {
        cf_x_case_array_add(array, each_object, value_copy);
      } else {
        cf_x_core_trace("strdup");
        cf_x_case_array_destroy(array);
        array = NULL;
        break;
      }
    }
  } else {
    cf_x_core_trace("x_case_array_create");
  }

  return array;
}

unsigned long cf_x_file_csv_get_field_count(cf_x_file_csv_t *csv)
{
  return csv->field_count;
}

cf_x_core_bool_t cf_x_file_csv_get_field_index(cf_x_file_csv_t *csv,
    char *field_name, unsigned long *field_index)
{
  assert(csv);
  unsigned long *field_index_object;
  cf_x_core_bool_t success;

  field_index_object = cf_x_case_map_find(csv->name_to_index, field_name);
  if (field_index_object) {
    success = cf_x_core_bool_true;
    *field_index = *((unsigned long *) field_index_object);
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

char *cf_x_file_csv_get_field_name(cf_x_file_csv_t *csv,
    unsigned long field_index)
{
  return cf_x_case_array_find(csv->index_to_name, field_index);
}

unsigned long cf_x_file_csv_get_object_count(cf_x_file_csv_t *csv)
{
  return csv->object_count;
}

cf_x_core_bit_t cf_x_file_csv_get_value_by_index_as_bit(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  char value_bit;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    if ('1' == *value_string) {
      value_bit = 1;
    } else {
      value_bit = 0;
    }
  } else {
    value_bit = 0;
  }

  return value_bit;
}

cf_x_core_bool_t cf_x_file_csv_get_value_by_index_as_bool(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  cf_x_core_bool_t value_bool;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    if ('1' == *value_string) {
      value_bool = cf_x_core_bool_true;
    } else {
      value_bool = cf_x_core_bool_false;
    }
  } else {
    value_bool = cf_x_core_bool_false;
  }

  return value_bool;
}

char cf_x_file_csv_get_value_by_index_as_char(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  char value_char;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    value_char = *value_string;
  } else {
    value_char = '\0';
  }

  return value_char;
}

double cf_x_file_csv_get_value_by_index_as_double(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  double value_double;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_double = atof(value_string);

  return value_double;
}

long cf_x_file_csv_get_value_by_index_as_long(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  long value_long;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_long = atol(value_string);

  return value_long;
}

short cf_x_file_csv_get_value_by_index_as_short(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  short value_short;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_short = atoi(value_string);

  return value_short;
}

char *cf_x_file_csv_get_value_by_index_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  return get_value_by_index_as_string(csv, object_index, field_index);
}

unsigned char cf_x_file_csv_get_value_by_index_as_unsigned_char
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned char value_unsigned_char;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_char = atoi(value_string);

  return value_unsigned_char;
}

unsigned long cf_x_file_csv_get_value_by_index_as_unsigned_long
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned long value_unsigned_long;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_long = atol(value_string);

  return value_unsigned_long;
}

unsigned short cf_x_file_csv_get_value_by_index_as_unsigned_short
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned short value_unsigned_short;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_short = atoi(value_string);

  return value_unsigned_short;
}

char cf_x_file_csv_get_value_by_name_as_char(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  char value_char;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  if (strlen(value_string) > 0) {
    value_char = *value_string;
  } else {
    value_char = '\0';
  }

  return value_char;
}

double cf_x_file_csv_get_value_by_name_as_double(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  double value_double;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_double = atof(value_string);

  return value_double;
}

long cf_x_file_csv_get_value_by_name_as_long(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  long value_long;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_long = atol(value_string);

  return value_long;
}

short cf_x_file_csv_get_value_by_name_as_short(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_short;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_short = atoi(value_string);

  return value_short;
}

char *cf_x_file_csv_get_value_by_name_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  return get_value_by_name_as_string(csv, object_index, field_name);
}

unsigned long cf_x_file_csv_get_value_by_name_as_unsigned_long
(cf_x_file_csv_t *csv, unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_unsigned_long;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_unsigned_long = atol(value_string);

  return value_unsigned_long;
}

unsigned short cf_x_file_csv_get_value_by_name_as_unsigned_short
(cf_x_file_csv_t *csv, unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_unsigned_short;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_unsigned_short = atoi(value_string);

  return value_unsigned_short;
}

cf_x_case_array_t *create_index_to_name_array(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file)
{
  assert(csv);
  assert(file);
  cf_x_case_array_t *index_to_name;
  char *first_line;
  cf_x_case_list_t *line_list;
  cf_x_case_list_t *field_names;
  char *field_name;
  unsigned long field_index;

  index_to_name = cf_x_case_array_create(csv->field_count,
      cf_x_core_string_compare, cf_x_core_string_copy,
      cf_x_core_string_destroy);
  if (index_to_name) {
    line_list = cf_x_file_basic_get_as_line_list(file);
    if (line_list) {
      first_line = cf_x_case_list_find_first(line_list);
      if (first_line) {
        field_names = cf_x_case_list_create_strings_from_string
          (first_line, ",");
        if (field_names) {
          field_index = 0;
          cf_x_case_list_iterate_start(field_names);
          while ((field_name = cf_x_case_list_iterate_next(field_names))) {
            cf_x_case_array_add(index_to_name, field_index, field_name);
            field_index++;
          }
          cf_x_case_list_dont_destroy_objects(field_names);
          cf_x_case_list_destroy(field_names);
        } else {
          cf_x_core_trace("x_case_list_create_strings_from_string");
        }
      } else {
        cf_x_core_trace_exit("");
      }
      cf_x_case_list_destroy(line_list);
    } else {
      cf_x_core_trace("x_file_basic_get_as_line_list");
    }
  } else {
    cf_x_core_trace("x_case_array_add");
  }

  return index_to_name;
}

cf_x_case_map_t *create_name_to_index_map(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file)
{
  assert(csv);
  assert(file);
  cf_x_case_list_t *line_list;
  cf_x_case_map_t *map;
  cf_x_case_list_t *field_names;
  char *first_line;
  char *name_object;
  unsigned long index;
  unsigned long *index_object;
  char *name;

  map = NULL;

  line_list = cf_x_file_basic_get_as_line_list(file);
  if (line_list) {
    first_line = cf_x_case_list_find_first(line_list);
    if (first_line) {
      field_names = cf_x_case_list_create_strings_from_string
        (first_line, ",");
      if (field_names) {
        csv->field_count = cf_x_case_list_get_size(field_names);
        map = cf_x_case_map_create(&cf_x_core_objects.string_objectey,
            &cf_x_core_objects.unsigned_long_objectey, CF_X_CASE_MAP_DESTROY);
        if (map) {
          index = 0;
          cf_x_case_list_iterate_start(field_names);
          while ((name = cf_x_case_list_iterate_next(field_names))) {
            name_object = cf_x_core_string_copy(name);
            if (name_object) {
              index_object = malloc(sizeof *index_object);
              if (index_object) {
                *index_object = index;
                if (!cf_x_case_map_add(map, name_object, index_object)) {
                  cf_x_core_trace("x_case_map_add");
                }
              } else {
                cf_x_core_trace("malloc");
              }
            } else {
              cf_x_core_trace("x_core_string_copy");
            }
            index++;
          }
        } else {
          cf_x_core_trace("x_case_map_create");
        }
        cf_x_case_list_destroy(field_names);
      } else {
        cf_x_core_trace("x_case_list_create_strings_from_string");
      }
    } else {
      cf_x_core_trace("x_case_list_find_first");
    }
    cf_x_case_list_destroy(line_list);
  } else {
    cf_x_core_trace("x_file_basic_get_as_line_list");
  }

  return map;
}

cf_x_case_array_t *create_objects_array(cf_x_file_csv_t *csv,
    cf_x_file_basic_t *file, unsigned long start_object,
    unsigned long end_object)
{
  assert(csv);
  assert(file);
  cf_x_case_array_t *objects_array;
  cf_x_case_list_t *lines;
  cf_x_case_array_t *values;
  char *line;
  unsigned long total_object_index;
  unsigned long relative_object_index;
  unsigned long total_object_count;

  objects_array = NULL;

  lines = cf_x_file_basic_get_as_line_list(file);
  if (lines) {

    total_object_count = cf_x_case_list_get_size(lines) - 1;
    if ((0 == start_object) && (0 == end_object)) {
      end_object = total_object_count - 1;
    }
    csv->object_count = (end_object - start_object) + 1;

    objects_array = cf_x_case_array_create(csv->object_count,
        cf_x_case_array_compare, cf_x_case_array_copy,
        cf_x_case_array_destroy);
    if (objects_array) {
      cf_x_case_list_iterate_start(lines);
      cf_x_case_list_iterate_next(lines);
      total_object_index = 0;
      relative_object_index = 0;
      while ((line = cf_x_case_list_iterate_next(lines))) {
        if ((total_object_index >= start_object)
            && (total_object_index <= end_object)) {
          values = cf_x_case_array_create_strings_from_string(line, ",");
          if (values) {
            cf_x_case_array_add(objects_array, relative_object_index,
                values);
          } else {
            cf_x_core_trace("x_case_array_create_strings_from_string");
            cf_x_case_array_destroy(values);
          }
          relative_object_index++;
        }
        total_object_index++;
      }
    } else {
      cf_x_core_trace("x_case_array_create");
    }
    cf_x_case_list_destroy(lines);
  } else {
    cf_x_core_trace("x_file_basic_get_as_line_list");
  }

  return objects_array;
}

char *get_value_by_index_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value;
  cf_x_case_array_t *object;

  object = cf_x_case_array_find(csv->objects, object_index);
  value = cf_x_case_array_find(object, field_index);

  return value;
}

char *get_value_by_name_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  unsigned long field_index;
  unsigned long *field_index_object;
  char *value;

  field_index_object = cf_x_case_map_find(csv->name_to_index, field_name);
  if (field_index_object) {
    field_index = *((unsigned long *) field_index_object);
    value = get_value_by_index_as_string(csv, object_index, field_index);
  } else {
    value = NULL;
    cf_x_core_trace("x_case_map_find");
  }

  return value;
}
