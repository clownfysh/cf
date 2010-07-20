#ifndef inferno_cor3_actor_h
#define inferno_cor3_actor_h

#include "inferno/cor3/declarations.h"
#include "inferno/core/actorey.h"

void inferno_cor3_actor_act(inferno_cor3_actor_t *actor);

int inferno_cor3_actor_compare_maximize(void *actor_a_void, void *actor_b_void);

int inferno_cor3_actor_compare_minimize(void *actor_a_void, void *actor_b_void);

void *inferno_cor3_actor_copy(void *actor_void);

void *inferno_cor3_actor_create(void *system_void, x_core_bitarray_t *solution);

void *inferno_cor3_actor_create_random(void *system_void);

void inferno_cor3_actor_destroy(void *actor_void);

void *inferno_cor3_actor_get_box_cell(void *actor_void);

x_core_bitarray_t *inferno_cor3_actor_get_solution(void *actor_void);

void inferno_cor3_actor_init_actorey(inferno_core_actorey_t *actorey);

void inferno_cor3_actor_set_box_cell(void *actor_void, void *box_cell);

#endif
