#include "x/core/run.h"
#include "x/core/tools.h"

struct x_core_run_t {
  x_core_run_style_t run_style;

  x_core_bool_t goal_reached;

  unsigned long iterations;
  unsigned long max_iterations;

  time_t start_time;
  unsigned long wall_time_expiration_seconds;
};

static void reset(x_core_run_t *run);

void x_core_run_consider_goal_reached(x_core_run_t *run)
{
  run->goal_reached = x_core_bool_true;
}

x_core_run_t *x_core_run_create(x_core_run_style_t run_style)
{
  x_core_run_t *run;

  run = malloc(sizeof *run);
  if (run) {
    run->run_style = run_style;
    reset(run);
  } else {
    x_core_trace("malloc");
  }

  return run;
}

void x_core_run_destroy(x_core_run_t *run)
{
  assert(run);
  free(run);
}

void x_core_run_increment_iterations(x_core_run_t *run)
{
  run->iterations++;
}

x_core_bool_t x_core_run_is_complete(x_core_run_t *run)
{
  assert(run);
  x_core_bool_t is_complete;
  time_t current_time;

  switch (run->run_style) {

    case X_CORE_RUN_STYLE_ITERATIONS:
      if (run->iterations >= run->max_iterations) {
        is_complete = x_core_bool_true;
      } else {
        is_complete = x_core_bool_false;
      }
      break;

    case X_CORE_RUN_STYLE_WALL_TIME:
      time(&current_time);
      if ((current_time - run->start_time)
          >= run->wall_time_expiration_seconds) {
        is_complete = x_core_bool_true;
      } else {
        is_complete = x_core_bool_false;
      }
      break;

    case X_CORE_RUN_STYLE_GOAL_REACHED:
      is_complete = run->goal_reached;
      break;

    case X_CORE_RUN_STYLE_INFINITE:
      is_complete = x_core_bool_false;
      break;

    case X_CORE_RUN_STYLE_UNKNOWN:
    default:
      is_complete = x_core_bool_false;
      break;
  }

  return is_complete;
}

void x_core_run_reset(x_core_run_t *run)
{
  reset(run);
}

void x_core_run_set_max_iterations(x_core_run_t *run,
    unsigned long max_iterations)
{
  run->max_iterations = max_iterations;
}

void x_core_run_set_wall_time_expiration_seconds(x_core_run_t *run,
    unsigned long seconds)
{
  run->wall_time_expiration_seconds = seconds;
}

void reset(x_core_run_t *run)
{
  assert(run);

  run->goal_reached = x_core_bool_false;
  run->iterations = 0;
  time(&run->start_time);
}
