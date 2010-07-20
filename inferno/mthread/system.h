#ifndef inferno_mthread_system_h
#define inferno_mthread_system_h

struct inferno_mthread_system_t;
typedef struct inferno_mthread_system_t inferno_mthread_system_t;

inferno_mthread_system_t *inferno_mthread_system_create();

void inferno_mthread_system_destroy(inferno_mthread_system_t *system);

#endif
