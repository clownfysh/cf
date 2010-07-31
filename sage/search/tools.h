#ifndef cf_sage_searcx_tools_h
#define cf_sage_searcx_tools_h

#include "cf/x/case/array.h"
#include "cf/sage/box/system.h"
#include "cf/sage/core/actorey.h"
#include "cf/sage/core/goal.h"

typedef void *(*cf_sage_core_create_actor_f)(void *searcx_system,
    cf_x_core_bitarray_t *solution);

typedef void *(*cf_sage_core_create_actor_random_f)(void *searcx_system);

typedef cf_x_core_bitarray_t *(*cf_sage_core_get_solution_f)(void *object);

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
typedef cf_x_core_bool_t (*cf_sage_core_score_solution_f)(void *context,
    cf_x_core_bitarray_t *solution, double *score);

cf_sage_box_system_t *cf_sage_core_create_actor_box(void *searcx_system,
    cf_sage_box_coordinate_t *dimension_coordinate,
    cf_x_case_array_t *initial_solutions, cf_sage_core_actorey_t *actorey,
    cf_x_audit_log_t *log);

cf_x_case_array_t *cf_sage_core_create_solutions_from_box(cf_sage_box_system_t *box,
    unsigned long max_solution_count, cf_sage_core_get_solution_f get_solution,
    cf_sage_core_score_solution_f score_solution, cf_sage_core_goal_t goal,
    cf_x_core_compare_f compare_maximize, cf_x_core_compare_f compare_minimize,
    cf_x_core_copy_f copy_object, void *context, cf_x_audit_log_t *log);

unsigned long cf_sage_core_get_gene_start_address(cf_x_core_bitarray_t *solution,
    unsigned long gene_index);

#endif
