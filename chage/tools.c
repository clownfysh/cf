#include "cf/chage/tools.h"
#include "cf/x/core/tools.h"

cf_chage_interval_t cf_chage_determine_interval()
{
  return cf_x_get_microseconds() / CF_CHAGE_INTERVAL_MICROSECONDS;
}
