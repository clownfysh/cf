#include "cf/x/core/standard.h"
#include "cf/x/core/long.h"
#include "cf/x/core/objects.h"
#include "cf/x/core/pair.h"
#include "cf/x/core/string.h"
#include "cf/x/core/unsigned_long.h"
#include "cf/x/core/void.h"

void cf_x_core_objects_free(cf_x_core_objects_t *objects) {}

void cf_x_core_objects_init(cf_x_core_objects_t *objects)
{
  assert(objects);

  cf_x_core_long_init_objectey(&objects->long_objectey);
  cf_x_core_pair_init_objectey(&objects->pair_objectey);
  cf_x_core_string_init_objectey(&objects->string_objectey);
  cf_x_core_unsigned_long_init_objectey(&objects->unsigned_long_objectey);
  cf_x_core_void_init_objectey(&objects->void_objectey);
}
