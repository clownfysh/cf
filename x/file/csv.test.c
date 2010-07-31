#include "cf/x/core/tools.h"
#include "cf/x/file/csv.h"

int main(int argc, char *argv[])
{
  cf_x_disable_test();

  cf_x_file_csv_t *csv;
  unsigned long object_count;
  unsigned long field_count;
  unsigned long each_object;
  unsigned long each_field;
  char *value;

  csv = cf_x_file_csv_create_extended("cf/x/file/csv_test.csv", 1, 2);
  if (!csv) {
    cf_x_core_trace_exit("x_file_csv_create");
  }

  object_count = cf_x_file_csv_get_object_count(csv);
  field_count = cf_x_file_csv_get_field_count(csv);

  for (each_field = 0; each_field < field_count; each_field++) {
    for (each_object = 0; each_object < object_count; each_object++) {
      value = cf_x_file_csv_get_value_by_index_as_string(csv, each_object,
          each_field);
      printf("%s,", value);
    }
    printf("\n");
  }

  cf_x_file_csv_destroy(csv);

  return 0;
}
