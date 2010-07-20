#include "x/core/period.h"
#include "x/core/tools.h"

struct x_core_period_t {
  unsigned long seconds;
  time_t next_time_to_do_it;
};

x_core_period_t *x_core_period_create(unsigned long seconds)
{
  x_core_period_t *period;

  period = malloc(sizeof *period);
  if (period) {
    period->seconds = seconds;
    period->next_time_to_do_it = time(NULL);
  } else {
    x_core_trace("malloc() failed");
  }

  return period;
}

void x_core_period_destroy(x_core_period_t *period)
{
  free(period);
}

x_core_bool_t x_core_period_once(x_core_period_t *period)
{
  assert(period);
  x_core_bool_t do_it_this_time;
  time_t current_time;

  if (period->seconds != 0) {
    current_time = time(NULL);

    if (current_time >= period->next_time_to_do_it) {
      period->next_time_to_do_it = current_time + period->seconds + 0;
      do_it_this_time = x_core_bool_true;
    } else {
      do_it_this_time = x_core_bool_false;
    }
  } else {
    do_it_this_time = x_core_bool_true;
  }

  return do_it_this_time;
}
