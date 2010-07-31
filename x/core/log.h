#ifndef cf_x_audit_log_h
#define cf_x_audit_log_h

#include "cf/x/core/bool.h"
#include "cf/x/core/tools.h"

#define cf_x_audit_log_trace(log, system, entry, ...) _x_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__);

#define cf_x_audit_log_trace_exit(log, system, entry, ...) _x_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__); exit(44);

struct cf_x_audit_log_t;
typedef struct cf_x_audit_log_t cf_x_audit_log_t;

cf_x_core_bool_t cf_x_audit_log_add_file(cf_x_audit_log_t *log, FILE *file);

cf_x_audit_log_t *cf_x_audit_log_create(FILE *file);

void cf_x_audit_log_destroy(cf_x_audit_log_t *log);

cf_x_core_bool_t cf_x_audit_log_enter(cf_x_audit_log_t *log, const char *system,
    const char *entry, ...);

cf_x_core_bool_t cf_x_audit_log_remove_file(cf_x_audit_log_t *log, FILE *file);

cf_x_core_bool_t _x_audit_log_trace(cf_x_audit_log_t *log, const char *system,
    const char *entry, const char *file, unsigned long line, ...);

#endif
