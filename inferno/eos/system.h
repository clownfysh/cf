#ifndef inferno_eos_system_h
#define inferno_eos_system_h

#include "inferno/eos/declarations.h"
#include "inferno/search/searchey.h"
#include "inferno/search/tools.h"

void *inferno_eos_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context,
    x_container_array_t *initial_solutions, x_audit_log_t *log);

void inferno_eos_system_destroy(void *system_object);

inferno_box_system_t *inferno_eos_system_get_box(inferno_eos_system_t *system);

void *inferno_eos_system_get_context(inferno_eos_system_t *system);

inferno_core_goal_t inferno_eos_system_get_goal(inferno_eos_system_t *system);

x_audit_log_t *inferno_eos_system_get_log(inferno_eos_system_t *system);

inferno_core_score_solution_f inferno_eos_system_get_score_solution
(inferno_eos_system_t *system);

x_container_array_t *inferno_eos_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void inferno_eos_system_init_searchey(inferno_searcx_searchey_t *searchey);

void inferno_eos_system_search(void *system_object);

#endif
