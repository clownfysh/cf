#include "i/tools.h"
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

unsigned long i_get_microseconds()
{
  struct timeval now;
  unsigned long microseconds;

  if (gettimeofday(&now, NULL) != -1) {
    microseconds = now.tv_usec;
  } else {
    i_trace("gettimeofday");
    microseconds = 0;
  }

  return microseconds;
}
