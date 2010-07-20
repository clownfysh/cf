#ifndef x_core_double_h
#define x_core_double_h

#include "x/core/object.h"

x_core_bool_t x_core_double_add_to_message(void *double_object,
    x_core_message_t *message);

int x_core_double_compare(void *double_object_a, void *double_object_b);

void *x_core_double_copy(void *double_object);

void *x_core_double_create_from_message(x_core_message_t *message);

void x_core_double_destroy(void *double_object);

#endif
