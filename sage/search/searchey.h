#ifndef cf_sage_searcx_searchey_h
#define cf_sage_searcx_searchey_h

#include "cf/sage/core/goal.h"
#include "cf/sage/search/tools.h"

typedef void * (*cf_sage_searcx_searchey_create_f)
(cf_sage_core_score_solution_f score_solution, cf_sage_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log);

typedef void (*cf_sage_searcx_searchey_destroy_f)(void *searcx_object);

typedef cf_x_case_array_t * (*cf_sage_searcx_searchey_get_solutions_copy_f)
  (void *searcx_object, unsigned short max_solution_count);

typedef void (*cf_sage_searcx_searchey_searcx_f)(void *searcx_object);

struct cf_sage_searcx_searchey_t {
  cf_sage_searcx_searchey_create_f create;
  cf_sage_searcx_searchey_destroy_f destroy;
  cf_sage_searcx_searchey_get_solutions_copy_f get_solutions_copy;
  cf_sage_searcx_searchey_searcx_f search;
};
typedef struct cf_sage_searcx_searchey_t cf_sage_searcx_searchey_t;

void cf_sage_searcx_searchey_init(cf_sage_searcx_searchey_t *searchey,
    cf_sage_searcx_searchey_create_f create, cf_sage_searcx_searchey_destroy_f destroy,
    cf_sage_searcx_searchey_get_solutions_copy_f get_solutions_copy,
    cf_sage_searcx_searchey_searcx_f search);

#endif
