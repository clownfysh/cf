#ifndef cf_x_core_unsigned_long_h
#define cf_x_core_unsigned_long_h

#include "cf/x/core/objectey.h"

int cf_x_core_unsigned_long_compare(void *unsigned_long_a_object,
    void *unsigned_long_b_object);

void *cf_x_core_unsigned_long_copy(void *unsigned_long_object);

void cf_x_core_unsigned_long_destroy(void *unsigned_long_object);

cf_x_core_bool_t cf_x_core_unsigned_long_equal(void *unsigned_long_a_object,
    void *unsigned_long_b_object);

char *cf_x_core_unsigned_long_get_as_string(void *unsigned_long_object);

void cf_x_core_unsigned_long_init_objectey(cf_x_core_objectey_t *objectey);

unsigned long cf_x_core_unsigned_long_mod(void *unsigned_long_object,
    unsigned long divisor);

#endif
