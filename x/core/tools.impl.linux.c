#include "x/core/tools.h"

unsigned long x_get_microseconds()
{
  struct timeval now;
  unsigned long microseconds;

  if (gettimeofday(&now, NULL) != -1) {
    microseconds = now.tv_usec;
  } else {
    x_trace("gettimeofday");
    microseconds = 0;
  }

  return microseconds;
}
