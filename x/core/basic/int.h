#ifndef cf_x_core_basic_int_h
#define cf_x_core_basic_int_h

#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"

struct cf_x_core_basic_int_t;
typedef struct cf_x_core_basic_int_t cf_x_core_basic_int_t;

int cf_x_core_basic_int_compare(void *int_object_a, void *int_object_b);

void *cf_x_core_basic_int_copy(void *int_object);

cf_x_core_basic_int_t *cf_x_core_basic_int_create();

void cf_x_core_basic_int_destroy(void *int_object);

char *cf_x_core_basic_int_get_as_string(void *int_object);

void cf_x_core_basic_int_init_objectey(cf_x_core_objectey_t *objectey);

#endif
