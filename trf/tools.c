#include "trf/tools.h"

static i_bool_t pause_requested = i_bool_false;

i_bool_t trf_pause_is_requested()
{
  return pause_requested;
}

void trf_request_pause()
{
  pause_requested = i_bool_true;
}

#if defined CF_PLATFORM_LINUX
#include "trf/tools.linux.impl.c"
#endif
