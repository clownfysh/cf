#ifndef cf_sage_core_goal_h
#define cf_sage_core_goal_h

/*!
 * the goal of a search or optimization.  specifies whether the search should
 * work at minimizing or maximizing solutions' scores.
 */
enum cf_sage_core_goal_t {
  CF_SAGE_CORE_GOAL_MAXIMIZE_SCORE,
  CF_SAGE_CORE_GOAL_MINIMIZE_SCORE
};
typedef enum cf_sage_core_goal_t cf_sage_core_goal_t;

#endif
