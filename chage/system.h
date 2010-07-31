#ifndef cf_chage_system_h
#define cf_chage_system_h

struct cf_chage_system_t;
typedef struct cf_chage_system_t cf_chage_system_t;

cf_chage_system_t *cf_chage_system_create();

void cf_chage_system_destroy(cf_chage_system_t *system);

void cf_chage_system_spark(cf_chage_system_t *system);

#endif
