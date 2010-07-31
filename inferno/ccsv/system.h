#ifndef cf_inferno_ccsv_system_h
#define cf_inferno_ccsv_system_h

struct cf_inferno_ccsv_system_t;
typedef struct cf_inferno_ccsv_system_t cf_inferno_ccsv_system_t;

cf_inferno_ccsv_system_t *cf_inferno_ccsv_system_create();

void cf_inferno_ccsv_system_destroy(void *system_void);

#endif
