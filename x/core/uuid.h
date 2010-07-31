#ifndef cf_x_core_uuid_h
#define cf_x_core_uuid_h

#include "cf/x/core/bool.h"
#include "cf/x/core/message.h"
#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"

struct cf_x_core_uuid_t;
typedef struct cf_x_core_uuid_t cf_x_core_uuid_t;

cf_x_core_bool_t cf_x_core_uuid_add_to_message(void *uuid_object,
    cf_x_core_message_t *message);

int cf_x_core_uuid_compare(void *uuid_a_object, void *uuid_b_object);

void *cf_x_core_uuid_copy(void *uuid_object);

cf_x_core_uuid_t *cf_x_core_uuid_create();

void *cf_x_core_uuid_create_from_message(cf_x_core_message_t *message);

cf_x_core_uuid_t *cf_x_core_uuid_create_from_string(const char *uuid_string);

cf_x_core_uuid_t *cf_x_core_uuid_create_null();

void cf_x_core_uuid_destroy(void *uuid_object);

cf_x_core_bool_t cf_x_core_uuid_equal(void *uuid_a_object, void *uuid_b_object);

char *cf_x_core_uuid_get_as_string(void *uuid_object);

unsigned long cf_x_core_uuid_get_memory_size_bytes(cf_x_core_uuid_t *uuid);

char *cf_x_core_uuid_get_string(void *uuid_object);

unsigned long cf_x_core_uuid_hash(cf_x_core_uuid_t *uuid);

void cf_x_core_uuid_init_objectey(cf_x_core_objectey_t *objectey);

cf_x_core_bool_t cf_x_core_uuid_is_null(cf_x_core_uuid_t *uuid);

unsigned long cf_x_core_uuid_mod(void *uuid_object, unsigned long divisor);

#endif
