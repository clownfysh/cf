#include "x/core/constants.h"
#include "x/core/time.h"
#include "x/core/tools.h"
#include "x/core/types.h"

struct x_core_time_t {
  unsigned long seconds;
};

x_core_time_t *x_core_time_create(unsigned long seconds)
{
  x_core_time_t *time;

  time = malloc(sizeof *time);
  if (time) {
    time->seconds = seconds;
  } else {
    x_core_trace("malloc");
  }

  return time;
}

void x_core_time_destroy(x_core_time_t *time)
{
  assert(time);
  free(time);
}

void x_core_time_get_days_hours_minutes_seconds(x_core_time_t *time,
    x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(time);
  assert(days_hours_minutes_seconds);
  unsigned long seconds_left;

  seconds_left = time->seconds;

  days_hours_minutes_seconds->days = seconds_left / X_CORE_SECONDS_PER_DAY;
  seconds_left -= (days_hours_minutes_seconds->days
      * X_CORE_SECONDS_PER_DAY);

  days_hours_minutes_seconds->hours = seconds_left / X_CORE_SECONDS_PER_HOUR;
  seconds_left -= (days_hours_minutes_seconds->hours
      * X_CORE_SECONDS_PER_HOUR);

  days_hours_minutes_seconds->minutes = seconds_left
    / X_CORE_SECONDS_PER_MINUTE;
  seconds_left -= (days_hours_minutes_seconds->minutes
      * X_CORE_SECONDS_PER_MINUTE);

  days_hours_minutes_seconds->seconds = seconds_left;

}
