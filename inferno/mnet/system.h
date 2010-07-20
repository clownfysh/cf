#ifndef inferno_mnet_system_h
#define inferno_mnet_system_h

struct inferno_mnet_system_t;
typedef struct inferno_mnet_system_t inferno_mnet_system_t;

inferno_mnet_system_t *inferno_mnet_system_create();

void inferno_mnet_system_destroy(inferno_mnet_system_t *system);

#endif
