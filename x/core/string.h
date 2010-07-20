#ifndef x_core_string_h
#define x_core_string_h

#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/types.h"

typedef char * x_core_string_t;

x_core_bool_t x_core_string_add_to_message(void *string_object,
    x_core_message_t *message);

int x_core_string_compare(void *string_object_a, void *string_object_b);

void *x_core_string_copy(void *string_object);

void *x_core_string_create_from_message(x_core_message_t *message);

void x_core_string_destroy(void *string_object);

char *x_core_string_get_as_string(void *string_object);

unsigned long x_core_string_hash(void *string_object);

void x_core_string_init_objectey(x_core_objectey_t *objectey);

void x_core_string_print(void *string_object);

x_core_string_t x_core_string_substring(x_core_string_t string,
    unsigned long start, unsigned long length);

#endif
