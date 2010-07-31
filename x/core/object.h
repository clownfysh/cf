#ifndef cf_x_core_object_h
#define cf_x_core_object_h

#include "cf/x/core/bool.h"

#define CF_X_CORE_NO_COMPARE_FUNCTION NULL
#define CF_X_CORE_NO_COPY_FUNCTION NULL
#define CF_X_CORE_NO_DESTROY_FUNCTION NULL
#define CF_X_CORE_NO_EQUAL_FUNCTION NULL
#define CF_X_CORE_NO_GET_AS_STRING_FUNCTION NULL
#define CF_X_CORE_NO_MOD_FUNCTION NULL

typedef int (*cf_x_core_compare_f)(void *a_object, void *b_object);

typedef int (*cf_x_core_compare_const_f)(const void *a_object,
    const void *b_object);

typedef cf_x_core_bool_t (*cf_x_core_condition_f)(void *object);

typedef void *(*cf_x_core_copy_f)(void *object);

typedef void *(*cf_x_core_create_f)();

typedef void (*cf_x_core_destroy_f)(void *object);

typedef cf_x_core_bool_t (*cf_x_core_equal_f)(void *a_object, void *b_object);

typedef char *(*cf_x_core_get_as_string_f)(void *object);

typedef void *(*cf_x_core_get_object_f)(void *object);

typedef unsigned long (*cf_x_core_hash_f)(void *object);

typedef unsigned long (*cf_x_core_mod_f)(void *object, unsigned long divisor);

void cf_x_core_print(void *object, cf_x_core_get_as_string_f get_as_string);

typedef void (*cf_x_core_set_object_f)(void *object, void *value);

#endif
