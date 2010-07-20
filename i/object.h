#ifndef i_object_h
#define i_object_h

#include "i/tools.h"

#define I_NO_COMPARE_FUNCTION NULL
#define I_NO_COPY_FUNCTION NULL
#define I_NO_DESTROY_FUNCTION NULL

typedef int (*i_compare_f)(void *a_object, void *b_object);

typedef int (*i_compare_const_f)(const void *a_object, const void *b_object);

typedef i_bool_t (*i_condition_f)(void *object);

typedef void *(*i_copy_f)(void *object);

typedef void *(*i_create_f)();

typedef void (*i_destroy_f)(void *object);

typedef i_bool_t (*i_equal_f)(void *object);

typedef char *(*i_get_as_string_f)(void *object);

typedef void *(*i_get_object_f)(void *object);

typedef unsigned long (*i_hash_f)(void *object);

typedef unsigned long (*i_mod_f)(void *object);

void i_print(void *object, i_get_as_string_f get_as_string);

typedef void (*i_set_object_f)(void *object, void *value);

#endif
