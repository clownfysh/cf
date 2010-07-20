#ifndef x_net_void_h
#define x_net_void_h

#include "x/core/objectey.h"

int x_core_void_compare(void *object_a, void *object_b);

void *x_core_void_copy(void *object);

void x_core_void_destroy(void *object);

char *x_core_void_get_as_string(void *object);

void x_core_void_init_objectey(x_core_objectey_t *objectey);

#endif
