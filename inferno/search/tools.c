#include "inferno/core/constants.h"
#include "inferno/search/tools.h"

static x_core_bool_t init_box_add_random_actors(inferno_box_system_t *box,
    inferno_box_coordinate_t *dimension_coordinate,
    inferno_core_create_actor_random_f create_actor_random, void *searcx_system,
    x_audit_log_t *log);

static x_core_bool_t init_box_add_actors_from_initial_solutions
(inferno_box_system_t *box, inferno_core_actorey_create_f actor_create,
    void *searcx_system, x_case_array_t *initial_solutions,
    x_audit_log_t *log);

x_case_array_t *inferno_core_create_solutions_from_box(inferno_box_system_t *box,
    unsigned long max_solution_count, inferno_core_get_solution_f get_solution,
    inferno_core_score_solution_f score_solution, inferno_core_goal_t goal,
    x_core_compare_f compare_maximize, x_core_compare_f compare_minimize,
    x_core_copy_f copy_object, void *context, x_audit_log_t *log)
{
  assert(box);
  assert(get_solution);
  assert(score_solution);
  assert(compare_maximize);
  assert(compare_minimize);
  assert(copy_object);
  assert(log);
  x_case_array_t *solutions;
  x_core_compare_f compare_objects;
  unsigned long box_volume;
  unsigned long index;
  x_core_bitarray_t *solution;
  x_core_bitarray_t *solution_copy;
  void *object;
  x_case_array_t *objects;

  if (INFERNO_CORE_GOAL_MAXIMIZE_SCORE == goal) {
    compare_objects = compare_maximize;
  } else {
    compare_objects = compare_minimize;
  }

  box_volume = inferno_box_system_get_volume(box);
  max_solution_count = x_core_min(max_solution_count, box_volume);

  objects = x_case_array_create(box_volume, compare_objects,
      copy_object, X_CORE_NO_DESTROY_FUNCTION);
  if (objects) {
    inferno_box_system_iterate_start(box);
    index = 0;
    while ((object = inferno_box_system_iterate_next(box))) {
      x_case_array_add(objects, index, object);
      index++;
    }

    x_case_array_sort(objects);

    solutions = x_case_array_create(max_solution_count,
        x_core_bitarray_compare, x_core_bitarray_copy,
        x_core_bitarray_destroy);
    if (solutions) {
      x_case_array_iterate_start(objects);
      for (index = 0; index < max_solution_count; index++) {
        object = x_case_array_iterate_next(objects);
        solution = get_solution(object);
        if (solution) {
          solution_copy = x_core_bitarray_copy(solution);
          if (solution_copy) {
            x_case_array_add(solutions, index, solution_copy);
          } else {
            x_audit_log_trace(log, "ih", "x_core_bitarray_copy");
          }
        } else {
          x_audit_log_trace(log, "ih", "x_core_bitarray_copy");
        }
        index++;
      }
    } else {
      x_audit_log_trace(log, "ih", "x_case_array_create");
    }
    x_case_array_destroy(objects);
  } else {
    solutions = NULL;
    x_audit_log_trace(log, "ih", "x_case_array_create");
  }

  return solutions;
}

inferno_box_system_t *inferno_core_create_actor_box(void *searcx_system,
    inferno_box_coordinate_t *dimension_coordinate,
    x_case_array_t *initial_solutions, inferno_core_actorey_t *actorey,
    x_audit_log_t *log)
{
  assert(searcx_system);
  assert(dimension_coordinate);
  assert(actorey);
  assert(log);
  inferno_box_system_t *box;

  box = inferno_box_system_create(dimension_coordinate, actorey->get_box_cell,
      actorey->set_box_cell, actorey->compare, actorey->copy, actorey->destroy,
      log);
  if (box) {
    if (init_box_add_random_actors(box, dimension_coordinate,
            actorey->create_random, searcx_system, log)) {
      if (initial_solutions) {
        if (!init_box_add_actors_from_initial_solutions(box, actorey->create,
                searcx_system, initial_solutions, log)) {
          inferno_box_system_destroy(box);
          box = NULL;
          x_audit_log_trace
            (log, "ih", "init_box_add_actors_from_initial_solutions");
        }
      }
    } else {
      inferno_box_system_destroy(box);
      box = NULL;
      x_audit_log_trace(log, "ih", "init_box_add_random_actors");
    }
  } else {
    x_audit_log_trace(log, "ih", "inferno_box_system_create");
  }

  return box;
}

x_core_bool_t init_box_add_random_actors(inferno_box_system_t *box,
    inferno_box_coordinate_t *dimension_coordinate,
    inferno_core_create_actor_random_f create_actor_random, void *searcx_system,
    x_audit_log_t *log)
{
  assert(box);
  assert(dimension_coordinate);
  assert(create_actor_random);
  assert(searcx_system);
  assert(log);
  x_core_bool_t success;
  inferno_box_coordinate_t coordinate;
  void *actor;

  success = x_core_bool_true;

  for (coordinate.x = 0;
       coordinate.x < dimension_coordinate->x;
       coordinate.x++) {
    for (coordinate.y = 0;
         coordinate.y < dimension_coordinate->y;
         coordinate.y++) {
      for (coordinate.z = 0;
           coordinate.z < dimension_coordinate->z;
           coordinate.z++) {
        actor = create_actor_random(searcx_system);
        if (actor) {
          inferno_box_system_add(box, &coordinate, actor);
        } else {
          success = x_core_bool_false;
          x_audit_log_trace(log, "cor3", "create_actor_random");
        }
      }
    }
  }

  return success;
}

/*  TODO: sort this array ourselves, by solution score  */
x_core_bool_t init_box_add_actors_from_initial_solutions(inferno_box_system_t *box,
    inferno_core_actorey_create_f actor_create, void *searcx_system,
    x_case_array_t *initial_solutions, x_audit_log_t *log)
{
  assert(box);
  assert(initial_solutions);
  assert(log);
  x_core_bool_t success;
  x_core_bitarray_t *bitarray;
  void *actor;

  success = x_core_bool_true;

  x_case_array_iterate_start(initial_solutions);
  while ((bitarray = x_case_array_iterate_next(initial_solutions))) {
    actor = actor_create(searcx_system, bitarray);
    if (actor) {
      inferno_box_system_replace_random(box, actor);
    } else {
      success = x_core_bool_false;
      x_audit_log_trace(log, "cor3", "actor_create");
    }
  }

  return success;
}

unsigned long inferno_core_get_gene_start_address(x_core_bitarray_t *solution,
    unsigned long gene_index)
{
  assert(solution);
  unsigned long address_of_gene_header;
  unsigned long start_address;

  address_of_gene_header = INFERNO_CORE_SOLUTION_BIT_COUNT_2N * gene_index;
  start_address = x_core_bitarray_get_unsigned_short(solution,
      address_of_gene_header) % INFERNO_CORE_SOLUTION_BIT_COUNT;

  return start_address;
}
