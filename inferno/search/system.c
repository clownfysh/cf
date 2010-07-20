#include "inferno/bios/system.h"
#include "inferno/cor3/system.h"
#include "inferno/eos/system.h"
#include "inferno/genetic/system.h"
#include "inferno/search/searchey.h"
#include "inferno/search/system.h"
#include "h/core/tools.h"

struct inferno_search_system_t {
  inferno_search_searchey_t searchey;
  void *search_object;
};

static void init_searchey(inferno_search_searchey_t *searchey,
    inferno_search_algorithm_t algorithm);

inferno_search_system_t *inferno_search_system_create
(inferno_core_score_solution_f score_solution, inferno_core_goal_t goal, void *context,
    h_container_array_t *initial_solutions, inferno_search_algorithm_t algorithm,
    h_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_search_system_t *search;

  search = malloc(sizeof *search);
  if (search) {
    init_searchey(&search->searchey, algorithm);
    search->search_object = search->searchey.create(score_solution, goal,
        context, initial_solutions, log);
    if (!search->search_object) {
      h_core_trace("create");
      free(search);
      search = NULL;
    }
  } else {
    h_core_trace("malloc");
  }

  return search;
}

void inferno_search_system_destroy(inferno_search_system_t *system)
{
  assert(system);
  system->searchey.destroy(system->search_object);
  free(system);
}

h_container_array_t *inferno_search_system_get_solutions_copy
(inferno_search_system_t *system, unsigned short max_solution_count)
{
  assert(system);

#ifdef INFERNO_BUILD_DEMO
  max_solution_count = inferno_core_min(max_solution_count,
      INFERNO_CORE_DEMO_SOLUTION_COUNT);
#endif

  return system->searchey.get_solutions_copy(system->search_object,
      max_solution_count);
}

void inferno_search_system_search(inferno_search_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;

  gettimeofday(&start_time, NULL);

  do {
    system->searchey.search(system->search_object);
  } while (h_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));
}

void init_searchey(inferno_search_searchey_t *searchey,
    inferno_search_algorithm_t algorithm)
{
  assert(searchey);

  switch (algorithm) {
    default:
    case INFERNO_SEARCH_ALGORITHM_BIOS:
      inferno_bios_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCH_ALGORITHM_COR3:
      inferno_cor3_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCH_ALGORITHM_EOS:
      inferno_eos_system_init_searchey(searchey);
      break;
    case INFERNO_SEARCH_ALGORITHM_GENETIC:
      inferno_genetic_system_init_searchey(searchey);
      break;
  }
}
