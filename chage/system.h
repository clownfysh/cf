#ifndef chage_system_h
#define chage_system_h

struct chage_system_t;
typedef struct chage_system_t chage_system_t;

chage_system_t *chage_system_create();

void chage_system_destroy(chage_system_t *system);

void chage_system_spark(chage_system_t *system);

#endif
