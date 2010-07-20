#ifndef inferno_mind_system_h
#define inferno_mind_system_h

struct inferno_mind_system_t;
typedef struct inferno_mind_system_t inferno_mind_system_t;

inferno_mind_system_t *inferno_mind_system_create();

void inferno_mind_system_destroy(inferno_mind_system_t *system);

#endif
