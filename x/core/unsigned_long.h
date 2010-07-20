#ifndef x_core_unsigned_long_h
#define x_core_unsigned_long_h

#include "x/core/objectey.h"

int x_core_unsigned_long_compare(void *unsigned_long_object_a,
    void *unsigned_long_object_b);

void *x_core_unsigned_long_copy(void *unsigned_long_object);

void x_core_unsigned_long_destroy(void *unsigned_long_object);

char *x_core_unsigned_long_get_as_string(void *unsigned_long_object);

void x_core_unsigned_long_init_objectey(x_core_objectey_t *objectey);

#endif
