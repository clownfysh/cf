#ifndef x_core_run_h
#define x_core_run_h

#include "x/core/types.h"

enum x_core_run_style_t {
  X_CORE_RUN_STYLE_UNKNOWN,
  X_CORE_RUN_STYLE_ITERATIONS,
  X_CORE_RUN_STYLE_WALL_TIME,
  X_CORE_RUN_STYLE_GOAL_REACHED,
  X_CORE_RUN_STYLE_INFINITE
};
typedef enum x_core_run_style_t x_core_run_style_t;

struct x_core_run_t;
typedef struct x_core_run_t x_core_run_t;

void x_core_run_consider_goal_reached(x_core_run_t *run);

x_core_run_t *x_core_run_create(x_core_run_style_t run_style);

void x_core_run_destroy(x_core_run_t *run);

void x_core_run_increment_iterations(x_core_run_t *run);

x_core_bool_t x_core_run_is_complete(x_core_run_t *run);

void x_core_run_reset(x_core_run_t *run);

void x_core_run_set_max_iterations(x_core_run_t *run,
    unsigned long max_iterations);

void x_core_run_set_wall_time_expiration_seconds(x_core_run_t *run,
    unsigned long seconds);

#endif
