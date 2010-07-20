#ifndef inferno_genetic_system_h
#define inferno_genetic_system_h

#include "inferno/core/goal.h"
#include "inferno/search/searchey.h"
#include "inferno/search/tools.h"

struct inferno_genetic_system_t;
typedef struct inferno_genetic_system_t inferno_genetic_system_t;

void *inferno_genetic_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context,
    h_container_array_t *initial_solutions, h_audit_log_t *log);

void inferno_genetic_system_destroy(void *system_object);

h_container_array_t *inferno_genetic_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void inferno_genetic_system_init_searchey(inferno_search_searchey_t *searchey);

void inferno_genetic_system_search(void *system_object);

#endif
