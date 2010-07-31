#include "cf/x/core/constants.h"
#include "cf/x/core/days_hours_minutes_seconds.h"
#include "cf/x/core/time.h"
#include "cf/x/core/tools.h"

int main(int argc, char *argv[])
{
  cf_x_core_time_t *time;
  cf_x_core_days_hours_minutes_seconds_t days_hours_minutes_seconds;
  unsigned long seconds;
  char *time_string;

  for (seconds = 1000000; seconds < 1000010; seconds++) {
    time = cf_x_core_time_create(seconds);
    if (time) {
      cf_x_core_time_get_days_hours_minutes_seconds(time,
          &days_hours_minutes_seconds);
      time_string = create_string_from_days_hours_minutes_seconds
        (&days_hours_minutes_seconds);
      if (time_string) {
        printf("%lu => %s\n", seconds, time_string);
        assert(seconds == days_hours_minutes_seconds.seconds
            + (days_hours_minutes_seconds.minutes * CF_X_CORE_SECONDS_PER_MINUTE)
            + (days_hours_minutes_seconds.hours * CF_X_CORE_SECONDS_PER_HOUR)
            + (days_hours_minutes_seconds.days * CF_X_CORE_SECONDS_PER_DAY));
        free(time_string);
      }
      cf_x_core_time_destroy(time);
    } else {
      cf_x_core_trace("x_core_time_create");
    }
  }

  return 0;
}
