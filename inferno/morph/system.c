#include "inferno/morph/system.h"
#include "inferno/search/system.h"
#include "h/core/tools.h"

struct inferno_morpinferno_system_t {
  inferno_core_score_solution_f score_solution;
  h_audit_log_t *log;
  inferno_search_system_t *search;
};

inferno_morpinferno_system_t *inferno_morpinferno_system_create
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    h_container_array_t *initial_solutions, h_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_morpinferno_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->log = log;
    system->search = inferno_search_system_create(score_solution, goal, context,
        initial_solutions, INFERNO_SEARCH_ALGORITHM_COR3, log);
    if (!system->search) {
      h_audit_log_trace(log, "mrph", "inferno_search_system_create");
      free(system);
      system = NULL;
    }
  } else {
    h_audit_log_trace(log, "mrph", "malloc");
  }

  return system;
}

void inferno_morpinferno_system_destroy(inferno_morpinferno_system_t *system)
{
  assert(system);

  inferno_search_system_destroy(system->search);
  free(system);
}

h_container_array_t *inferno_morpinferno_system_get_solutions_copy
(inferno_morpinferno_system_t *system, unsigned short max_solution_count)
{
  assert(system);

  return inferno_search_system_get_solutions_copy
    (system->search, max_solution_count);
}

void inferno_morpinferno_system_search(inferno_morpinferno_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);

  return inferno_search_system_search(system->search, max_wall_time_microseconds);
}
