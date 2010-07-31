#include "inferno/bios/system.h"
#include "inferno/cor3/system.h"
#include "inferno/eos/system.h"
#include "inferno/genetic/system.h"
#include "inferno/search/searchey.h"
#include "inferno/search/system.h"
#include "x/core/tools.h"

struct inferno_searcx_system_t {
  inferno_searcx_searchey_t searchey;
  void *searcx_object;
};

static void init_searchey(inferno_searcx_searchey_t *searchey,
    inferno_searcx_algorithm_t algorithm);

inferno_searcx_system_t *inferno_searcx_system_create
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    x_case_array_t *initial_solutions, inferno_searcx_algorithm_t algorithm,
    x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_searcx_system_t *search;

  search = malloc(sizeof *search);
  if (search) {
    init_searchey(&search->searchey, algorithm);
    search->searcx_object = search->searchey.create(score_solution, goal,
        context, initial_solutions, log);
    if (!search->searcx_object) {
      x_core_trace("create");
      free(search);
      search = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return search;
}

void inferno_searcx_system_destroy(inferno_searcx_system_t *system)
{
  assert(system);
  system->searchey.destroy(system->searcx_object);
  free(system);
}

x_case_array_t *inferno_searcx_system_get_solutions_copy
(inferno_searcx_system_t *system, unsigned short max_solution_count)
{
  assert(system);

#ifdef INFERNO_BUILD_DEMO
  max_solution_count = inferno_core_min(max_solution_count,
      INFERNO_CORE_DEMO_SOLUTION_COUNT);
#endif

  return system->searchey.get_solutions_copy(system->searcx_object,
      max_solution_count);
}

void inferno_searcx_system_search(inferno_searcx_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;

  gettimeofday(&start_time, NULL);

  do {
    system->searchey.search(system->searcx_object);
  } while (x_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));
}

void init_searchey(inferno_searcx_searchey_t *searchey,
    inferno_searcx_algorithm_t algorithm)
{
  assert(searchey);

  switch (algorithm) {
    default:
    case INFERNO_SEARCX_ALGORITHM_BIOS:
      inferno_bios_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCX_ALGORITHM_COR3:
      inferno_cor3_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCX_ALGORITHM_EOS:
      inferno_eos_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCX_ALGORITHM_GENETIC:
      inferno_genetic_system_init_searchey(searchey);
      break;
  }
}
