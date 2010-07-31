#include "cf/x/case/array.h"
#include "cf/x/case/map.h"
#include "cf/x/core/tools.h"
#include "cf/x/psql/result.h"

struct cf_x_psql_result_t {
  cf_x_case_array_t *rows;
  PGresult *pg_result;
};

cf_x_psql_result_t *cf_x_psql_result_create(PGresult *pg_result,
    cf_x_core_objectey_t *string_objectey, cf_x_psql_result_create_error_t *error)
{
  assert(pg_result);
  cf_x_psql_result_t *result;
  int row_count;
  int field_count;
  int each_row;
  int each_column;
  char *field_name;
  char *field_value;
  cf_x_case_map_t *map;
  cf_x_core_bool_t so_far_so_good;

  result = malloc(sizeof *result);
  if (result) {
    result->pg_result = pg_result;
    row_count = PQntuples(pg_result);
    result->rows = cf_x_case_array_create(row_count, cf_x_case_map_compare,
        cf_x_case_map_copy, cf_x_case_map_destroy);
    if (result->rows) {
      field_count = PQnfields(pg_result);
      so_far_so_good = cf_x_core_bool_true;
      for (each_row = 0; each_row < row_count; each_row++) {
        map = cf_x_case_map_create(string_objectey, string_objectey,
            CF_X_CASE_MAP_DONT_DESTROY);
        if (map) {
          for (each_column = 0; each_column < field_count; each_column++) {
            field_name = PQfname(pg_result, each_column);
            field_value = PQgetvalue(pg_result, each_row, each_column);
            if (cf_x_case_map_add(map, field_name, field_value)) {
              cf_x_case_array_add(result->rows, each_row, map);
            } else {
              *error = cf_x_psql_result_create_error_x_case_map_add_failed;
              so_far_so_good = cf_x_core_bool_false;
              cf_x_case_map_destroy(map);
              break;
            }
          }
        } else {
          *error = cf_x_psql_result_create_error_x_case_map_create_failed;
          so_far_so_good = cf_x_core_bool_false;
          break;
        }
      }
      if (!so_far_so_good) {
        free(result);
        result = NULL;
      }
    } else {
      *error = cf_x_psql_result_create_error_x_case_array_create_failed;
    }
  } else {
    *error = cf_x_psql_result_create_error_malloc_failed;
  }

  return result;
}

void cf_x_psql_result_destroy(cf_x_psql_result_t *result)
{
  cf_x_case_array_destroy(result->rows);
  PQclear(result->pg_result);
  free(result);
}

cf_x_case_map_t *cf_x_psql_result_get_row(cf_x_psql_result_t *result,
    unsigned long row_index)
{
  return cf_x_case_array_find(result->rows, row_index);
}

unsigned long cf_x_psql_result_get_row_count(cf_x_psql_result_t *result)
{
  return cf_x_case_array_get_size(result->rows);
}

cf_x_case_map_t *cf_x_psql_result_iterate_next(cf_x_psql_result_t *result)
{
  return cf_x_case_array_iterate_next(result->rows);
}

void cf_x_psql_result_iterate_start(cf_x_psql_result_t *result)
{
  cf_x_case_array_iterate_start(result->rows);
}
