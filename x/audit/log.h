#ifndef x_audit_log_h
#define x_audit_log_h

#include "x/core/bool.h"
#include "x/core/tools.h"

#define x_audit_log_trace(log, system, entry, ...) _x_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__);

#define x_audit_log_trace_exit(log, system, entry, ...) _x_audit_log_trace \
  (log, system, entry, __FILE__, __LINE__, ##__VA_ARGS__); exit(44);

struct x_audit_log_t;
typedef struct x_audit_log_t x_audit_log_t;

x_core_bool_t x_audit_log_add_file(x_audit_log_t *log, FILE *file);

x_audit_log_t *x_audit_log_create(FILE *file);

void x_audit_log_destroy(x_audit_log_t *log);

x_core_bool_t x_audit_log_enter(x_audit_log_t *log, const char *system,
    const char *entry, ...);

x_core_bool_t x_audit_log_remove_file(x_audit_log_t *log, FILE *file);

x_core_bool_t _x_audit_log_trace(x_audit_log_t *log, const char *system,
    const char *entry, const char *file, unsigned long line, ...);

#endif
