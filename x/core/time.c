#include "cf/x/core/time.h"
#include "cf/x/core/tools.h"

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

void cf_x_core_time_get_dayhour(cf_x_core_time_t *time,
    cf_x_core_dayhour_t *dayhour)
{
  assert(time);
  assert(dayhour);
  unsigned long seconds_left;

  seconds_left = time->seconds;

  dayhour->days = seconds_left / CF_X_CORE_SECONDS_PER_DAY;
  seconds_left -= (dayhour->days * CF_X_CORE_SECONDS_PER_DAY);

  dayhour->hours = seconds_left / CF_X_CORE_SECONDS_PER_HOUR;
  seconds_left -= (dayhour->hours * CF_X_CORE_SECONDS_PER_HOUR);

  dayhour->minutes = seconds_left / CF_X_CORE_SECONDS_PER_MINUTE;
  seconds_left -= (dayhour->minutes * CF_X_CORE_SECONDS_PER_MINUTE);

  dayhour->seconds = seconds_left;

}
