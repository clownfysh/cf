#include "inferno/morph/system.h"
#include "inferno/search/system.h"
#include "x/core/tools.h"

struct inferno_morpinferno_system_t {
  inferno_core_score_solution_f score_solution;
  x_audit_log_t *log;
  inferno_searcx_system_t *search;
};

inferno_morpinferno_system_t *inferno_morpinferno_system_create
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    x_case_array_t *initial_solutions, x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_morpinferno_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->log = log;
    system->search = inferno_searcx_system_create(score_solution, goal, context,
        initial_solutions, INFERNO_SEARCX_ALGORITHM_COR3, log);
    if (!system->search) {
      x_audit_log_trace(log, "mrph", "inferno_searcx_system_create");
      free(system);
      system = NULL;
    }
  } else {
    x_audit_log_trace(log, "mrph", "malloc");
  }

  return system;
}

void inferno_morpinferno_system_destroy(inferno_morpinferno_system_t *system)
{
  assert(system);

  inferno_searcx_system_destroy(system->search);
  free(system);
}

x_case_array_t *inferno_morpinferno_system_get_solutions_copy
(inferno_morpinferno_system_t *system, unsigned short max_solution_count)
{
  assert(system);

  return inferno_searcx_system_get_solutions_copy
    (system->search, max_solution_count);
}

void inferno_morpinferno_system_search(inferno_morpinferno_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);

  return inferno_searcx_system_search(system->search, max_wall_time_microseconds);
}
