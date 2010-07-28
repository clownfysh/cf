#ifndef x_core_object_h
#define x_core_object_h

#include "x/core/bool.h"

#define X_CORE_NO_COMPARE_FUNCTION NULL
#define X_CORE_NO_COPY_FUNCTION NULL
#define X_CORE_NO_DESTROY_FUNCTION NULL

typedef int (*x_core_compare_f)(void *a_object, void *b_object);

typedef int (*x_core_compare_const_f)(const void *a_object,
    const void *b_object);

typedef x_core_bool_t (*x_core_condition_f)(void *object);

typedef void *(*x_core_copy_f)(void *object);

typedef void *(*x_core_create_f)();

typedef void (*x_core_destroy_f)(void *object);

typedef x_core_bool_t (*x_core_equal_f)(void *a_object, void *b_object);

typedef char *(*x_core_get_as_string_f)(void *object);

typedef void *(*x_core_get_object_f)(void *object);

typedef unsigned long (*x_core_hash_f)(void *object);

typedef unsigned long (*x_core_mod_f)(void *object, unsigned long divisor);

void x_core_print(void *object, x_core_get_as_string_f get_as_string);

typedef void (*x_core_set_object_f)(void *object, void *value);

#endif
