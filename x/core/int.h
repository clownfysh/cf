#ifndef x_core_int_h
#define x_core_int_h

#include "x/core/object.h"
#include "x/core/objectey.h"

struct x_core_int_t;
typedef struct x_core_int_t x_core_int_t;

int x_core_int_compare(void *int_object_a, void *int_object_b);

void *x_core_int_copy(void *int_object);

x_core_int_t *x_core_int_create();

void x_core_int_destroy(void *int_object);

char *x_core_int_get_as_string(void *int_object);

void x_core_int_init_objectey(x_core_objectey_t *objectey);

#endif
