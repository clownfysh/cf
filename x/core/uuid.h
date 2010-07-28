#ifndef x_core_uuid_h
#define x_core_uuid_h

#include "x/core/bool.h"
#include "x/core/message.h"
#include "x/core/object.h"

struct x_core_uuid_t;
typedef struct x_core_uuid_t x_core_uuid_t;

x_core_bool_t x_core_uuid_add_to_message(void *uuid_object,
    x_core_message_t *message);

int x_core_uuid_compare(void *uuid_object_a, void *uuid_object_b);

void *x_core_uuid_copy(void *uuid_object);

x_core_uuid_t *x_core_uuid_create();

void *x_core_uuid_create_from_message(x_core_message_t *message);

x_core_uuid_t *x_core_uuid_create_from_string(const char *uuid_string);

x_core_uuid_t *x_core_uuid_create_null();

void x_core_uuid_destroy(void *uuid_object);

x_core_bool_t x_core_uuid_equal(void *uuid_a_object, void *uuid_b_object);

unsigned long x_core_uuid_get_memory_size_bytes(x_core_uuid_t *uuid);

char *x_core_uuid_get_string(void *uuid_object);

unsigned long x_core_uuid_hash(x_core_uuid_t *uuid);

x_core_bool_t x_core_uuid_is_null(x_core_uuid_t *uuid);

unsigned long x_core_uuid_mod(void *uuid_object, unsigned long divisor);

#endif
