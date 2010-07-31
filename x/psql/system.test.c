#include "cf/x/config/tools.h"
#include "cf/x/core/tools.h"
#include "cf/x/psql/result.h"
#include "cf/x/psql/system.h"

int main(int argc, char *argv[])
{
  cf_x_config_disable_if_running_batcx_tests(argc, argv);

  cf_x_psql_system_t *cf_x_psql_system;
  cf_x_psql_system_create_error_t cf_x_psql_system_create_error;
  cf_x_psql_system_query_error_t cf_x_psql_system_query_error;
  char *cf_x_psql_system_query_error_text;
  cf_x_psql_result_t *cf_x_psql_result;
  char *sql;
  cf_x_case_map_t *row;
  cf_x_psql_system_execute_error_t cf_x_psql_system_execute_error;
  char *cf_x_psql_system_execute_error_text;
  cf_x_audit_log_t *log;
  unsigned long connection_count;

  connection_count = 1;

  log = cf_x_audit_log_create(stdout);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  cf_x_psql_system = cf_x_psql_system_create("localhost", "h", "h", "h",
      connection_count, log, &cf_x_psql_system_create_error);
  if (!cf_x_psql_system) {
    switch (cf_x_psql_system_create_error) {
      case cf_x_psql_system_create_error_connect_failed:
        cf_x_core_trace_exit("connect failed");
        break;
      case cf_x_psql_system_create_error_unknown:
      default:
        cf_x_core_trace_exit("x_psql_system_create");
        break;
    }
  }

  sql = cf_x_core_string_append(NULL, "select * from thing where id > 1");
  if (!sql) {
    cf_x_core_trace_exit("x_core_string_append");
  }
  cf_x_psql_result = cf_x_psql_system_query(cf_x_psql_system, sql,
      &cf_x_psql_system_query_error, &cf_x_psql_system_query_error_text);
  if (!cf_x_psql_result) {
    cf_x_core_trace_exit("%s", cf_x_psql_system_query_error_text);
  }

  cf_x_psql_result_iterate_start(cf_x_psql_result);
  while ((row = cf_x_psql_result_iterate_next(cf_x_psql_result))) {
    printf("id=%s name=%s\n", (char *) cf_x_case_map_find(row, "id"),
        (char *) cf_x_case_map_find(row, "name"));
  }

  cf_x_psql_result_destroy(cf_x_psql_result);
  free(sql);

  sql = cf_x_core_string_append(NULL,
      "insert into thing (id, name) values (4, 'd')");
  if (!cf_x_psql_system_execute(cf_x_psql_system, sql, &cf_x_psql_system_execute_error,
          &cf_x_psql_system_execute_error_text)) {
    cf_x_core_trace_exit("%s", cf_x_psql_system_execute_error_text);
  }
  free(sql);

  cf_x_psql_system_destroy(cf_x_psql_system);
  cf_x_audit_log_destroy(log);

  return 0;
}
