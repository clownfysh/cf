#include "x/container/array.h"
#include "x/container/map.h"
#include "x/core/tools.h"
#include "x/psql/result.h"

struct x_psql_result_t {
  x_container_array_t *rows;
  PGresult *pg_result;
};

x_psql_result_t *x_psql_result_create(PGresult *pg_result,
    x_core_objectey_t *string_objectey, x_psql_result_create_error_t *error)
{
  assert(pg_result);
  x_psql_result_t *result;
  int row_count;
  int field_count;
  int eacx_row;
  int eacx_column;
  char *field_name;
  char *field_value;
  x_container_map_t *map;
  x_core_bool_t so_far_so_good;

  result = malloc(sizeof *result);
  if (result) {
    result->pg_result = pg_result;
    row_count = PQntuples(pg_result);
    result->rows = x_container_array_create(row_count, x_container_map_compare,
        x_container_map_copy, x_container_map_destroy);
    if (result->rows) {
      field_count = PQnfields(pg_result);
      so_far_so_good = x_core_bool_true;
      for (eacx_row = 0; eacx_row < row_count; eacx_row++) {
        map = x_container_map_create(string_objectey, string_objectey,
            X_CONTAINER_MAP_DONT_DESTROY);
        if (map) {
          for (eacx_column = 0; eacx_column < field_count; eacx_column++) {
            field_name = PQfname(pg_result, eacx_column);
            field_value = PQgetvalue(pg_result, eacx_row, eacx_column);
            if (x_container_map_add(map, field_name, field_value)) {
              x_container_array_add(result->rows, eacx_row, map);
            } else {
              *error = x_psql_result_create_error_x_container_map_add_failed;
              so_far_so_good = x_core_bool_false;
              x_container_map_destroy(map);
              break;
            }
          }
        } else {
          *error = x_psql_result_create_error_x_container_map_create_failed;
          so_far_so_good = x_core_bool_false;
          break;
        }
      }
      if (!so_far_so_good) {
        free(result);
        result = NULL;
      }
    } else {
      *error = x_psql_result_create_error_x_container_array_create_failed;
    }
  } else {
    *error = x_psql_result_create_error_malloc_failed;
  }

  return result;
}

void x_psql_result_destroy(x_psql_result_t *result)
{
  x_container_array_destroy(result->rows);
  PQclear(result->pg_result);
  free(result);
}

x_container_map_t *x_psql_result_get_row(x_psql_result_t *result,
    unsigned long row_index)
{
  return x_container_array_find(result->rows, row_index);
}

unsigned long x_psql_result_get_row_count(x_psql_result_t *result)
{
  return x_container_array_get_size(result->rows);
}

x_container_map_t *x_psql_result_iterate_next(x_psql_result_t *result)
{
  return x_container_array_iterate_next(result->rows);
}

void x_psql_result_iterate_start(x_psql_result_t *result)
{
  x_container_array_iterate_start(result->rows);
}
