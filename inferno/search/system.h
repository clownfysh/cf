#ifndef cf_inferno_search_system_h
#define cf_inferno_search_system_h

#include "cf/inferno/search/algorithm.h"
#include "cf/inferno/search/tools.h"

struct cf_inferno_search_system_t;
typedef struct cf_inferno_search_system_t cf_inferno_search_system_t;

cf_inferno_search_system_t *cf_inferno_search_system_create
(cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal,
    void *context, cf_x_case_array_t *initial_solutions,
    cf_inferno_search_algorithm_t algorithm, cf_x_core_log_t *log);

void cf_inferno_search_system_destroy(cf_inferno_search_system_t *system);

cf_x_case_array_t *cf_inferno_search_system_get_solutions_copy
(cf_inferno_search_system_t *system, unsigned short max_solution_count);

void cf_inferno_search_system_search(cf_inferno_search_system_t *system,
    unsigned long max_wall_time_microseconds);

#endif
