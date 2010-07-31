#include "cf/sage/bios/system.h"
#include "cf/sage/cor3/system.h"
#include "cf/sage/eos/system.h"
#include "cf/sage/genetic/system.h"
#include "cf/sage/search/searchey.h"
#include "cf/sage/search/system.h"
#include "cf/x/core/tools.h"

struct cf_sage_searcx_system_t {
  cf_sage_searcx_searchey_t searchey;
  void *searcx_object;
};

static void init_searchey(cf_sage_searcx_searchey_t *searchey,
    cf_sage_searcx_algorithm_t algorithm);

cf_sage_searcx_system_t *cf_sage_searcx_system_create
(cf_sage_core_score_solution_f score_solution, cf_sage_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_sage_searcx_algorithm_t algorithm,
    cf_x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  cf_sage_searcx_system_t *search;

  search = malloc(sizeof *search);
  if (search) {
    init_searchey(&search->searchey, algorithm);
    search->searcx_object = search->searchey.create(score_solution, goal,
        context, initial_solutions, log);
    if (!search->searcx_object) {
      cf_x_core_trace("create");
      free(search);
      search = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return search;
}

void cf_sage_searcx_system_destroy(cf_sage_searcx_system_t *system)
{
  assert(system);
  system->searchey.destroy(system->searcx_object);
  free(system);
}

cf_x_case_array_t *cf_sage_searcx_system_get_solutions_copy
(cf_sage_searcx_system_t *system, unsigned short max_solution_count)
{
  assert(system);

#ifdef CF_SAGE_BUILD_DEMO
  max_solution_count = cf_sage_core_min(max_solution_count,
      CF_SAGE_CORE_DEMO_SOLUTION_COUNT);
#endif

  return system->searchey.get_solutions_copy(system->searcx_object,
      max_solution_count);
}

void cf_sage_searcx_system_search(cf_sage_searcx_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;

  gettimeofday(&start_time, NULL);

  do {
    system->searchey.search(system->searcx_object);
  } while (cf_x_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));
}

void init_searchey(cf_sage_searcx_searchey_t *searchey,
    cf_sage_searcx_algorithm_t algorithm)
{
  assert(searchey);

  switch (algorithm) {
    default:
    case CF_SAGE_SEARCX_ALGORITHM_BIOS:
      cf_sage_bios_system_init_searchey(searchey);
      break;
    case CF_SAGE_SEARCX_ALGORITHM_COR3:
      cf_sage_cor3_system_init_searchey(searchey);
      break;
    case CF_SAGE_SEARCX_ALGORITHM_EOS:
      cf_sage_eos_system_init_searchey(searchey);
      break;
    case CF_SAGE_SEARCX_ALGORITHM_GENETIC:
      cf_sage_genetic_system_init_searchey(searchey);
      break;
  }
}
