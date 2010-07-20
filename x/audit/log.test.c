#include "x/audit/log.h"
#include "x/file/basic.h"

int main(int argc, char *argv[])
{
  x_audit_log_t *log;
  x_file_basic_t *disk_file_a;
  x_file_basic_t *disk_file_b;
  FILE *disk_file_a_fp;
  FILE *disk_file_b_fp;

  disk_file_a = x_file_basic_create("x/audit/a.log",
      X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_a) {
    x_core_trace_exit("x_file_basic_create");
  }
  disk_file_a_fp = x_file_basic_get_file(disk_file_a);

  log = x_audit_log_create(disk_file_a_fp);
  if (!log) {
    x_core_trace_exit("x_audit_log_create");
  }

  x_audit_log_enter(log, "haud", "::int=%d::double=%f::string=%s::", 11111,
      2.2222, "three");

  disk_file_b = x_file_basic_create("x/audit/b.log",
      X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_b) {
    x_core_trace_exit("x_audit_file_create");
  }
  disk_file_b_fp = x_file_basic_get_file(disk_file_b);

  if (!x_audit_log_add_file(log, disk_file_b_fp)) {
    x_core_trace_exit("x_audit_log_add_file");
  }

  x_audit_log_enter(log, "haud", "this should be in both %s.log and %s.log",
      "a", "b");

  if (!x_audit_log_add_file(log, stdout)) {
    x_core_trace_exit("x_audit_log_add_file");
  }

  x_audit_log_enter(log, "haud", "word");

  x_audit_log_enter(log, "haud", "The quick brown fox jumps over the lazy dog."
      "  The quick brown fox jumps over the lazy dog.");

  x_audit_log_destroy(log);
  x_file_basic_destroy(disk_file_a);
  x_file_basic_destroy(disk_file_b);

  return 0;
}
