#include "chage/tools.h"
#include "i/tools.h"

chage_interval_t chage_determine_interval()
{
  return i_get_microseconds() / CHAGE_INTERVAL_MICROSECONDS;
}
