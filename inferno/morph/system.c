#include "cf/inferno/morph/system.h"
#include "cf/inferno/search/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_morpinferno_system_t {
  cf_inferno_core_score_solution_f score_solution;
  cf_x_audit_log_t *log;
  cf_inferno_searcx_system_t *search;
};

cf_inferno_morpinferno_system_t *cf_inferno_morpinferno_system_create
(cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  cf_inferno_morpinferno_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->log = log;
    system->search = cf_inferno_searcx_system_create(score_solution, goal, context,
        initial_solutions, CF_INFERNO_SEARCX_ALGORITHM_COR3, log);
    if (!system->search) {
      cf_x_audit_log_trace(log, "mrph", "inferno_searcx_system_create");
      free(system);
      system = NULL;
    }
  } else {
    cf_x_audit_log_trace(log, "mrph", "malloc");
  }

  return system;
}

void cf_inferno_morpinferno_system_destroy(cf_inferno_morpinferno_system_t *system)
{
  assert(system);

  cf_inferno_searcx_system_destroy(system->search);
  free(system);
}

cf_x_case_array_t *cf_inferno_morpinferno_system_get_solutions_copy
(cf_inferno_morpinferno_system_t *system, unsigned short max_solution_count)
{
  assert(system);

  return cf_inferno_searcx_system_get_solutions_copy
    (system->search, max_solution_count);
}

void cf_inferno_morpinferno_system_search(cf_inferno_morpinferno_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);

  return cf_inferno_searcx_system_search(system->search, max_wall_time_microseconds);
}
