#ifndef x_core_objects_h
#define x_core_objects_h

#include "x/core/objectey.h"

struct x_core_objects_t {
  x_core_objectey_t long_objectey;
  x_core_objectey_t pair_objectey;
  x_core_objectey_t string_objectey;
  x_core_objectey_t unsigned_long_objectey;
  x_core_objectey_t void_objectey;
};
typedef struct x_core_objects_t x_core_objects_t;

void x_core_objects_free(x_core_objects_t *objects);

void x_core_objects_init(x_core_objects_t *objects);

#endif
