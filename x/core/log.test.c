#include "cf/x/core/log.h"
#include "cf/x/file/basic.h"

int main(int argc, char *argv[])
{
  cf_x_core_log_t *log;
  cf_x_file_basic_t *disk_file_a;
  cf_x_file_basic_t *disk_file_b;
  FILE *disk_file_a_fp;
  FILE *disk_file_b_fp;

  disk_file_a = cf_x_file_basic_create("x/core/a.log",
      CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_a) {
    cf_x_core_trace_exit("x_file_basic_create");
  }
  disk_file_a_fp = cf_x_file_basic_get_file(disk_file_a);

  log = cf_x_core_log_create(disk_file_a_fp);
  if (!log) {
    cf_x_core_trace_exit("x_audit_log_create");
  }

  cf_x_core_log_enter(log, "haud", "::int=%d::double=%f::string=%s::", 11111,
      2.2222, "three");

  disk_file_b = cf_x_file_basic_create("x/core/b.log",
      CF_X_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_b) {
    cf_x_core_trace_exit("x_audit_file_create");
  }
  disk_file_b_fp = cf_x_file_basic_get_file(disk_file_b);

  if (!cf_x_core_log_add_file(log, disk_file_b_fp)) {
    cf_x_core_trace_exit("x_audit_log_add_file");
  }

  cf_x_core_log_enter(log, "haud", "this should be in both %s.log and %s.log",
      "a", "b");

  if (!cf_x_core_log_add_file(log, stdout)) {
    cf_x_core_trace_exit("x_audit_log_add_file");
  }

  cf_x_core_log_enter(log, "haud", "word");

  cf_x_core_log_enter(log, "haud", "The quick brown fox jumps over the lazy dog."
      "  The quick brown fox jumps over the lazy dog.");

  cf_x_core_log_destroy(log);
  cf_x_file_basic_destroy(disk_file_a);
  cf_x_file_basic_destroy(disk_file_b);

  return 0;
}
