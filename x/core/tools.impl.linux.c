#include "cf/x/core/tools.h"

unsigned long cf_x_get_microseconds()
{
  struct timeval now;
  unsigned long microseconds;

  if (gettimeofday(&now, NULL) != -1) {
    microseconds = now.tv_usec;
  } else {
    cf_x_trace("gettimeofday");
    microseconds = 0;
  }

  return microseconds;
}
