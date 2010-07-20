#ifndef inferno_bios_system_h
#define inferno_bios_system_h

#include "inferno/bios/declarations.h"
#include "inferno/search/searchey.h"
#include "inferno/search/tools.h"

void *inferno_bios_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context, h_container_array_t *initial_solutions,
    h_audit_log_t *log);

void inferno_bios_system_destroy(void *system_object);

inferno_box_system_t *inferno_bios_system_get_box(inferno_bios_system_t *system);

void *inferno_bios_system_get_context(inferno_bios_system_t *system);

inferno_core_goal_t inferno_bios_system_get_goal(inferno_bios_system_t *system);

h_audit_log_t *inferno_bios_system_get_log(inferno_bios_system_t *system);

inferno_core_score_solution_f inferno_bios_system_get_score_solution
(inferno_bios_system_t *system);

h_container_array_t *inferno_bios_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void inferno_bios_system_init_searchey(inferno_search_searchey_t *searchey);

void inferno_bios_system_search(void *system_object);

#endif
