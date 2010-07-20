#include "x/core/constants.h"
#include "x/core/days_hours_minutes_seconds.h"
#include "x/core/time.h"
#include "x/core/tools.h"

int main(int argc, char *argv[])
{
  x_core_time_t *time;
  x_core_days_hours_minutes_seconds_t days_hours_minutes_seconds;
  unsigned long seconds;
  char *time_string;

  for (seconds = 1000000; seconds < 1000010; seconds++) {
    time = x_core_time_create(seconds);
    if (time) {
      x_core_time_get_days_hours_minutes_seconds(time,
          &days_hours_minutes_seconds);
      time_string = create_string_from_days_hours_minutes_seconds
        (&days_hours_minutes_seconds);
      if (time_string) {
        printf("%lu => %s\n", seconds, time_string);
        assert(seconds == days_hours_minutes_seconds.seconds
            + (days_hours_minutes_seconds.minutes * X_CORE_SECONDS_PER_MINUTE)
            + (days_hours_minutes_seconds.hours * X_CORE_SECONDS_PER_HOUR)
            + (days_hours_minutes_seconds.days * X_CORE_SECONDS_PER_DAY));
        free(time_string);
      }
      x_core_time_destroy(time);
    } else {
      x_core_trace("x_core_time_create");
    }
  }

  return 0;
}
