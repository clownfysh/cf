#ifndef inferno_stravel_system_h
#define inferno_stravel_system_h

struct inferno_stravel_system_t;
typedef struct inferno_stravel_system_t inferno_stravel_system_t;

inferno_stravel_system_t *inferno_stravel_system_create();

void inferno_stravel_system_destroy(inferno_stravel_system_t *system);

#endif
