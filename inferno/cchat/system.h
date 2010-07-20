#ifndef inferno_cchat_system_h
#define inferno_cchat_system_h

struct inferno_cchat_system_t;
typedef struct inferno_cchat_system_t inferno_cchat_system_t;

inferno_cchat_system_t *inferno_cchat_system_create();

void inferno_cchat_system_destroy(inferno_cchat_system_t *system);

#endif
