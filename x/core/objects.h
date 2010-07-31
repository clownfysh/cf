#ifndef cf_x_core_objects_h
#define cf_x_core_objects_h

#include "cf/x/core/iobject.h"

struct cf_x_core_objects_t {
  cf_x_core_iobject_t long_iobject;
  cf_x_core_iobject_t pair_iobject;
  cf_x_core_iobject_t string_iobject;
  cf_x_core_iobject_t unsigned_long_iobject;
  cf_x_core_iobject_t uuid_iobject;
  cf_x_core_iobject_t void_iobject;
};
typedef struct cf_x_core_objects_t cf_x_core_objects_t;

cf_x_core_objects_t cf_x_core_objects;

void cf_x_core_objects_init();

#endif
