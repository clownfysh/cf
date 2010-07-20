#include "chage/system.h"
#include "trf/tools.h"

static const char* copyright = "^~% Copyright 2010-2012 Matthew Temple.  All "
  "Rights Reserved.";

int main(int argc, char *argv[])
{
  chage_system_t *chage;
  int result = 0;

  printf("%s\n", copyright);
  chage = chage_system_create();
  if (chage) {
    if (trf_platform_init()) {
      while (!trf_pause_is_requested()) {
        if (!trf_platform_input()) {
          i_trace("platform_input");
          result = -4;
        }
        if (!trf_platform_output()) {
          i_trace("trf_platform_output");
          result = -5;
        }
        chage_system_spark(chage);
      }
      /*
        TODO: save state to disk
      */
      if (!trf_platform_free()) {
        i_trace("trf_platform_free");
        result = -3;
      }
    } else {
      i_trace("trf_platform_init");
      result = -2;
    }
    chage_system_destroy(chage);
  } else {
    i_trace("chage_system_create");
    result = -1;
  }

  return result;
}
