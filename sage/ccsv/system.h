#ifndef cf_sage_ccsv_system_h
#define cf_sage_ccsv_system_h

struct cf_sage_ccsv_system_t;
typedef struct cf_sage_ccsv_system_t cf_sage_ccsv_system_t;

cf_sage_ccsv_system_t *cf_sage_ccsv_system_create();

void cf_sage_ccsv_system_destroy(void *system_void);

#endif
