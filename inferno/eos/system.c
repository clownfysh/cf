#include "inferno/eos/actor.h"
#include "inferno/eos/constants.h"
#include "inferno/eos/system.h"

struct inferno_eos_system_t {
  inferno_box_system_t *box;
  inferno_core_score_solution_f score_solution;
  inferno_core_goal_t goal;
  inferno_core_actorey_t actorey;
  void *context;
  h_audit_log_t *log;
};

void *inferno_eos_system_create(inferno_core_score_solution_f score_solution,
    inferno_core_goal_t goal, void *context, h_container_array_t *initial_solutions,
    h_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  inferno_eos_system_t *system;
  inferno_box_coordinate_t dimension_coordinate;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->goal = goal;
    system->context = context;
    system->log = log;
    h_core_seed_random_with_time();
    inferno_box_coordinate_init_with_xyz(&dimension_coordinate,
        INFERNO_EOS_BOX_DIMENSION, INFERNO_EOS_BOX_DIMENSION, 1);
    inferno_eos_actor_init_actorey(&system->actorey);
    system->box = inferno_core_create_actor_box(system, &dimension_coordinate,
        initial_solutions, &system->actorey, log);
    if (!system->box) {
      h_audit_log_trace(log, "bios", "inferno_core_create_actor_box");
      free(system);
      system = NULL;
    }

  } else {
    h_audit_log_trace(log, "eos", "malloc");
  }

  return system;
}

void inferno_eos_system_destroy(void *system_object)
{
  assert(system_object);
  inferno_eos_system_t *system;

  system = system_object;

  inferno_box_system_destroy(system->box);
  free(system);
}

inferno_box_system_t *inferno_eos_system_get_box(inferno_eos_system_t *system)
{
  return system->box;
}

void *inferno_eos_system_get_context(inferno_eos_system_t *system)
{
  return system->context;
}

inferno_core_goal_t inferno_eos_system_get_goal(inferno_eos_system_t *system)
{
  return system->goal;
}

h_audit_log_t *inferno_eos_system_get_log(inferno_eos_system_t *system)
{
  return system->log;
}

inferno_core_score_solution_f inferno_eos_system_get_score_solution
(inferno_eos_system_t *system)
{
  return system->score_solution;
}

h_container_array_t *inferno_eos_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  assert(system_object);
  inferno_eos_system_t *system;
  h_container_array_t *solutions;

  system = system_object;

  solutions = inferno_core_create_solutions_from_box(system->box,
      max_solution_count, inferno_eos_actor_get_solution, system->score_solution,
      system->goal, inferno_eos_actor_compare_maximize,
      inferno_eos_actor_compare_minimize, inferno_eos_actor_copy, system->context,
      system->log);
  if (!solutions) {
    h_audit_log_trace(system->log, "eos", "create_solutions_from_box");
  }

  return solutions;
}

void inferno_eos_system_init_searchey(inferno_search_searchey_t *searchey)
{
  inferno_search_searchey_init(searchey, inferno_eos_system_create,
      inferno_eos_system_destroy, inferno_eos_system_get_solutions_copy,
      inferno_eos_system_search);
}

void inferno_eos_system_search(void *system_object)
{
  assert(system_object);
 inferno_eos_system_t *system;
  inferno_eos_actor_t *actor;
  unsigned long i;

  system = system_object;

  for (i = 0; i < 32; i++) {
    actor = inferno_box_system_find_random(system->box);
    inferno_eos_actor_act(actor);
  }
}
