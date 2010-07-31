#include "cf/x/sync/period.h"
#include "cf/x/core/tools.h"

struct cf_x_sync_period_t {
  unsigned long seconds;
  time_t next_time_to_do_it;
};

cf_x_sync_period_t *cf_x_sync_period_create(unsigned long seconds)
{
  cf_x_sync_period_t *period;

  period = malloc(sizeof *period);
  if (period) {
    period->seconds = seconds;
    period->next_time_to_do_it = time(NULL);
  } else {
    cf_x_core_trace("malloc() failed");
  }

  return period;
}

void cf_x_sync_period_destroy(cf_x_sync_period_t *period)
{
  free(period);
}

cf_x_core_bool_t cf_x_sync_period_once(cf_x_sync_period_t *period)
{
  assert(period);
  cf_x_core_bool_t do_it_this_time;
  time_t current_time;

  if (period->seconds != 0) {
    current_time = time(NULL);

    if (current_time >= period->next_time_to_do_it) {
      period->next_time_to_do_it = current_time + period->seconds + 0;
      do_it_this_time = cf_x_core_bool_true;
    } else {
      do_it_this_time = cf_x_core_bool_false;
    }
  } else {
    do_it_this_time = cf_x_core_bool_true;
  }

  return do_it_this_time;
}
