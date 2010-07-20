#ifndef x_psql_system_error_h
#define x_psql_system_error_h

enum x_psql_system_create_error_t {
  x_psql_system_create_error_unknown,
  x_psql_system_create_error_connect_failed,
};
typedef enum x_psql_system_create_error_t x_psql_system_create_error_t;

enum x_psql_system_execute_error_t {
  x_psql_system_execute_error_unknown,
  x_psql_system_execute_error_bad_response,
  x_psql_system_execute_error_empty_query,
  x_psql_system_execute_error_fatal_error,
};
typedef enum x_psql_system_execute_error_t
x_psql_system_execute_error_t;

enum x_psql_system_query_count_error_t {
  x_psql_system_query_count_error_unknown,
  x_psql_system_query_count_error_no_c_column,
};
typedef enum x_psql_system_query_count_error_t
x_psql_system_query_count_error_t;

enum x_psql_system_query_error_t {
  x_psql_system_query_error_unknown,
  x_psql_system_query_error_bad_response,
  x_psql_system_query_error_empty_query,
  x_psql_system_query_error_fatal_error,
};
typedef enum x_psql_system_query_error_t x_psql_system_query_error_t;

enum x_psql_system_vacuum_error_t {
  x_psql_system_vacuum_error_unknown,
};
typedef enum x_psql_system_vacuum_error_t x_psql_system_vacuum_error_t;

#endif
