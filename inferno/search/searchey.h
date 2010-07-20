#ifndef inferno_search_searchey_h
#define inferno_search_searchey_h

#include "inferno/core/goal.h"
#include "inferno/search/tools.h"

typedef void * (*inferno_search_searchey_create_f)
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    h_container_array_t *initial_solutions, h_audit_log_t *log);

typedef void (*inferno_search_searchey_destroy_f)(void *search_object);

typedef h_container_array_t * (*inferno_search_searchey_get_solutions_copy_f)
  (void *search_object, unsigned short max_solution_count);

typedef void (*inferno_search_searchey_search_f)(void *search_object);

struct inferno_search_searchey_t {
  inferno_search_searchey_create_f create;
  inferno_search_searchey_destroy_f destroy;
  inferno_search_searchey_get_solutions_copy_f get_solutions_copy;
  inferno_search_searchey_search_f search;
};
typedef struct inferno_search_searchey_t inferno_search_searchey_t;

void inferno_search_searchey_init(inferno_search_searchey_t *searchey,
    inferno_search_searchey_create_f create, inferno_search_searchey_destroy_f destroy,
    inferno_search_searchey_get_solutions_copy_f get_solutions_copy,
    inferno_search_searchey_search_f search);

#endif
