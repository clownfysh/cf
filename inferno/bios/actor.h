#ifndef cf_inferno_bios_actor_h
#define cf_inferno_bios_actor_h

#include "cf/inferno/core/iactor.h"

struct cf_inferno_bios_actor_t;
typedef struct cf_inferno_bios_actor_t cf_inferno_bios_actor_t;

void cf_inferno_bios_actor_act(cf_inferno_bios_actor_t *actor);

int cf_inferno_bios_actor_compare_maximize(void *actor_a_void, void *actor_b_void);

int cf_inferno_bios_actor_compare_minimize(void *actor_a_void, void *actor_b_void);

void *cf_inferno_bios_actor_copy(void *actor_void);

void *cf_inferno_bios_actor_create(void *system_void, cf_x_core_bitarray_t *solution);

void *cf_inferno_bios_actor_create_random(void *system_void);

void cf_inferno_bios_actor_destroy(void *actor_void);

void *cf_inferno_bios_actor_get_box_cell(void *actor_void);

cf_x_core_bitarray_t *cf_inferno_bios_actor_get_solution(void *actor_void);

void cf_inferno_bios_actor_init_iactor(cf_inferno_core_iactor_t *iactor);

void cf_inferno_bios_actor_set_box_cell(void *actor_void, void *box_cell);

#endif
