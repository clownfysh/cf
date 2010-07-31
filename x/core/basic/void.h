#ifndef cf_x_net_void_h
#define cf_x_net_void_h

#include "cf/x/core/objectey.h"

int cf_x_core_basic_void_compare(void *object_a, void *object_b);

void *cf_x_core_basic_void_copy(void *object);

void cf_x_core_basic_void_destroy(void *object);

char *cf_x_core_basic_void_get_as_string(void *object);

void cf_x_core_basic_void_init_objectey(cf_x_core_objectey_t *objectey);

#endif
