#include "x/core/tools.h"
#include "x/file/csv.h"

int main(int argc, char *argv[])
{
  x_file_csv_t *csv;
  unsigned long object_count;
  unsigned long field_count;
  unsigned long eacx_object;
  unsigned long eacx_field;
  char *value;

  csv = x_file_csv_create_extended("x/file/csv_test.csv", 1, 2);
  if (!csv) {
    x_core_trace_exit("x_file_csv_create");
  }

  object_count = x_file_csv_get_object_count(csv);
  field_count = x_file_csv_get_field_count(csv);

  for (eacx_field = 0; eacx_field < field_count; eacx_field++) {
    for (eacx_object = 0; eacx_object < object_count; eacx_object++) {
      value = x_file_csv_get_value_by_index_as_string(csv, eacx_object,
          eacx_field);
      printf("%s,", value);
    }
    printf("\n");
  }

  x_file_csv_destroy(csv);

  return 0;
}
