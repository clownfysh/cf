#ifndef x_psql_system_h
#define x_psql_system_h

#include "x/audit/log.h"
#include "x/psql/result.h"
#include "x/psql/system_error.h"

struct x_psql_system_t;
typedef struct x_psql_system_t x_psql_system_t;

x_psql_system_t *x_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, x_audit_log_t *log,
    x_psql_system_create_error_t *error);

void x_psql_system_destroy(x_psql_system_t *system);

x_core_bool_t x_psql_system_execute(x_psql_system_t *system, char *sql,
    x_psql_system_execute_error_t *error, char **error_text);

x_psql_result_t *x_psql_system_query(x_psql_system_t *system,
    char *sql, x_psql_system_query_error_t *error, char **error_text);

x_core_bool_t x_psql_system_query_count(x_psql_system_t *system,
    char *sql, unsigned long *count, x_psql_system_query_count_error_t *error,
    char **error_text);

x_core_bool_t x_psql_system_vacuum(x_psql_system_t *system,
    x_psql_system_vacuum_error_t *error);

#endif
