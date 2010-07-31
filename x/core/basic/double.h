#ifndef cf_x_core_basic_double_h
#define cf_x_core_basic_double_h

#include "cf/x/core/object.h"

cf_x_core_bool_t cf_x_core_basic_double_add_to_message(void *double_object,
    cf_x_core_message_t *message);

int cf_x_core_basic_double_compare(void *double_object_a,
    void *double_object_b);

void *cf_x_core_basic_double_copy(void *double_object);

void *cf_x_core_basic_double_create_from_message(cf_x_core_message_t *message);

void cf_x_core_basic_double_destroy(void *double_object);

#endif
