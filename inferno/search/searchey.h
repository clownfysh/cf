#ifndef cf_inferno_searcx_searchey_h
#define cf_inferno_searcx_searchey_h

#include "cf/inferno/core/goal.h"
#include "cf/inferno/search/tools.h"

typedef void * (*cf_inferno_searcx_searchey_create_f)
(cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log);

typedef void (*cf_inferno_searcx_searchey_destroy_f)(void *searcx_object);

typedef cf_x_case_array_t * (*cf_inferno_searcx_searchey_get_solutions_copy_f)
  (void *searcx_object, unsigned short max_solution_count);

typedef void (*cf_inferno_searcx_searchey_searcx_f)(void *searcx_object);

struct cf_inferno_searcx_searchey_t {
  cf_inferno_searcx_searchey_create_f create;
  cf_inferno_searcx_searchey_destroy_f destroy;
  cf_inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy;
  cf_inferno_searcx_searchey_searcx_f search;
};
typedef struct cf_inferno_searcx_searchey_t cf_inferno_searcx_searchey_t;

void cf_inferno_searcx_searchey_init(cf_inferno_searcx_searchey_t *searchey,
    cf_inferno_searcx_searchey_create_f create, cf_inferno_searcx_searchey_destroy_f destroy,
    cf_inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy,
    cf_inferno_searcx_searchey_searcx_f search);

#endif
