#include "cf/x/sync/run.h"
#include "cf/x/core/tools.h"

struct cf_x_sync_run_t {
  cf_x_sync_run_style_t run_style;

  cf_x_core_bool_t goal_reached;

  unsigned long iterations;
  unsigned long max_iterations;

  time_t start_time;
  unsigned long wall_time_expiration_seconds;
};

static void reset(cf_x_sync_run_t *run);

void cf_x_sync_run_consider_goal_reached(cf_x_sync_run_t *run)
{
  run->goal_reached = cf_x_core_bool_true;
}

cf_x_sync_run_t *cf_x_sync_run_create(cf_x_sync_run_style_t run_style)
{
  cf_x_sync_run_t *run;

  run = malloc(sizeof *run);
  if (run) {
    run->run_style = run_style;
    reset(run);
  } else {
    cf_x_core_trace("malloc");
  }

  return run;
}

void cf_x_sync_run_destroy(cf_x_sync_run_t *run)
{
  assert(run);
  free(run);
}

void cf_x_sync_run_increment_iterations(cf_x_sync_run_t *run)
{
  run->iterations++;
}

cf_x_core_bool_t cf_x_sync_run_is_complete(cf_x_sync_run_t *run)
{
  assert(run);
  cf_x_core_bool_t is_complete;
  time_t current_time;

  switch (run->run_style) {

    case CF_X_SYNC_RUN_STYLE_ITERATIONS:
      if (run->iterations >= run->max_iterations) {
        is_complete = cf_x_core_bool_true;
      } else {
        is_complete = cf_x_core_bool_false;
      }
      break;

    case CF_X_SYNC_RUN_STYLE_WALL_TIME:
      time(&current_time);
      if ((current_time - run->start_time)
          >= run->wall_time_expiration_seconds) {
        is_complete = cf_x_core_bool_true;
      } else {
        is_complete = cf_x_core_bool_false;
      }
      break;

    case CF_X_SYNC_RUN_STYLE_GOAL_REACHED:
      is_complete = run->goal_reached;
      break;

    case CF_X_SYNC_RUN_STYLE_INFINITE:
      is_complete = cf_x_core_bool_false;
      break;

    case CF_X_SYNC_RUN_STYLE_UNKNOWN:
    default:
      is_complete = cf_x_core_bool_false;
      break;
  }

  return is_complete;
}

void cf_x_sync_run_reset(cf_x_sync_run_t *run)
{
  reset(run);
}

void cf_x_sync_run_set_max_iterations(cf_x_sync_run_t *run,
    unsigned long max_iterations)
{
  run->max_iterations = max_iterations;
}

void cf_x_sync_run_set_wall_time_expiration_seconds(cf_x_sync_run_t *run,
    unsigned long seconds)
{
  run->wall_time_expiration_seconds = seconds;
}

void reset(cf_x_sync_run_t *run)
{
  assert(run);

  run->goal_reached = cf_x_core_bool_false;
  run->iterations = 0;
  time(&run->start_time);
}
