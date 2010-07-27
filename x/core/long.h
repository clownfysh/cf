#ifndef x_core_long_h
#define x_core_long_h

#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/types.h"

x_core_bool_t x_core_long_add_to_message(void *long_object,
    x_core_message_t *message);

int x_core_long_compare(void *long_object_a, void *long_object_b);

void *x_core_long_copy(void *long_object);

void *x_core_long_create_from_message(x_core_message_t *message);

void x_core_long_destroy(void *long_object);

x_core_bool_t x_core_long_equal(void *long_a_object, void *long_b_object);

char *x_core_long_get_as_string(void *long_object);

void x_core_long_init_objectey(x_core_objectey_t *objectey);

unsigned long x_core_long_mod(void *long_object, unsigned long modulus);

void x_core_long_print(void *long_object);

#endif
