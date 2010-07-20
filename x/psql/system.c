#include "x/core/constants.h"
#include "x/core/objectey.h"
#include "x/core/string.h"
#include "x/core/tools.h"
#include "x/psql/system.h"

#define LOCK_CONNECTION_SLEEP_MICROSECONDS X_CORE_STANDARD_SLEEP_MICROSECONDS
#define MAX_CONNECTION_COUNT 32

struct x_psql_system_t {
  PGconn *connections[MAX_CONNECTION_COUNT];
  pthread_mutex_t connection_mutexes[MAX_CONNECTION_COUNT];
  unsigned short connection_count;

  x_core_objectey_t string_objectey;
  char *db_host;
  char *db_name;
  x_audit_log_t *log;
};

static unsigned short lock_connection(x_psql_system_t *system);

static void unlock_connection(x_psql_system_t *system,
    unsigned short connection_index);

unsigned short lock_connection(x_psql_system_t *system)
{
  assert(system);
  unsigned short connection_index;
  x_core_bool_t got_one;
  unsigned long failed_tries;

  got_one = x_core_bool_false;
  failed_tries = 0;

  do {
    connection_index = random() % system->connection_count;
    if (0 == pthread_mutex_trylock
        (system->connection_mutexes + connection_index)) {
      got_one = x_core_bool_true;
    } else {
      failed_tries++;
      if (failed_tries > (system->connection_count * 2)) {
        usleep(LOCK_CONNECTION_SLEEP_MICROSECONDS);
      }
    }
  } while (!got_one);

  return connection_index;
}

x_psql_system_t *x_psql_system_create(const char *db_host, const char *db_name,
    const char *db_user, const char *db_password,
    unsigned short connection_count, x_audit_log_t *log,
    x_psql_system_create_error_t *error)
{
  assert(db_host);
  assert(db_name);
  assert(db_user);
  assert(db_password);
  assert(connection_count >= 1);
  assert(log);
  assert(error);
  x_psql_system_t *system;
  ConnStatusType pq_status;
  unsigned short eacx_connection;
  x_core_bool_t so_far_so_good;
  x_core_bool_t connection_mutex_needs_destroy[MAX_CONNECTION_COUNT];
  char *connection_string;

  if (!PQisthreadsafe()) {
    x_audit_log_trace(log, "hpsq", "PQisthreadsafe");
  }

  if (connection_count > MAX_CONNECTION_COUNT) {
    connection_count = MAX_CONNECTION_COUNT;
  }

  system = malloc(sizeof *system);
  if (system) {
    x_core_objectey_init(&system->string_objectey, x_core_string_compare,
        x_core_string_copy, x_core_string_destroy,
        x_core_string_get_as_string);

    system->connection_count = connection_count;
    system->log = log;

    /*
      TODO: check return codes
    */
    system->db_host = strdup(db_host);
    system->db_name = strdup(db_name);

    for (eacx_connection = 0; eacx_connection < connection_count;
         eacx_connection++) {
      *(system->connections + eacx_connection) = NULL;
      *(connection_mutex_needs_destroy + eacx_connection) = x_core_bool_false;
    }

    so_far_so_good = x_core_bool_true;
    for (eacx_connection = 0; eacx_connection < connection_count;
         eacx_connection++) {
      if (0 == pthread_mutex_init
          (system->connection_mutexes + eacx_connection, NULL)) {
        *(connection_mutex_needs_destroy + eacx_connection) = x_core_bool_true;
      } else {
        so_far_so_good = x_core_bool_false;
        x_audit_log_trace(log, "hpsq", "pthread_mutex_init");
        break;
      }
    }

  } else {
    so_far_so_good = x_core_bool_false;
    *error = x_psql_system_create_error_unknown;
    x_audit_log_trace(log, "hpsq", "malloc");
  }

  if (so_far_so_good) {
    if (asprintf(&connection_string, "host=%s dbname=%s user=%s password=%s",
            db_host, db_name, db_user, db_password) != -1) {
      for (eacx_connection = 0; eacx_connection < connection_count;
           eacx_connection++) {
        *(system->connections + eacx_connection)
          = PQconnectdb(connection_string);
        pq_status = PQstatus(*(system->connections + eacx_connection));
        if (CONNECTION_OK != pq_status) {
          switch (pq_status) {
            case CONNECTION_BAD:
              *error = x_psql_system_create_error_connect_failed;
              break;
            default:
              *error = x_psql_system_create_error_unknown;
              break;
          }
          x_audit_log_trace(log, "hpsq", "PQsetdbLogin");
          PQfinish(*(system->connections + eacx_connection));
          *(system->connections + eacx_connection) = NULL;
          free(system);
          system = NULL;
          break;
        }
      }
      free(connection_string);
    } else {
      x_audit_log_trace(log, "hpsq", "asprintf");
    }
  }

  if (system && !so_far_so_good) {
    for (eacx_connection = 0; eacx_connection < connection_count;
         eacx_connection++) {
      if (*(connection_mutex_needs_destroy + eacx_connection)) {
        if (pthread_mutex_destroy
            (system->connection_mutexes + eacx_connection) != 0) {
          x_audit_log_trace(log, "hpsq", "pthread_mutex_destroy");
        }
      }
      if (*(system->connections + eacx_connection)) {
        PQfinish(*(system->connections + eacx_connection));
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void x_psql_system_destroy(x_psql_system_t *system)
{
  assert(system);
  unsigned short eacx_connection;

  free(system->db_host);
  free(system->db_name);
  for (eacx_connection = 0; eacx_connection < system->connection_count;
       eacx_connection++) {
    if (pthread_mutex_destroy
        (system->connection_mutexes + eacx_connection) != 0) {
      x_audit_log_trace(system->log, "hpsq", "pthread_mutex_destroy");
    }
    PQfinish(*(system->connections + eacx_connection));
  }
  free(system);
}

x_core_bool_t x_psql_system_execute(x_psql_system_t *system, char *sql,
    x_psql_system_execute_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  x_core_bool_t success;
  PGresult *pg_result;
  ExecStatusType exec_status;
  unsigned short connection_index;

  success = x_core_bool_false;

  connection_index = lock_connection(system);
  {
    pg_result = PQexec(*(system->connections + connection_index),
        (const char *) sql);
  }
  unlock_connection(system, connection_index);

  if (pg_result) {
    exec_status = PQresultStatus(pg_result);
    if (PGRES_COMMAND_OK == exec_status) {
      success = x_core_bool_true;
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = x_psql_system_execute_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = x_psql_system_execute_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = x_psql_system_execute_error_fatal_error;
          break;
        default:
          *error = x_psql_system_execute_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      x_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
    PQclear(pg_result);
  } else {
    *error = x_psql_system_execute_error_unknown;
  }

  return success;
}

x_psql_result_t *x_psql_system_query(x_psql_system_t *system, char *sql,
    x_psql_system_query_error_t *error, char **error_text)
{
  assert(system);
  assert(sql);
  assert(error);
  assert(error_text);
  x_psql_result_t *result;
  PGresult *pg_result;
  ExecStatusType exec_status;
  x_psql_result_create_error_t x_psql_result_create_error;
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
      result = x_psql_result_create(pg_result, &system->string_objectey,
          &x_psql_result_create_error);
      if (!result) {
        *error = x_psql_system_query_error_unknown;
        x_audit_log_trace(system->log, "hpsq", "x_psql_result_create");
      }
    } else {
      switch (exec_status) {
        case PGRES_BAD_RESPONSE:
          *error = x_psql_system_query_error_bad_response;
          break;
        case PGRES_EMPTY_QUERY:
          *error = x_psql_system_query_error_empty_query;
          break;
        case PGRES_FATAL_ERROR:
          *error = x_psql_system_query_error_fatal_error;
          break;
        default:
          *error = x_psql_system_query_error_unknown;
          break;
      }
      *error_text = PQresultErrorMessage(pg_result);
      x_audit_log_trace(system->log, "hpsq", "%s", *error_text);
    }
  } else {
    *error = x_psql_system_query_error_unknown;
  }

  return result;
}

x_core_bool_t x_psql_system_query_count(x_psql_system_t *system, char *sql,
    unsigned long *count, x_psql_system_query_count_error_t *error,
    char **error_text)
{
  assert(system);
  assert(sql);
  assert(count);
  assert(error);
  assert(error_text);
  x_core_bool_t success;
  x_psql_result_t *result;
  x_container_map_t *row;
  char *count_string;
  x_psql_system_query_error_t x_psql_system_query_error;

  success = x_core_bool_false;

  result = x_psql_system_query
    (system, sql, &x_psql_system_query_error, error_text);
  if (result) {
    row = x_psql_result_get_row(result, 0);
    count_string = x_container_map_find(row, "c");
    if (count_string) {
      success = x_core_bool_true;
      *count = atol(count_string);
    } else {
      *error = x_psql_system_query_count_error_no_c_column;
    }
    x_psql_result_destroy(result);
  } else {
    *error = x_psql_system_query_count_error_unknown;
  }

  return success;
}

x_core_bool_t x_psql_system_vacuum(x_psql_system_t *system,
    x_psql_system_vacuum_error_t *error)
{
  assert(system);
  assert(error);
  x_core_bool_t success;
  x_psql_system_execute_error_t x_psql_system_execute_error;
  char *x_psql_system_execute_error_text;

  x_audit_log_enter(system->log, "hpsq", "vacuum %s:%s", system->db_host,
      system->db_name);

  if (x_psql_system_execute(system, "vacuum", &x_psql_system_execute_error,
          &x_psql_system_execute_error_text)) {
    success = x_core_bool_true;
    x_audit_log_enter(system->log, "hpsq", "vacuum %s:%s complete",
        system->db_host, system->db_name);
  } else {
    success = x_core_bool_false;
    *error = x_psql_system_vacuum_error_unknown;
    x_audit_log_trace(system->log, "hpsq", "x_psql_system_execute");
  }

  return success;
}

void unlock_connection(x_psql_system_t *system,
    unsigned short connection_index)
{
  pthread_mutex_unlock(system->connection_mutexes + connection_index);
}
