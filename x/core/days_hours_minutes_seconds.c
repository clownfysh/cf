#include "cf/x/core/days_hours_minutes_seconds.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/types.h"

char *create_string_from_days_hours_minutes_seconds
(cf_x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(days_hours_minutes_seconds);
  char *string;
  int asprintf_result;

  asprintf_result = asprintf(&string, "%02i:%02i:%02i:%02i",
      days_hours_minutes_seconds->days, days_hours_minutes_seconds->hours,
      days_hours_minutes_seconds->minutes,
      days_hours_minutes_seconds->seconds);
  if (-1 == asprintf_result) {
    string = NULL;
    cf_x_core_trace("asprintf");
  }

  return string;
}
