#ifndef cf_x_core_basic_long_h
#define cf_x_core_basic_long_h

#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"
#include "cf/x/core/types.h"

cf_x_core_bool_t cf_x_core_basic_long_add_to_message(void *long_object,
    cf_x_core_message_t *message);

int cf_x_core_basic_long_compare(void *long_a_object, void *long_b_object);

void *cf_x_core_basic_long_copy(void *long_object);

void *cf_x_core_basic_long_create_from_message(cf_x_core_message_t *message);

void cf_x_core_basic_long_destroy(void *long_object);

cf_x_core_bool_t cf_x_core_basic_long_compare_equal(void *long_a_object,
    void *long_b_object);

char *cf_x_core_basic_long_get_as_string(void *long_object);

void cf_x_core_basic_long_init_objectey(cf_x_core_objectey_t *objectey);

unsigned long cf_x_core_basic_long_mod(void *long_object,
    unsigned long divisor);

void cf_x_core_basic_long_print(void *long_object);

#endif
