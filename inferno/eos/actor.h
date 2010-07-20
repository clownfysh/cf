#ifndef inferno_eos_actor_h
#define inferno_eos_actor_h

#include "inferno/core/actorey.h"
#include "inferno/eos/declarations.h"

void inferno_eos_actor_act(inferno_eos_actor_t *actor);

int inferno_eos_actor_compare_maximize(void *actor_a_void, void *actor_b_void);

int inferno_eos_actor_compare_minimize(void *actor_a_void, void *actor_b_void);

void *inferno_eos_actor_copy(void *actor_void);

void *inferno_eos_actor_create(void *system_void, h_core_bitarray_t *solution);

void *inferno_eos_actor_create_random(void *system_void);

void inferno_eos_actor_destroy(void *actor_void);

void *inferno_eos_actor_get_box_cell(void *actor_void);

h_core_bitarray_t *inferno_eos_actor_get_solution(void *actor_void);

void inferno_eos_actor_init_actorey(inferno_core_actorey_t *actorey);

void inferno_eos_actor_set_box_cell(void *actor_void, void *box_cell);

#endif
