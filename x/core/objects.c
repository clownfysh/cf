#include "cf/x/core/bool.h"
#include "cf/x/core/objects.h"
#include "cf/x/core/pair.h"
#include "cf/x/core/standard.h"
#include "cf/x/core/string.h"
#include "cf/x/core/tools.h"
#include "cf/x/core/uuid.h"
#include "cf/x/core/basic/long.h"
#include "cf/x/core/basic/unsigned_long.h"
#include "cf/x/core/basic/void.h"

static cf_x_core_bool_t initialized = cf_x_core_bool_false;

void cf_x_core_objects_init()
{
  if (!initialized) {
    cf_x_core_basic_long_init_iobject(&cf_x_core_objects.long_iobject);
    cf_x_core_pair_init_iobject(&cf_x_core_objects.pair_iobject);
    cf_x_core_string_init_iobject(&cf_x_core_objects.string_iobject);
    cf_x_core_basic_unsigned_long_init_iobject
      (&cf_x_core_objects.unsigned_long_iobject);
    cf_x_core_uuid_init_iobject(&cf_x_core_objects.uuid_iobject);
    cf_x_core_basic_void_init_iobject(&cf_x_core_objects.void_iobject);
    initialized = cf_x_core_bool_true;
  } else {
    cf_x_trace("fyi :: cf_x_core_objects_init called more than once");
  }
}
