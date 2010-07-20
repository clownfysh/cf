#ifndef inferno_ccsv_system_h
#define inferno_ccsv_system_h

struct inferno_ccsv_system_t;
typedef struct inferno_ccsv_system_t inferno_ccsv_system_t;

inferno_ccsv_system_t *inferno_ccsv_system_create();

void inferno_ccsv_system_destroy(void *system_void);

#endif
