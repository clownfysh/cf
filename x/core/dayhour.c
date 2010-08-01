#include "cf/x/core/dayhour.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/types.h"

char *create_string_from_dayhour(cf_x_core_dayhour_t *dayhour)
{
  assert(dayhour);
  char *string;
  int asprintf_result;

  asprintf_result = asprintf(&string, "%02i:%02i:%02i:%02i", dayhour->days,
      dayhour->hours, dayhour->minutes, dayhour->seconds);
  if (-1 == asprintf_result) {
    string = NULL;
    cf_x_core_trace("asprintf");
  }

  return string;
}
