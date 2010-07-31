#include "cf/x/core/constants.h"
#include "cf/x/core/time.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/types.h"

struct cf_x_core_time_t {
  unsigned long seconds;
};

cf_x_core_time_t *cf_x_core_time_create(unsigned long seconds)
{
  cf_x_core_time_t *time;

  time = malloc(sizeof *time);
  if (time) {
    time->seconds = seconds;
  } else {
    cf_x_core_trace("malloc");
  }

  return time;
}

void cf_x_core_time_destroy(cf_x_core_time_t *time)
{
  assert(time);
  free(time);
}

void cf_x_core_time_get_days_hours_minutes_seconds(cf_x_core_time_t *time,
    cf_x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(time);
  assert(days_hours_minutes_seconds);
  unsigned long seconds_left;

  seconds_left = time->seconds;

  days_hours_minutes_seconds->days = seconds_left / CF_X_CORE_SECONDS_PER_DAY;
  seconds_left -= (days_hours_minutes_seconds->days
      * CF_X_CORE_SECONDS_PER_DAY);

  days_hours_minutes_seconds->hours = seconds_left / CF_X_CORE_SECONDS_PER_HOUR;
  seconds_left -= (days_hours_minutes_seconds->hours
      * CF_X_CORE_SECONDS_PER_HOUR);

  days_hours_minutes_seconds->minutes = seconds_left
    / CF_X_CORE_SECONDS_PER_MINUTE;
  seconds_left -= (days_hours_minutes_seconds->minutes
      * CF_X_CORE_SECONDS_PER_MINUTE);

  days_hours_minutes_seconds->seconds = seconds_left;

}
