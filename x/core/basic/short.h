#ifndef cf_x_core_basic_short_h
#define cf_x_core_basic_short_h

#include "cf/x/core/object.h"
#include "cf/x/core/types.h"

cf_x_core_bool_t cf_x_core_basic_short_add_to_message(void *short_object,
    cf_x_core_message_t *message);

int cf_x_core_basic_short_compare(void *short_a_object, void *short_b_object);

void *cf_x_core_basic_short_copy(void *short_object);

void *cf_x_core_basic_short_take_from_message(cf_x_core_message_t *message);

void cf_x_core_basic_short_destroy(void *short_object);

void cf_x_core_basic_short_print(void *short_object);

#endif
