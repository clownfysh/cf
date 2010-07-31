#ifndef cf_x_core_object_h
#define cf_x_core_object_h

#include "cf/x/core/bool.h"

#define CF_X_CORE_OBJECT_NO_COMPARE_F NULL
#define CF_X_CORE_OBJECT_NO_COMPARE_EQUAL_F NULL
#define CF_X_CORE_OBJECT_NO_COPY_F NULL
#define CF_X_CORE_OBJECT_NO_DESTROY_F NULL
#define CF_X_CORE_OBJECT_NO_GET_AS_STRING_F NULL
#define CF_X_CORE_OBJECT_NO_MOD_F NULL

typedef int (*cf_x_core_object_compare_f)(void *object_a, void *object_b);

typedef cf_x_core_bool_t (*cf_x_core_object_compare_equal_f)(void *object_a,
    void *object_b);

typedef int (*cf_x_core_object_compare_const_f)(const void *a_object,
    const void *b_object);

typedef cf_x_core_bool_t (*cf_x_core_object_evaluate_condition_f)
(void *object);

typedef void *(*cf_x_core_object_copy_f)(void *object);

typedef void *(*cf_x_core_object_create_f)();

typedef void (*cf_x_core_object_destroy_f)(void *object);

typedef char *(*cf_x_core_object_get_as_string_f)(void *object);

typedef void *(*cf_x_core_object_get_object_f)(void *object);

typedef unsigned long (*cf_x_core_object_hash_f)(void *object);

typedef unsigned long (*cf_x_core_object_mod_f)(void *object,
    unsigned long divisor);

void cf_x_core_object_print(void *object,
    cf_x_core_object_get_as_string_f get_as_string);

typedef void (*cf_x_core_object_set_object_f)(void *object, void *value);

#endif
