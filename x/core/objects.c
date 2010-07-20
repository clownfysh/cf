#include "x/core/standard.h"
#include "x/core/long.h"
#include "x/core/objects.h"
#include "x/core/pair.h"
#include "x/core/string.h"
#include "x/core/unsigned_long.h"
#include "x/core/void.h"

void x_core_objects_free(x_core_objects_t *objects) {}

void x_core_objects_init(x_core_objects_t *objects)
{
  assert(objects);

  x_core_long_init_objectey(&objects->long_objectey);
  x_core_pair_init_objectey(&objects->pair_objectey);
  x_core_string_init_objectey(&objects->string_objectey);
  x_core_unsigned_long_init_objectey(&objects->unsigned_long_objectey);
  x_core_void_init_objectey(&objects->void_objectey);
}
