#ifndef inferno_core_goal_h
#define inferno_core_goal_h

/*!
 * the goal of a search or optimization.  specifies whether the search should
 * work at minimizing or maximizing solutions' scores.
 */
enum inferno_core_goal_t {
  INFERNO_CORE_GOAL_MAXIMIZE_SCORE,
  INFERNO_CORE_GOAL_MINIMIZE_SCORE
};
typedef enum inferno_core_goal_t inferno_core_goal_t;

#endif
