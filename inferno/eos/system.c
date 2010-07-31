#include "cf/inferno/eos/actor.h"
#include "cf/inferno/eos/constants.h"
#include "cf/inferno/eos/system.h"

struct cf_inferno_eos_system_t {
  cf_inferno_box_system_t *box;
  cf_inferno_core_score_solution_f score_solution;
  cf_inferno_core_goal_t goal;
  cf_inferno_core_actorey_t actorey;
  void *context;
  cf_x_audit_log_t *log;
};

void *cf_inferno_eos_system_create(cf_inferno_core_score_solution_f score_solution,
    cf_inferno_core_goal_t goal, void *context, cf_x_case_array_t *initial_solutions,
    cf_x_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  cf_inferno_eos_system_t *system;
  cf_inferno_box_coordinate_t dimension_coordinate;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->goal = goal;
    system->context = context;
    system->log = log;
    cf_x_core_seed_random_witx_time();
    cf_inferno_box_coordinate_init_witx_xyz(&dimension_coordinate,
        CF_INFERNO_EOS_BOX_DIMENSION, CF_INFERNO_EOS_BOX_DIMENSION, 1);
    cf_inferno_eos_actor_init_actorey(&system->actorey);
    system->box = cf_inferno_core_create_actor_box(system, &dimension_coordinate,
        initial_solutions, &system->actorey, log);
    if (!system->box) {
      cf_x_audit_log_trace(log, "bios", "inferno_core_create_actor_box");
      free(system);
      system = NULL;
    }

  } else {
    cf_x_audit_log_trace(log, "eos", "malloc");
  }

  return system;
}

void cf_inferno_eos_system_destroy(void *system_object)
{
  assert(system_object);
  cf_inferno_eos_system_t *system;

  system = system_object;

  cf_inferno_box_system_destroy(system->box);
  free(system);
}

cf_inferno_box_system_t *cf_inferno_eos_system_get_box(cf_inferno_eos_system_t *system)
{
  return system->box;
}

void *cf_inferno_eos_system_get_context(cf_inferno_eos_system_t *system)
{
  return system->context;
}

cf_inferno_core_goal_t cf_inferno_eos_system_get_goal(cf_inferno_eos_system_t *system)
{
  return system->goal;
}

cf_x_audit_log_t *cf_inferno_eos_system_get_log(cf_inferno_eos_system_t *system)
{
  return system->log;
}

cf_inferno_core_score_solution_f cf_inferno_eos_system_get_score_solution
(cf_inferno_eos_system_t *system)
{
  return system->score_solution;
}

cf_x_case_array_t *cf_inferno_eos_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  assert(system_object);
  cf_inferno_eos_system_t *system;
  cf_x_case_array_t *solutions;

  system = system_object;

  solutions = cf_inferno_core_create_solutions_from_box(system->box,
      max_solution_count, cf_inferno_eos_actor_get_solution, system->score_solution,
      system->goal, cf_inferno_eos_actor_compare_maximize,
      cf_inferno_eos_actor_compare_minimize, cf_inferno_eos_actor_copy, system->context,
      system->log);
  if (!solutions) {
    cf_x_audit_log_trace(system->log, "eos", "create_solutions_from_box");
  }

  return solutions;
}

void cf_inferno_eos_system_init_searchey(cf_inferno_searcx_searchey_t *searchey)
{
  cf_inferno_searcx_searchey_init(searchey, cf_inferno_eos_system_create,
      cf_inferno_eos_system_destroy, cf_inferno_eos_system_get_solutions_copy,
      cf_inferno_eos_system_search);
}

void cf_inferno_eos_system_search(void *system_object)
{
  assert(system_object);
 cf_inferno_eos_system_t *system;
  cf_inferno_eos_actor_t *actor;
  unsigned long i;

  system = system_object;

  for (i = 0; i < 32; i++) {
    actor = cf_inferno_box_system_find_random(system->box);
    cf_inferno_eos_actor_act(actor);
  }
}
