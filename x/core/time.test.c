#include "cf/x/core/dayhour.h"
#include "cf/x/core/time.h"
#include "cf/x/core/tools.h"

int main(int argc, char *argv[])
{
  cf_x_core_time_t *time;
  cf_x_core_dayhour_t dayhour;
  unsigned long seconds;
  char *time_string;

  for (seconds = 1000000; seconds < 1000010; seconds++) {
    time = cf_x_core_time_create(seconds);
    if (time) {
      cf_x_core_time_get_dayhour(time, &dayhour);
      time_string = create_string_from_dayhour(&dayhour);
      if (time_string) {
        printf("%lu => %s\n", seconds, time_string);
        assert(seconds == dayhour.seconds
            + (dayhour.minutes * CF_X_CORE_SECONDS_PER_MINUTE)
            + (dayhour.hours * CF_X_CORE_SECONDS_PER_HOUR)
            + (dayhour.days * CF_X_CORE_SECONDS_PER_DAY));
        free(time_string);
      }
      cf_x_core_time_destroy(time);
    } else {
      cf_x_core_trace("x_core_time_create");
    }
  }

  return 0;
}
