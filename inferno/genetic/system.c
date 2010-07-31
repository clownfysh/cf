#include "cf/inferno/core/goal.h"
#include "cf/inferno/genetic/system.h"
#include "cf/inferno/search/searchey.h"
#include "cf/inferno/search/tools.h"
#include "cf/x/audit/log.h"
#include "cf/x/case/array.h"

struct cf_inferno_genetic_system_t {
};

void *cf_inferno_genetic_system_create(cf_inferno_core_score_solution_f score_solution,
    cf_inferno_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log)
{
  return NULL;
}

void cf_inferno_genetic_system_destroy(void *system_object)
{
}

cf_x_case_array_t *cf_inferno_genetic_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  return NULL;
}

void cf_inferno_genetic_system_init_searchey(cf_inferno_searcx_searchey_t *searchey)
{
  cf_inferno_searcx_searchey_init(searchey, cf_inferno_genetic_system_create,
      cf_inferno_genetic_system_destroy, cf_inferno_genetic_system_get_solutions_copy,
      cf_inferno_genetic_system_search);
}

void cf_inferno_genetic_system_search(void *system_object)
{
}
