#ifndef cf_inferno_morpinferno_system_h
#define cf_inferno_morpinferno_system_h

#include "cf/inferno/core/goal.h"
#include "cf/inferno/search/algorithm.h"
#include "cf/inferno/search/tools.h"
#include "cf/x/core/tools.h"

struct cf_inferno_morpinferno_system_t;
typedef struct cf_inferno_morpinferno_system_t cf_inferno_morpinferno_system_t;

/*!
 * create an inhesion::morph system.  the resulting pointer is a context
 * variable that other morph methods need.  when you're finished using the
 * search, call cf_inferno_morpinferno_system_destroy() to release resources the morph is
 * using.
 *
 * \param score_solution a function with the signature specified by
 *        cf_inferno_core_score_solution_f.  this will be used to direct the morph
 *        through possibility space.
 * \param goal either CF_INFERNO_CORE_GOAL_MAXIMIZE_SCORE or
 *        CF_INFERNO_CORE_GOAL_MINIMIZE_SCORE, telling the search to find solutions
 *        that either maximize or minimize solutions' scores.
 * \param context pointer to a domain-specific context.  if your
 *        cf_inferno_core_score_solution_f needs to interact with your larger
 *        application context to do its work, pass in a pointer to that context
 *        here, or pass NULL otherwise.
 * \param initial_solutions array of solutions, each as an cf_x_core_bitarray_t *.
 *        the morph will be initialized with some of these solutions.
 *        inhesion morphs may or may not use all of the solutions you provide.
 *        put solutions you favor earlier in the array.  inhesion's use of
 *        these solutions is stochastic...don't count on multiple identical
 *        morph function calls producing identical results.
 * \param log pointer to an cf_x_core_log_t where trace and log statements will
 *        be sent.
 *
 * \return pointer to an cf_inferno_morpinferno_system_t, or NULL if the call was
 *         unsuccessful.
 */
cf_inferno_morpinferno_system_t *cf_inferno_morpinferno_system_create
(cf_inferno_core_score_solution_f score_solution, cf_inferno_core_goal_t goal, void *context,
    cf_x_case_array_t *initial_solutions, cf_x_core_log_t *log);

/*!
 * destroy an inhesion::morph system.  release resources the morph is using.
 *
 * \param system pointer to an inhesion::morph system.
 */
void cf_inferno_morpinferno_system_destroy(cf_inferno_morpinferno_system_t *system);

/*!
 * get copies of the solutions in, or configurations of, the morph.  this
 * doesn't affect the contents of the system.  when you want to get morph
 * solutions the morph has found, to use in modifying your domain or to store
 * for later use as initial solutions in a morph, call this function.  inhesion
 * places better solutions earlier in the returned array.
 *
 * \param system pointer to an inhesion::morph system
 * \param max_solution_count maximum number of solutions to return.  in the
 *        demo version, the number of solutions returned is limited.  we know
 *        you can hack the library to make it fully functional.  we're not the
 *        karma police.  if you use this, license it.
 *
 * \return an array of pointers to cf_inferno_core_solution_t variables, which the
 *         caller of this function is responsible for freeing.
 */
cf_x_case_array_t *cf_inferno_morpinferno_system_get_solutions_copy
(cf_inferno_morpinferno_system_t *system, unsigned short max_solution_count);

/*!
 * search for solutions.  use the cf_inferno_core_score_solution_f passed to
 * cf_inferno_morpinferno_system_create() to guide the morph.  call this method once, or
 * call it multiple times in a loop.  if called multiple times on the same
 * system, the morph picks up where it left off each time you call
 * cf_inferno_morpinferno_system_search().
 *
 * \param system pointer to an inhesion::morph system
 * \param max_wall_time_microseconds try to take no more than
 *        max_wall_time_microseconds to advance the morph (there will be some
 *        minimum amount of time it takes for a call to this method to
 *        complete...above that, the max_wall_time_microseconds are used to
 *        pause the search).
 */
void cf_inferno_morpinferno_system_search(cf_inferno_morpinferno_system_t *system,
    unsigned long max_wall_time_microseconds);

#endif
