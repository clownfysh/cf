#ifndef cf_x_core_unsigned_short_h
#define cf_x_core_unsigned_short_h

#include "cf/x/core/object.h"
#include "cf/x/core/types.h"

int cf_x_core_unsigned_short_compare(void *unsigned_short_a_object,
    void *unsigned_short_b_object);

void *cf_x_core_unsigned_short_copy(void *unsigned_short_object);

void cf_x_core_unsigned_short_destroy(void *unsigned_short_object);

void cf_x_core_unsigned_short_print(void *unsigned_short_object);

#endif
