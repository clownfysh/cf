#include "cf/x/core/constants.h"
#include "cf/x/core/objectey.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/psql/system.h"

#define LOCK_CONNECTION_SLEEP_MICROSECONDS CF_X_CORE_STANDARD_SLEEP_MICROSECONDS
#define MAX_CONNECTION_COUNT 32

struct cf_x_psql_system_t {
  PGconn *connections[MAX_CONNECTION_COUNT];
  pthread_mutex_t connection_mutexes[MAX_CONNECTION_COUNT];
  unsigned short connection_count;

  cf_x_core_objectey_t string_objectey;
  char *db_host;
  char *db_name;
  cf_x_audit_log_t *log;
};

static unsigned short lock_connection(cf_x_psql_system_t *system);

static void unlock_connection(cf_x_psql_system_t *system,
    unsigned short connection_index);

unsigned short lock_connection(cf_x_psql_system_t *system)
{
  assert(system);
  unsigned short connection_index;
  cf_x_core_bool_t got_one;
  unsigned long failed_tries;

  got_one = cf_x_core_bool_false;
  failed_tries = 0;

  do {
    connection_index = random() % system->connection_count;
    if (0 == pthread_mutex_trylock
        (system->connection_mutexes + connection_index)) {
      got_one = cf_x_core_bool_true;
    } else {
      failed_tries++;
      if (failed_tries > (system->connection_count * 2)) {
        usleep(LOCK_CONNECTION_SLEEP_MICROSECONDS);
      }
    }
  } while (!got_one);

  return connection_index;
}

cf_x_psql_system_t *cf_x_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, cf_x_audit_log_t *log,
    cf_x_psql_system_create_error_t *error)
{
  assert(db_host);
  assert(db_name);
  assert(db_user);
  assert(db_password);
  assert(connection_count >= 1);
  assert(log);
  assert(error);
  cf_x_psql_system_t *system;
  ConnStatusType pq_status;
  unsigned short each_connection;
  cf_x_core_bool_t so_far_so_good;
  cf_x_core_bool_t connection_mutex_needs_destroy[MAX_CONNECTION_COUNT];
  char *connection_string;

  if (!PQisthreadsafe()) {
    cf_x_audit_log_trace(log, "hpsq", "PQisthreadsafe");
  }

  if (connection_count > MAX_CONNECTION_COUNT) {
    connection_count = MAX_CONNECTION_COUNT;
  }

  system = malloc(sizeof *system);
  if (system) {
    cf_x_core_objectey_init(&system->string_objectey, cf_x_core_string_compare,
        cf_x_core_string_copy, cf_x_core_string_destroy, CF_X_CORE_NO_EQUAL_FUNCTION,
        cf_x_core_string_get_as_string, CF_X_CORE_NO_MOD_FUNCTION);

    system->connection_count = connection_count;
    system->log = log;

    /*
      TODO: check return codes
    */
    system->db_host = strdup(db_host);
    system->db_name = strdup(db_name);

    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      *(system->connections + each_connection) = NULL;
      *(connection_mutex_needs_destroy + each_connection) = cf_x_core_bool_false;
    }

    so_far_so_good = cf_x_core_bool_true;
    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      if (0 == pthread_mutex_init
          (system->connection_mutexes + each_connection, NULL)) {
        *(connection_mutex_needs_destroy + each_connection) = cf_x_core_bool_true;
      } else {
        so_far_so_good = cf_x_core_bool_false;
        cf_x_audit_log_trace(log, "hpsq", "pthread_mutex_init");
        break;
      }
    }

  } else {
    so_far_so_good = cf_x_core_bool_false;
    *error = cf_x_psql_system_create_error_unknown;
    cf_x_audit_log_trace(log, "hpsq", "malloc");
  }

  if (so_far_so_good) {
    if (asprintf(&connection_string, "host=%s dbname=%s user=%s password=%s",
            db_host, db_name, db_user, db_password) != -1) {
      for (each_connection = 0; each_connection < connection_count;
           each_connection++) {
        *(system->connections + each_connection)
          = PQconnectdb(connection_string);
        pq_status = PQstatus(*(system->connections + each_connection));
        if (CONNECTION_OK != pq_status) {
          switch (pq_status) {
            case CONNECTION_BAD:
              *error = cf_x_psql_system_create_error_connect_failed;
              break;
            default:
              *error = cf_x_psql_system_create_error_unknown;
              break;
          }
          cf_x_audit_log_trace(log, "hpsq", "PQsetdbLogin");
          PQfinish(*(system->connections + each_connection));
          *(system->connections + each_connection) = NULL;
          free(system);
          system = NULL;
          break;
        }
      }
      free(connection_string);
    } else {
      cf_x_audit_log_trace(log, "hpsq", "asprintf");
    }
  }

  if (system && !so_far_so_good) {
    for (each_connection = 0; each_connection < connection_count;
         each_connection++) {
      if (*(connection_mutex_needs_destroy + each_connection)) {
        if (pthread_mutex_destroy
            (system->connection_mutexes + each_connection) != 0) {
          cf_x_audit_log_trace(log, "hpsq", "pthread_mutex_destroy");
        }
      }
      if (*(system->connections + each_connection)) {
        PQfinish(*(system->connections + each_connection));
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void cf_x_psql_system_destroy(cf_x_psql_system_t *system)
{
  assert(system);
  unsigned short each_connection;

  free(system->db_host);
  free(system->db_name);
  for (each_connection = 0; each_connection < system->connection_count;
       each_connection++) {
    if (pthread_mutex_destroy
        (system->connection_mutexes + each_connection) != 0) {
      cf_x_audit_log_trace(system->log, "hpsq", "pthread_mutex_destroy");
    }
    PQfinish(*(system->connections + each_connection));
  }
  free(system);
}

cf_x_core_bool_t cf_x_psql_system_execute(cf_x_psql_system_t *system, char *sql,
    cf_x_psql_system_execute_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  cf_x_core_bool_t success;
  PGresult *pg_result;
  ExecStatusType exec_status;
  unsigned short connection_index;

  success = cf_x_core_bool_false;

  connection_index = lock_connection(system);
  {
    pg_result = PQexec(*(system->connections + connection_index),
        (const char *) sql);
  }
  unlock_connection(system, connection_index);

  if (pg_result) {
    exec_status = PQresultStatus(pg_result);
    if (PGRES_COMMAND_OK == exec_status) {
      success = cf_x_core_bool_true;
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = cf_x_psql_system_execute_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = cf_x_psql_system_execute_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = cf_x_psql_system_execute_error_fatal_error;
          break;
        default:
          *error = cf_x_psql_system_execute_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      cf_x_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
    PQclear(pg_result);
  } else {
    *error = cf_x_psql_system_execute_error_unknown;
  }

  return success;
}

cf_x_psql_result_t *cf_x_psql_system_query(cf_x_psql_system_t *system, char *sql,
    cf_x_psql_system_query_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  cf_x_psql_result_t *result;
  PGresult *pg_result;
  ExecStatusType exec_status;
  cf_x_psql_result_create_error_t cf_x_psql_result_create_error;
  unsigned short connection_index;

  result = NULL;

  connection_index = lock_connection(system);
  {
    pg_result = PQexec(*(system->connections + connection_index),
        (const char *) sql);
  }
  unlock_connection(system, connection_index);

  if (pg_result) {
    exec_status = PQresultStatus(pg_result);
    if (PGRES_TUPLES_OK == exec_status) {
      result = cf_x_psql_result_create(pg_result, &system->string_objectey,
          &cf_x_psql_result_create_error);
      if (!result) {
        *error = cf_x_psql_system_query_error_unknown;
        cf_x_audit_log_trace(system->log, "hpsq", "x_psql_result_create");
      }
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = cf_x_psql_system_query_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = cf_x_psql_system_query_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = cf_x_psql_system_query_error_fatal_error;
          break;
        default:
          *error = cf_x_psql_system_query_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      cf_x_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
  } else {
    *error = cf_x_psql_system_query_error_unknown;
  }

  return result;
}

cf_x_core_bool_t cf_x_psql_system_query_count(cf_x_psql_system_t *system, char *sql,
    unsigned long *count, cf_x_psql_system_query_count_error_t *error,
    char **error_text)
{
  assert(system);
  assert(sql);
  assert(count);
  assert(error);
  assert(error_text);
  cf_x_core_bool_t success;
  cf_x_psql_result_t *result;
  cf_x_case_map_t *row;
  char *count_string;
  cf_x_psql_system_query_error_t cf_x_psql_system_query_error;

  success = cf_x_core_bool_false;

  result = cf_x_psql_system_query
    (system, sql, &cf_x_psql_system_query_error, error_text);
  if (result) {
    row = cf_x_psql_result_get_row(result, 0);
    count_string = cf_x_case_map_find(row, "c");
    if (count_string) {
      success = cf_x_core_bool_true;
      *count = atol(count_string);
    } else {
      *error = cf_x_psql_system_query_count_error_no_c_column;
    }
    cf_x_psql_result_destroy(result);
  } else {
    *error = cf_x_psql_system_query_count_error_unknown;
  }

  return success;
}

cf_x_core_bool_t cf_x_psql_system_vacuum(cf_x_psql_system_t *system,
    cf_x_psql_system_vacuum_error_t *error)
{
  assert(system);
  assert(error);
  cf_x_core_bool_t success;
  cf_x_psql_system_execute_error_t cf_x_psql_system_execute_error;
  char *cf_x_psql_system_execute_error_text;

  cf_x_audit_log_enter(system->log, "hpsq", "vacuum %s:%s", system->db_host,
      system->db_name);

  if (cf_x_psql_system_execute(system, "vacuum", &cf_x_psql_system_execute_error,
          &cf_x_psql_system_execute_error_text)) {
    success = cf_x_core_bool_true;
    cf_x_audit_log_enter(system->log, "hpsq", "vacuum %s:%s complete",
        system->db_host, system->db_name);
  } else {
    success = cf_x_core_bool_false;
    *error = cf_x_psql_system_vacuum_error_unknown;
    cf_x_audit_log_trace(system->log, "hpsq", "x_psql_system_execute");
  }

  return success;
}

void unlock_connection(cf_x_psql_system_t *system,
    unsigned short connection_index)
{
  pthread_mutex_unlock(system->connection_mutexes + connection_index);
}
