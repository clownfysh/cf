#ifndef x_core_short_h
#define x_core_short_h

#include "x/core/object.h"
#include "x/core/types.h"

x_core_bool_t x_core_short_add_to_message(void *short_object,
    x_core_message_t *message);

int x_core_short_compare(void *short_a_object, void *short_b_object);

void *x_core_short_copy(void *short_object);

void *x_core_short_take_from_message(x_core_message_t *message);

void x_core_short_destroy(void *short_object);

void x_core_short_print(void *short_object);

#endif
