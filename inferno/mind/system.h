#ifndef cf_inferno_mind_system_h
#define cf_inferno_mind_system_h

struct cf_inferno_mind_system_t;
typedef struct cf_inferno_mind_system_t cf_inferno_mind_system_t;

cf_inferno_mind_system_t *cf_inferno_mind_system_create();

void cf_inferno_mind_system_destroy(cf_inferno_mind_system_t *system);

#endif
