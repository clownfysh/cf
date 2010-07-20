#include "x/config/tools.h"
#include "x/core/tools.h"
#include "x/psql/result.h"
#include "x/psql/system.h"

int main(int argc, char *argv[])
{
  x_config_disable_if_running_batcx_tests(argc, argv);

  x_psql_system_t *x_psql_system;
  x_psql_system_create_error_t x_psql_system_create_error;
  x_psql_system_query_error_t x_psql_system_query_error;
  char *x_psql_system_query_error_text;
  x_psql_result_t *x_psql_result;
  char *sql;
  x_container_map_t *row;
  x_psql_system_execute_error_t x_psql_system_execute_error;
  char *x_psql_system_execute_error_text;
  x_audit_log_t *log;
  unsigned long connection_count;

  connection_count = 1;

  log = x_audit_log_create(stdout);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  x_psql_system = x_psql_system_create("localhost", "h", "h", "h",
      connection_count, log, &x_psql_system_create_error);
  if (!x_psql_system) {
    switch (x_psql_system_create_error) {
      case x_psql_system_create_error_connect_failed:
        x_core_trace_exit("connect failed");
        break;
      case x_psql_system_create_error_unknown:
      default:
        x_core_trace_exit("x_psql_system_create");
        break;
    }
  }

  sql = x_core_string_append(NULL, "select * from thing where id > 1");
  if (!sql) {
    x_core_trace_exit("x_core_string_append");
  }
  x_psql_result = x_psql_system_query(x_psql_system, sql,
      &x_psql_system_query_error, &x_psql_system_query_error_text);
  if (!x_psql_result) {
    x_core_trace_exit("%s", x_psql_system_query_error_text);
  }

  x_psql_result_iterate_start(x_psql_result);
  while ((row = x_psql_result_iterate_next(x_psql_result))) {
    printf("id=%s name=%s\n", (char *) x_container_map_find(row, "id"),
        (char *) x_container_map_find(row, "name"));
  }

  x_psql_result_destroy(x_psql_result);
  free(sql);

  sql = x_core_string_append(NULL,
      "insert into thing (id, name) values (4, 'd')");
  if (!x_psql_system_execute(x_psql_system, sql, &x_psql_system_execute_error,
          &x_psql_system_execute_error_text)) {
    x_core_trace_exit("%s", x_psql_system_execute_error_text);
  }
  free(sql);

  x_psql_system_destroy(x_psql_system);
  x_audit_log_destroy(log);

  return 0;
}
