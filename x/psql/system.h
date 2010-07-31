#ifndef cf_x_psql_system_h
#define cf_x_psql_system_h

#include "cf/x/audit/log.h"
#include "cf/x/psql/result.h"
#include "cf/x/psql/system_error.h"

struct cf_x_psql_system_t;
typedef struct cf_x_psql_system_t cf_x_psql_system_t;

cf_x_psql_system_t *cf_x_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, cf_x_audit_log_t *log,
    cf_x_psql_system_create_error_t *error);

void cf_x_psql_system_destroy(cf_x_psql_system_t *system);

cf_x_core_bool_t cf_x_psql_system_execute(cf_x_psql_system_t *system, char *sql,
    cf_x_psql_system_execute_error_t *error, char **error_text);

cf_x_psql_result_t *cf_x_psql_system_query(cf_x_psql_system_t *system,
    char *sql, cf_x_psql_system_query_error_t *error, char **error_text);

cf_x_core_bool_t cf_x_psql_system_query_count(cf_x_psql_system_t *system,
    char *sql, unsigned long *count, cf_x_psql_system_query_count_error_t *error,
    char **error_text);

cf_x_core_bool_t cf_x_psql_system_vacuum(cf_x_psql_system_t *system,
    cf_x_psql_system_vacuum_error_t *error);

#endif
