#include "cf/x/audit/log.h"
#include "cf/x/case/set.h"
#include "cf/x/core/unsigned_long.h"

#define MAX_ENTRY_SIZE 1024
#define MAX_LINE_LENGTH 79
#define SPACE_CUTOFF 64
#define SPACE_CHAR ' '

struct cf_x_audit_log_t {
  cf_x_case_set_t *files;
  pthread_mutex_t mutex;
  cf_x_core_objectey_t files_objectey;
};

static int compare_files(void *a_file_object, void *b_file_object);

static cf_x_core_bool_t enter_file(char *formatted_entry, FILE *file);

static unsigned short find_line_end(char *entry,
    unsigned short max_line_length);

static char *format_entry(char *entry);

static cf_x_core_bool_t write_entry_to_all_files(cf_x_audit_log_t *log,
    char *formatted_entry);

int compare_files(void *a_file_object, void *b_file_object)
{
  assert(a_file_object);
  assert(b_file_object);
  int compare_result;
  FILE *a_file;
  FILE *b_file;

  a_file = a_file_object;
  b_file = b_file_object;

  if (a_file < b_file) {
    compare_result = -1;
  } else if (a_file > b_file) {
    compare_result = 1;
  } else {
    compare_result = 0;
  }

  return compare_result;
}

cf_x_core_bool_t cf_x_audit_log_add_file(cf_x_audit_log_t *log, FILE *file)
{
  assert(log);
  assert(file);
  cf_x_core_bool_t success;

  pthread_mutex_lock(&log->mutex);
  success = cf_x_case_set_add(log->files, file);
  pthread_mutex_unlock(&log->mutex);

  return success;
}

cf_x_audit_log_t *cf_x_audit_log_create(FILE *file)
{
  cf_x_audit_log_t *log;
  cf_x_core_bool_t so_far_so_good;

  log = malloc(sizeof *log);
  if (log) {
    so_far_so_good = cf_x_core_bool_true;
  } else {
    cf_x_core_trace("malloc");
    so_far_so_good = cf_x_core_bool_false;
  }

  if (so_far_so_good) {
    cf_x_core_objectey_init(&log->files_objectey, compare_files,
        CF_X_CORE_NO_COPY_FUNCTION, CF_X_CORE_NO_DESTROY_FUNCTION,
        cf_x_core_unsigned_long_equal, CF_X_CORE_NO_GET_AS_STRING_FUNCTION,
        cf_x_core_unsigned_long_mod);
    log->files = cf_x_case_set_create(&log->files_objectey);
    if (log->files) {
      if (file) {
        if (cf_x_case_set_add(log->files, file)) {
          so_far_so_good = cf_x_core_bool_true;
        } else {
          so_far_so_good = cf_x_core_bool_false;
        }
      } else {
        so_far_so_good = cf_x_core_bool_true;
      }
    } else {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (0 != pthread_mutex_init(&log->mutex, NULL)) {
      so_far_so_good = cf_x_core_bool_false;
    }
  }

  if (!so_far_so_good && log) {
    if (log->files) {
      cf_x_case_set_destroy(log->files);
    }
  }

  assert(!log || (log->files));
  return log;
}

void cf_x_audit_log_destroy(cf_x_audit_log_t *log)
{
  assert(log);

  cf_x_case_set_destroy(log->files);
  pthread_mutex_destroy(&log->mutex);
  free(log);
}

cf_x_core_bool_t cf_x_audit_log_enter(cf_x_audit_log_t *log, const char *system,
    const char *entry, ...)
{
  assert(log);
  assert(system);
  assert(entry);
  char *timed_entry;
  char interpolated_entry[MAX_ENTRY_SIZE];
  char *formatted_entry;
  va_list ap;
  cf_x_core_bool_t success;
  cf_x_core_time_string_t time_string;

  success = cf_x_core_bool_false;

  if (cf_x_core_get_current_time_string(time_string)) {
    if (asprintf(&timed_entry, "%s [%4s]   %s", time_string, system, entry)
        != -1) {
      va_start(ap, entry);
      if (vsnprintf(interpolated_entry, MAX_ENTRY_SIZE, timed_entry, ap)
          != -1) {
        formatted_entry = format_entry(interpolated_entry);
        if (formatted_entry) {
          if (write_entry_to_all_files(log, formatted_entry)) {
            success = cf_x_core_bool_true;
          } else {
            cf_x_core_trace("write_entry_to_all_files");
          }
          free(formatted_entry);
        } else {
          cf_x_core_trace("format_entry");
        }
      } else {
        cf_x_core_trace("asprintf");
      }
      free(timed_entry);
    } else {
      cf_x_core_trace("asprintf");
    }
  } else {
    cf_x_core_trace("x_core_get_current_time_string");
  }

  return success;
}

cf_x_core_bool_t cf_x_audit_log_remove_file(cf_x_audit_log_t *log, FILE *file)
{
  assert(log);
  assert(file);
  cf_x_core_bool_t success;

  pthread_mutex_lock(&log->mutex);
  success = cf_x_case_set_remove(log->files, file);
  pthread_mutex_unlock(&log->mutex);

  return success;
}

cf_x_core_bool_t _x_audit_log_trace(cf_x_audit_log_t *log, const char *system,
    const char *entry, const char *file, unsigned long line, ...)
{
  assert(log);
  assert(system);
  assert(entry);
  assert(file);
  char *traced_entry;
  char interpolated_entry[MAX_ENTRY_SIZE];
  char *formatted_entry;
  va_list ap;
  cf_x_core_bool_t success;
  cf_x_core_time_string_t time_string;

  success = cf_x_core_bool_false;

  if (cf_x_core_get_current_time_string(time_string)) {
    if (asprintf(&traced_entry, "%s [%4s] X %s() in %s:%lu", time_string,
            system, entry, file, line) != -1) {
      va_start(ap, line);
      if (vsnprintf(interpolated_entry, MAX_ENTRY_SIZE, traced_entry, ap)
          != -1) {
        formatted_entry = format_entry(interpolated_entry);
        if (formatted_entry) {
          if (write_entry_to_all_files(log, formatted_entry)) {
            success = cf_x_core_bool_true;
          } else {
            cf_x_core_trace("write_entry_to_all_files");
          }
          free(formatted_entry);
        } else {
          cf_x_core_trace("format_entry");
        }
      } else {
        cf_x_core_trace("asprintf");
      }
      va_end(ap);
      free(traced_entry);
    } else {
      cf_x_core_trace("asprintf");
    }
  } else {
    cf_x_core_trace("x_core_get_current_time_string");
  }

  return success;
}

cf_x_core_bool_t enter_file(char *formatted_entry, FILE *file)
{
  assert(formatted_entry);
  assert(file);
  cf_x_core_bool_t success;

  if (fprintf(file, "%s", formatted_entry) >= 0) {
    if (0 == fflush(file)) {
      success = cf_x_core_bool_true;
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("fflush");
    }
  } else {
    success = cf_x_core_bool_false;
    cf_x_core_trace("fprintf");
  }

  return success;
}

unsigned short find_line_end(char *entry, unsigned short max_line_length)
{
  /*
    TODO (rainy day): make this support other whitespace chars
  */

  assert(entry);
  unsigned short line_end;
  unsigned long entry_size;

  entry_size = strlen(entry);
  if (entry_size <= max_line_length) {
    line_end = entry_size;
  } else {
    line_end = max_line_length - 1;
    while (line_end > SPACE_CUTOFF && (*(entry + line_end) != SPACE_CHAR)) {
      line_end--;
    }
    if (*(entry + line_end) != SPACE_CHAR) {
      line_end = max_line_length;
    } else {
      line_end++;
    }
  }

  return line_end;
}

/*
  TODO: test return codes
*/
char *format_entry(char *entry)
{
  assert(entry);
  char *formatted_entry;
  unsigned short line_end;
  char *entry_start;

  formatted_entry = NULL;
  entry_start = entry;

  line_end = find_line_end(entry_start, MAX_LINE_LENGTH);
  while (line_end > 0) {
    formatted_entry = cf_x_core_string_append_n(formatted_entry,
        entry_start, line_end);
    formatted_entry = cf_x_core_string_append_char(formatted_entry, '\n');
    entry_start += line_end;
    line_end = find_line_end(entry_start, MAX_LINE_LENGTH
        - (CF_X_CORE_TIME_STRING_LENGTH + 1 + 1 + 4 + 1 + 3));
    if (line_end > 0) {
      formatted_entry = cf_x_core_string_append
        (formatted_entry, "                         ");
    }
  }

  return formatted_entry;
}

cf_x_core_bool_t write_entry_to_all_files(cf_x_audit_log_t *log,
    char *formatted_entry)
{
  assert(log);
  assert(formatted_entry);
  cf_x_core_bool_t success;
  FILE *file;

  success = cf_x_core_bool_true;

  pthread_mutex_lock(&log->mutex);
  {
    cf_x_case_set_iterate_start(log->files);
    while ((file = cf_x_case_set_iterate_next(log->files))) {
      if (!enter_file(formatted_entry, file)) {
        cf_x_core_trace("enter_file");
        success = cf_x_core_bool_false;
      }
    }
  }
  pthread_mutex_unlock(&log->mutex);

  return success;
}
