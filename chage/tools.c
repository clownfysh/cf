#include "chage/tools.h"
#include "x/core/tools.h"

chage_interval_t chage_determine_interval()
{
  return x_get_microseconds() / CHAGE_INTERVAL_MICROSECONDS;
}
