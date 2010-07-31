#ifndef cf_x_sync_run_h
#define cf_x_sync_run_h

#include "cf/x/core/types.h"

enum cf_x_sync_run_style_t {
  CF_X_SYNC_RUN_STYLE_UNKNOWN,
  CF_X_SYNC_RUN_STYLE_ITERATIONS,
  CF_X_SYNC_RUN_STYLE_WALL_TIME,
  CF_X_SYNC_RUN_STYLE_GOAL_REACHED,
  CF_X_SYNC_RUN_STYLE_INFINITE
};
typedef enum cf_x_sync_run_style_t cf_x_sync_run_style_t;

struct cf_x_sync_run_t;
typedef struct cf_x_sync_run_t cf_x_sync_run_t;

void cf_x_sync_run_consider_goal_reached(cf_x_sync_run_t *run);

cf_x_sync_run_t *cf_x_sync_run_create(cf_x_sync_run_style_t run_style);

void cf_x_sync_run_destroy(cf_x_sync_run_t *run);

void cf_x_sync_run_increment_iterations(cf_x_sync_run_t *run);

cf_x_core_bool_t cf_x_sync_run_is_complete(cf_x_sync_run_t *run);

void cf_x_sync_run_reset(cf_x_sync_run_t *run);

void cf_x_sync_run_set_max_iterations(cf_x_sync_run_t *run,
    unsigned long max_iterations);

void cf_x_sync_run_set_wall_time_expiration_seconds(cf_x_sync_run_t *run,
    unsigned long seconds);

#endif
