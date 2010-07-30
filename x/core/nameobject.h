#ifndef x_core_nameobject_h
#define x_core_nameobject_h

#include "x/core/object.h"
#include "x/core/objectey.h"

struct x_core_nameobject_t;
typedef struct x_core_nameobject_t x_core_nameobject_t;

int x_core_nameobject_compare(void *nameobject_object_a,
    void *nameobject_object_b);

void *x_core_nameobject_copy(void *nameobject_object);

x_core_nameobject_t *x_core_nameobject_create(char *name, void *object,
    x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_get_as_string_f get_as_string);

x_core_nameobject_t *x_core_nameobject_create_decoy(char *name);

void x_core_nameobject_destroy(void *nameobject_object);

void x_core_nameobject_destroy_decoy(void *nameobject_object);

x_core_bool_t x_core_nameobject_equal(void *nameobject_a_object,
    void *nameobject_b_object);

char *x_core_nameobject_get_as_string(void *nameobject_object);

char *x_core_nameobject_get_name(x_core_nameobject_t *nameobject);

void *x_core_nameobject_get_object(x_core_nameobject_t *nameobject);

void x_core_nameobject_init_objectey(x_core_objectey_t *objectey);

unsigned long x_core_nameobject_mod(void *nameobject_object,
    unsigned long divisor);

void x_core_nameobject_print(void *nameobject_object);

#endif
