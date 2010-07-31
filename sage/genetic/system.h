#ifndef cf_sage_genetic_system_h
#define cf_sage_genetic_system_h

#include "cf/sage/core/goal.h"
#include "cf/sage/search/searchey.h"
#include "cf/sage/search/tools.h"

struct cf_sage_genetic_system_t;
typedef struct cf_sage_genetic_system_t cf_sage_genetic_system_t;

void *cf_sage_genetic_system_create(cf_sage_core_score_solution_f score_solution,
    cf_sage_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log);

void cf_sage_genetic_system_destroy(void *system_object);

cf_x_case_array_t *cf_sage_genetic_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void cf_sage_genetic_system_init_searchey(cf_sage_searcx_searchey_t *searchey);

void cf_sage_genetic_system_search(void *system_object);

#endif
