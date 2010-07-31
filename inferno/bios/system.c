#include "inferno/bios/actor.h"
#include "inferno/bios/constants.h"
#include "inferno/bios/declarations.h"
#include "inferno/bios/system.h"
#include "inferno/core/constants.h"

struct inferno_bios_system_t {
  inferno_box_system_t *box;
  inferno_core_score_solution_f score_solution;
  inferno_core_goal_t goal;
  inferno_core_actorey_t actorey;
  void *context;
  x_audit_log_t *log;
};

void *inferno_bios_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context, x_case_array_t *initial_solutions,
    x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_bios_system_t *system;
  inferno_box_coordinate_t dimension_coordinate;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->goal = goal;
    system->context = context;
    system->log = log;
    x_core_seed_random_witx_time();
    inferno_box_coordinate_init_witx_xyz(&dimension_coordinate,
        INFERNO_BIOS_BOX_DIMENSION, INFERNO_BIOS_BOX_DIMENSION, 1);
    inferno_bios_actor_init_actorey(&system->actorey);
    system->box = inferno_core_create_actor_box(system, &dimension_coordinate,
        initial_solutions, &system->actorey, log);
    if (!system->box) {
      x_audit_log_trace(log, "bios", "inferno_core_create_actor_box");
      free(system);
      system = NULL;
    }

  } else {
    x_audit_log_trace(log, "bios", "malloc");
  }

  return system;
}

void inferno_bios_system_destroy(void *system_object)
{
  assert(system_object);
  inferno_bios_system_t *system;

  system = system_object;

  inferno_box_system_destroy(system->box);
  free(system);
}

inferno_box_system_t *inferno_bios_system_get_box(inferno_bios_system_t *system)
{
  return system->box;
}

void *inferno_bios_system_get_context(inferno_bios_system_t *system)
{
  return system->context;
}

inferno_core_goal_t inferno_bios_system_get_goal(inferno_bios_system_t *system)
{
  return system->goal;
}

x_audit_log_t *inferno_bios_system_get_log(inferno_bios_system_t *system)
{
  return system->log;
}

inferno_core_score_solution_f inferno_bios_system_get_score_solution
(inferno_bios_system_t *system)
{
  return system->score_solution;
}

x_case_array_t *inferno_bios_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  assert(system_object);
  inferno_bios_system_t *system;
  x_case_array_t *solutions;

  system = system_object;

  solutions = inferno_core_create_solutions_from_box(system->box,
      max_solution_count, inferno_bios_actor_get_solution, system->score_solution,
      system->goal, inferno_bios_actor_compare_maximize,
      inferno_bios_actor_compare_minimize, inferno_bios_actor_copy, system->context,
      system->log);
  if (!solutions) {
    x_audit_log_trace(system->log, "bios", "create_solutions_from_box");
  }

  return solutions;
}

void inferno_bios_system_init_searchey(inferno_searcx_searchey_t *searchey)
{
  inferno_searcx_searchey_init(searchey, inferno_bios_system_create,
      inferno_bios_system_destroy, inferno_bios_system_get_solutions_copy,
      inferno_bios_system_search);
}

void inferno_bios_system_search(void *system_object)
{
  assert(system_object);
  inferno_bios_system_t *system;
  inferno_bios_actor_t *actor;
  unsigned long i;

  system = system_object;

  for (i = 0; i < 32; i++) {
    actor = inferno_box_system_find_random(system->box);
    inferno_bios_actor_act(actor);
  }
}
