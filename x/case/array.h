#ifndef x_case_array_h
#define x_case_array_h

#include "x/case/declarations.h"
#include "x/core/bool.h"
#include "x/core/message.h"
#include "x/core/object.h"

void x_case_array_add(x_case_array_t *array, unsigned long index,
    void *object);

x_core_bool_t x_case_array_add_to_message(x_case_array_t *array,
    x_core_message_t *message,
    x_core_message_add_to_message_f add_to_message);

void x_case_array_clear(x_case_array_t *array);

int x_case_array_compare(void *array_object_a, void *array_object_b);

void *x_case_array_copy(void *array_object);

unsigned long x_case_array_count_non_null_objects
(x_case_array_t *array);

x_case_array_t *x_case_array_create(unsigned long initial_size,
    x_core_compare_f compare, x_core_copy_f copy, x_core_destroy_f destroy);

x_case_array_t *x_case_array_create_from_message
(x_core_compare_f compare_object, x_core_copy_f copy,
    x_core_destroy_f destroy, x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message);

x_case_array_t *x_case_array_create_strings_from_string
(char *string, char *separators);

void x_case_array_destroy(void *array_object);

void x_case_array_expunge_set(x_case_array_t *array,
    x_case_set_t *expunge_these);

void x_case_array_expunge_set_inverse(x_case_array_t *array,
    x_case_set_t *expunge_all_but_these);

void *x_case_array_find(x_case_array_t *array,
    unsigned long index);

void *x_case_array_find_random(x_case_array_t *array);

void *x_case_array_find_first(x_case_array_t *array);

void *x_case_array_find_last(x_case_array_t *array);

void **x_case_array_get_array(x_case_array_t *array);

char *x_case_array_get_as_delimited_string(x_case_array_t *array,
    x_core_get_as_string_f get_as_string, char *delimiter);

x_core_compare_f x_case_array_get_compare(x_case_array_t *array);

x_core_copy_f x_case_array_get_copy(x_case_array_t *array);

x_core_destroy_f x_case_array_get_destroy(x_case_array_t *array);

unsigned long x_case_array_get_size(x_case_array_t *array);

void *x_case_array_iterate_next(x_case_array_t *array);

void x_case_array_iterate_remove(x_case_array_t *array);

void x_case_array_iterate_start(x_case_array_t *array);

void x_case_array_print(x_case_array_t *array);

void x_case_array_remove(x_case_array_t *array,
    unsigned long index);

void x_case_array_replace(x_case_array_t *array,
    unsigned long index, void *object);

void x_case_array_replace_random(x_case_array_t *array,
    void *object);

x_core_bool_t x_case_array_resize(x_case_array_t *array,
    unsigned long new_size);

void x_case_array_sort(x_case_array_t *array);

#endif
