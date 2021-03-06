#ifndef cf_x_core_nameobject_h
#define cf_x_core_nameobject_h

#include "cf/x/core/object.h"
#include "cf/x/core/iobject.h"

struct cf_x_core_nameobject_t;
typedef struct cf_x_core_nameobject_t cf_x_core_nameobject_t;

int cf_x_core_nameobject_compare(void *nameobject_object_a,
    void *nameobject_object_b);

cf_x_core_bool_t cf_x_core_nameobject_compare_equal(void *nameobject_a_object,
    void *nameobject_b_object);

void *cf_x_core_nameobject_copy(void *nameobject_object);

cf_x_core_nameobject_t *cf_x_core_nameobject_create(char *name, void *object,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_object_get_as_string_f get_as_string);

cf_x_core_nameobject_t *cf_x_core_nameobject_create_decoy(char *name);

void cf_x_core_nameobject_destroy(void *nameobject_object);

void cf_x_core_nameobject_destroy_decoy(void *nameobject_object);

char *cf_x_core_nameobject_get_as_string(void *nameobject_object);

char *cf_x_core_nameobject_get_name(cf_x_core_nameobject_t *nameobject);

void *cf_x_core_nameobject_get_object(cf_x_core_nameobject_t *nameobject);

void cf_x_core_nameobject_init_iobject(cf_x_core_iobject_t *iobject);

unsigned long cf_x_core_nameobject_mod(void *nameobject_object,
    unsigned long divisor);

void cf_x_core_nameobject_print(void *nameobject_object);

#endif
