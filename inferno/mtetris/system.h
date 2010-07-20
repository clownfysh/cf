#ifndef inferno_mtetris_system_h
#define inferno_mtetris_system_h

struct inferno_mtetris_system_t;
typedef struct inferno_mtetris_system_t inferno_mtetris_system_t;

inferno_mtetris_system_t *inferno_mtetris_system_create();

void inferno_mtetris_system_destroy(inferno_mtetris_system_t *system);

#endif
