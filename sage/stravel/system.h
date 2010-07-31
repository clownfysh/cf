#ifndef cf_sage_stravel_system_h
#define cf_sage_stravel_system_h

struct cf_sage_stravel_system_t;
typedef struct cf_sage_stravel_system_t cf_sage_stravel_system_t;

cf_sage_stravel_system_t *cf_sage_stravel_system_create();

void cf_sage_stravel_system_destroy(cf_sage_stravel_system_t *system);

#endif
