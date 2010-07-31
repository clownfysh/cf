#ifndef cf_sage_cor3_system_h
#define cf_sage_cor3_system_h

#include "cf/sage/cor3/declarations.h"
#include "cf/sage/core/goal.h"
#include "cf/sage/search/searchey.h"
#include "cf/sage/search/tools.h"
#include "cf/x/core/tools.h"

void *cf_sage_cor3_system_create(cf_sage_core_score_solution_f score_solution,
    cf_sage_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log);

void cf_sage_cor3_system_destroy(void *system_object);

cf_sage_box_system_t *cf_sage_cor3_system_get_box(cf_sage_cor3_system_t *system);

void *cf_sage_cor3_system_get_context(cf_sage_cor3_system_t *system);

cf_sage_core_goal_t cf_sage_cor3_system_get_goal(cf_sage_cor3_system_t *system);

cf_x_audit_log_t *cf_sage_cor3_system_get_log(cf_sage_cor3_system_t *system);

cf_sage_core_score_solution_f cf_sage_cor3_system_get_score_solution
(cf_sage_cor3_system_t *system);

cf_x_case_array_t *cf_sage_cor3_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void cf_sage_cor3_system_init_searchey(cf_sage_searcx_searchey_t *searchey);

void cf_sage_cor3_system_search(void *system_object);

#endif
