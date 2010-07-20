#ifndef x_psql_result_h
#define x_psql_result_h

#include "x/container/map.h"
#include "x/core/objectey.h"
#include "x/core/standard.h"
#include "x/psql/result_error.h"

struct x_psql_result_t;
typedef struct x_psql_result_t x_psql_result_t;

x_psql_result_t *x_psql_result_create(PGresult *pg_result,
    x_core_objectey_t *string_objectey, x_psql_result_create_error_t *error);

void x_psql_result_destroy(x_psql_result_t *result);

x_container_map_t *x_psql_result_get_row(x_psql_result_t *result,
    unsigned long row_index);

unsigned long x_psql_result_get_row_count(x_psql_result_t *result);

x_container_map_t *x_psql_result_iterate_next(x_psql_result_t *result);

void x_psql_result_iterate_start(x_psql_result_t *result);

#endif
