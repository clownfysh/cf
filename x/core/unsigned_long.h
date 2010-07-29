#ifndef x_core_unsigned_long_h
#define x_core_unsigned_long_h

#include "x/core/objectey.h"

int x_core_unsigned_long_compare(void *unsigned_long_a_object,
    void *unsigned_long_b_object);

void *x_core_unsigned_long_copy(void *unsigned_long_object);

void x_core_unsigned_long_destroy(void *unsigned_long_object);

x_core_bool_t x_core_unsigned_long_equal(void *unsigned_long_a_object,
    void *unsigned_long_b_object);

char *x_core_unsigned_long_get_as_string(void *unsigned_long_object);

void x_core_unsigned_long_init_objectey(x_core_objectey_t *objectey);

unsigned long x_core_unsigned_long_mod(void *unsigned_long_object,
    unsigned long divisor);

#endif
