#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "trf/tools.h"

static void handle_signal(int signal);

static void handle_signal(int signal)
{
  switch (signal) {
    case SIGINT:
      trf_request_pause();
      break;
    default:
      break;
  }
}

i_bool_t trf_platform_free()
{
  return i_bool_true;
}

unsigned long trf_platform_get_microseconds()
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

i_bool_t trf_platform_init()
{
  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  return i_bool_true;
}

i_bool_t trf_platform_input()
{
  return i_bool_true;
}

i_bool_t trf_platform_output()
{
  return i_bool_true;
}
