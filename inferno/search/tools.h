#ifndef cf_inferno_search_tools_h
#define cf_inferno_search_tools_h

#include "cf/x/case/array.h"
#include "cf/inferno/box/system.h"
#include "cf/inferno/core/iactor.h"
#include "cf/inferno/core/goal.h"

typedef void *(*cf_inferno_core_create_actor_f)(void *search_system,
    cf_x_core_bitarray_t *solution);

typedef void *(*cf_inferno_core_create_actor_random_f)(void *search_system);

typedef cf_x_core_bitarray_t *(*cf_inferno_core_get_solution_f)(void *object);

/*!
 * prototype for a score-solution function.  various inhesion systems search
 * for solutions to a problem.  this function scores a particular solution, to
 * direct the search.  define a function with this signature for use with some
 * inhesion systems.  systems that find solutions can either minimize or
 * maximize solution scores.
 *
 * \param context a pointer to your domain's custom context.  if you use some
 *        external context in the calculation of a solution's score, that
 *        context is available via this pointer.  you pass in the context to
 *        various inhesion system functions, then it is made available to your
 *        score-solution function as the value of this parameter.
 * \param solution the solution data.
 * \param score the solution's score.  implement your score_solution_f to set
 *        the value of the associated double, based on the solution data.
 *
 * \return success of the call
 */
typedef cf_x_core_bool_t (*cf_inferno_core_score_solution_f)(void *context,
    cf_x_core_bitarray_t *solution, double *score);

cf_inferno_box_system_t *cf_inferno_core_create_actor_box(void *search_system,
    cf_inferno_box_coordinate_t *dimension_coordinate,
    cf_x_case_array_t *initial_solutions, cf_inferno_core_iactor_t *iactor,
    cf_x_core_log_t *log);

cf_x_case_array_t *cf_inferno_core_create_solutions_from_box(cf_inferno_box_system_t *box,
    unsigned long max_solution_count, cf_inferno_core_get_solution_f get_solution,
    cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal,
    cf_x_core_object_compare_f compare_maximize, cf_x_core_object_compare_f compare_minimize,
    cf_x_core_object_copy_f copy_object, void *context, cf_x_core_log_t *log);

unsigned long cf_inferno_core_get_gene_start_address(cf_x_core_bitarray_t *solution,
    unsigned long gene_index);

#endif
