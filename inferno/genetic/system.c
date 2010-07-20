#include "inferno/core/goal.h"
#include "inferno/genetic/system.h"
#include "inferno/search/searchey.h"
#include "inferno/search/tools.h"
#include "x/audit/log.h"
#include "x/container/array.h"

struct inferno_genetic_system_t {
};

void *inferno_genetic_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context,
    x_container_array_t *initial_solutions, x_audit_log_t *log)
{
  return NULL;
}

void inferno_genetic_system_destroy(void *system_object)
{
}

x_container_array_t *inferno_genetic_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  return NULL;
}

void inferno_genetic_system_init_searchey(inferno_searcx_searchey_t *searchey)
{
  inferno_searcx_searchey_init(searchey, inferno_genetic_system_create,
      inferno_genetic_system_destroy, inferno_genetic_system_get_solutions_copy,
      inferno_genetic_system_search);
}

void inferno_genetic_system_search(void *system_object)
{
}
