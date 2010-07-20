#ifndef inferno_search_tools_h
#define inferno_search_tools_h

#include "h/container/array.h"
#include "inferno/box/system.h"
#include "inferno/core/actorey.h"
#include "inferno/core/goal.h"

typedef void *(*inferno_core_create_actor_f)(void *search_system,
    h_core_bitarray_t *solution);

typedef void *(*inferno_core_create_actor_random_f)(void *search_system);

typedef h_core_bitarray_t *(*inferno_core_get_solution_f)(void *object);

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
typedef h_core_bool_t (*inferno_core_score_solution_f)(void *context,
    h_core_bitarray_t *solution, double *score);

inferno_box_system_t *inferno_core_create_actor_box(void *search_system,
    inferno_box_coordinate_t *dimension_coordinate,
    h_container_array_t *initial_solutions, inferno_core_actorey_t *actorey,
    h_audit_log_t *log);

h_container_array_t *inferno_core_create_solutions_from_box(inferno_box_system_t *box,
    unsigned long max_solution_count, inferno_core_get_solution_f get_solution,
    inferno_core_score_solution_f score_solution, inferno_core_goal_t goal,
    h_core_compare_f compare_maximize, h_core_compare_f compare_minimize,
    h_core_copy_f copy_object, void *context, h_audit_log_t *log);

unsigned long inferno_core_get_gene_start_address(h_core_bitarray_t *solution,
    unsigned long gene_index);

#endif
