#ifndef x_core_unsigned_short_h
#define x_core_unsigned_short_h

#include "x/core/object.h"
#include "x/core/types.h"

int x_core_unsigned_short_compare(void *unsigned_short_object_a,
    void *unsigned_short_object_b);

void *x_core_unsigned_short_copy(void *unsigned_short_object);

void x_core_unsigned_short_destroy(void *unsigned_short_object);

void x_core_unsigned_short_print(void *unsigned_short_object);

#endif
