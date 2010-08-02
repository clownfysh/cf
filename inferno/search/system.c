#include "cf/inferno/bios/system.h"
#include "cf/inferno/cor3/system.h"
#include "cf/inferno/eos/system.h"
#include "cf/inferno/genetic/system.h"
#include "cf/inferno/search/isearch.h"
#include "cf/inferno/search/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_search_system_t {
  cf_inferno_search_isearch_t isearch;
  void *search_object;
};

static void init_isearch(cf_inferno_search_isearch_t *isearch,
    cf_inferno_search_algorithm_t algorithm);

cf_inferno_search_system_t *cf_inferno_search_system_create
(cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_inferno_search_algorithm_t algorithm,
    cf_x_core_log_t *log)
{
  assert(score_solution);
  assert(log);
  cf_inferno_search_system_t *search;

  search = malloc(sizeof *search);
  if (search) {
    init_isearch(&search->isearch, algorithm);
    search->search_object = search->isearch.create(score_solution, goal,
        context, initial_solutions, log);
    if (!search->search_object) {
      cf_x_core_trace("create");
      free(search);
      search = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return search;
}

void cf_inferno_search_system_destroy(cf_inferno_search_system_t *system)
{
  assert(system);
  system->isearch.destroy(system->search_object);
  free(system);
}

cf_x_case_array_t *cf_inferno_search_system_get_solutions_copy
(cf_inferno_search_system_t *system, unsigned short max_solution_count)
{
  assert(system);

#ifdef CF_INFERNO_BUILD_DEMO
  max_solution_count = cf_inferno_core_min(max_solution_count,
      CF_INFERNO_CORE_DEMO_SOLUTION_COUNT);
#endif

  return system->isearch.get_solutions_copy(system->search_object,
      max_solution_count);
}

void cf_inferno_search_system_search(cf_inferno_search_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;

  gettimeofday(&start_time, NULL);

  do {
    system->isearch.search(system->search_object);
  } while (cf_x_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));
}

void init_isearch(cf_inferno_search_isearch_t *isearch,
    cf_inferno_search_algorithm_t algorithm)
{
  assert(isearch);

  switch (algorithm) {
    default:
    case CF_INFERNO_SEARCH_ALGORITHM_BIOS:
      cf_inferno_bios_system_init_isearch(isearch);
      break;
    case CF_INFERNO_SEARCH_ALGORITHM_COR3:
      cf_inferno_cor3_system_init_isearch(isearch);
      break;
    case CF_INFERNO_SEARCH_ALGORITHM_EOS:
      cf_inferno_eos_system_init_isearch(isearch);
      break;
    case CF_INFERNO_SEARCH_ALGORITHM_GENETIC:
      cf_inferno_genetic_system_init_isearch(isearch);
      break;
  }
}
