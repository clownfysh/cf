#ifndef cf_x_core_string_h
#define cf_x_core_string_h

#include "cf/x/core/object.h"
#include "cf/x/core/iobject.h"

typedef char * cf_x_core_string_t;

cf_x_core_bool_t cf_x_core_string_add_to_message(void *string_object,
    cf_x_core_message_t *message);

int cf_x_core_string_compare(void *string_a_object, void *string_b_object);

cf_x_core_bool_t cf_x_core_string_compare_equal(void *string_a_object,
    void *string_b_object);

void *cf_x_core_string_copy(void *string_object);

void *cf_x_core_string_create_from_message(cf_x_core_message_t *message);

void cf_x_core_string_destroy(void *string_object);

char *cf_x_core_string_get_as_string(void *string_object);

unsigned long cf_x_core_string_hash(void *string_object);

void cf_x_core_string_init_iobject(cf_x_core_iobject_t *iobject);

unsigned long cf_x_core_string_mod(void *string_object, unsigned long divisor);

void cf_x_core_string_print(void *string_object);

cf_x_core_string_t cf_x_core_string_substring(cf_x_core_string_t string,
    unsigned long start, unsigned long length);

#endif
