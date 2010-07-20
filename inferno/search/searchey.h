#ifndef inferno_searcx_searchey_h
#define inferno_searcx_searchey_h

#include "inferno/core/goal.h"
#include "inferno/search/tools.h"

typedef void * (*inferno_searcx_searchey_create_f)
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    x_container_array_t *initial_solutions, x_audit_log_t *log);

typedef void (*inferno_searcx_searchey_destroy_f)(void *searcx_object);

typedef x_container_array_t * (*inferno_searcx_searchey_get_solutions_copy_f)
  (void *searcx_object, unsigned short max_solution_count);

typedef void (*inferno_searcx_searchey_searcx_f)(void *searcx_object);

struct inferno_searcx_searchey_t {
  inferno_searcx_searchey_create_f create;
  inferno_searcx_searchey_destroy_f destroy;
  inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy;
  inferno_searcx_searchey_searcx_f search;
};
typedef struct inferno_searcx_searchey_t inferno_searcx_searchey_t;

void inferno_searcx_searchey_init(inferno_searcx_searchey_t *searchey,
    inferno_searcx_searchey_create_f create, inferno_searcx_searchey_destroy_f destroy,
    inferno_searcx_searchey_get_solutions_copy_f get_solutions_copy,
    inferno_searcx_searchey_searcx_f search);

#endif
