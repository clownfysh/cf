#ifndef cf_x_psql_result_h
#define cf_x_psql_result_h

#include "cf/x/case/map.h"
#include "cf/x/core/objectey.h"
#include "cf/x/core/standard.h"
#include "cf/x/psql/result_error.h"

struct cf_x_psql_result_t;
typedef struct cf_x_psql_result_t cf_x_psql_result_t;

cf_x_psql_result_t *cf_x_psql_result_create(PGresult *pg_result,
    cf_x_core_objectey_t *string_objectey, cf_x_psql_result_create_error_t *error);

void cf_x_psql_result_destroy(cf_x_psql_result_t *result);

cf_x_case_map_t *cf_x_psql_result_get_row(cf_x_psql_result_t *result,
    unsigned long row_index);

unsigned long cf_x_psql_result_get_row_count(cf_x_psql_result_t *result);

cf_x_case_map_t *cf_x_psql_result_iterate_next(cf_x_psql_result_t *result);

void cf_x_psql_result_iterate_start(cf_x_psql_result_t *result);

#endif
