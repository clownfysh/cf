#ifndef cf_x_core_nameobject_h
#define cf_x_core_nameobject_h

#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"

struct cf_x_core_nameobject_t;
typedef struct cf_x_core_nameobject_t cf_x_core_nameobject_t;

int cf_x_core_nameobject_compare(void *nameobject_object_a,
    void *nameobject_object_b);

void *cf_x_core_nameobject_copy(void *nameobject_object);

cf_x_core_nameobject_t *cf_x_core_nameobject_create(char *name, void *object,
    cf_x_core_copy_f copy, cf_x_core_destroy_f destroy,
    cf_x_core_get_as_string_f get_as_string);

cf_x_core_nameobject_t *cf_x_core_nameobject_create_decoy(char *name);

void cf_x_core_nameobject_destroy(void *nameobject_object);

void cf_x_core_nameobject_destroy_decoy(void *nameobject_object);

cf_x_core_bool_t cf_x_core_nameobject_equal(void *nameobject_a_object,
    void *nameobject_b_object);

char *cf_x_core_nameobject_get_as_string(void *nameobject_object);

char *cf_x_core_nameobject_get_name(cf_x_core_nameobject_t *nameobject);

void *cf_x_core_nameobject_get_object(cf_x_core_nameobject_t *nameobject);

void cf_x_core_nameobject_init_objectey(cf_x_core_objectey_t *objectey);

unsigned long cf_x_core_nameobject_mod(void *nameobject_object,
    unsigned long divisor);

void cf_x_core_nameobject_print(void *nameobject_object);

#endif
