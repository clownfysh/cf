#ifndef cf_x_core_objects_h
#define cf_x_core_objects_h

#include "cf/x/core/objectey.h"

struct cf_x_core_objects_t {
  cf_x_core_objectey_t long_objectey;
  cf_x_core_objectey_t pair_objectey;
  cf_x_core_objectey_t string_objectey;
  cf_x_core_objectey_t unsigned_long_objectey;
  cf_x_core_objectey_t void_objectey;
};
typedef struct cf_x_core_objects_t cf_x_core_objects_t;

void cf_x_core_objects_free(cf_x_core_objects_t *objects);

void cf_x_core_objects_init(cf_x_core_objects_t *objects);

#endif
