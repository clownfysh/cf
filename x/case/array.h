#ifndef cf_x_case_array_h
#define cf_x_case_array_h

#include "cf/x/case/declarations.h"
#include "cf/x/core/bool.h"
#include "cf/x/core/message.h"
#include "cf/x/core/object.h"

void cf_x_case_array_add(cf_x_case_array_t *array, unsigned long index,
    void *object);

cf_x_core_bool_t cf_x_case_array_add_to_message(cf_x_case_array_t *array,
    cf_x_core_message_t *message,
    cf_x_core_message_add_to_message_f add_to_message);

void cf_x_case_array_clear(cf_x_case_array_t *array);

int cf_x_case_array_compare(void *array_object_a, void *array_object_b);

void *cf_x_case_array_copy(void *array_object);

unsigned long cf_x_case_array_count_non_null_objects
(cf_x_case_array_t *array);

cf_x_case_array_t *cf_x_case_array_create(unsigned long initial_size,
    cf_x_core_compare_f compare, cf_x_core_copy_f copy, cf_x_core_destroy_f destroy);

cf_x_case_array_t *cf_x_case_array_create_from_message
(cf_x_core_compare_f compare_object, cf_x_core_copy_f copy,
    cf_x_core_destroy_f destroy, cf_x_core_message_t *message,
    cf_x_core_message_create_from_message_f create_from_message);

cf_x_case_array_t *cf_x_case_array_create_strings_from_string
(char *string, char *separators);

void cf_x_case_array_destroy(void *array_object);

void cf_x_case_array_expunge_set(cf_x_case_array_t *array,
    cf_x_case_set_t *expunge_these);

void cf_x_case_array_expunge_set_inverse(cf_x_case_array_t *array,
    cf_x_case_set_t *expunge_all_but_these);

void *cf_x_case_array_find(cf_x_case_array_t *array,
    unsigned long index);

void *cf_x_case_array_find_random(cf_x_case_array_t *array);

void *cf_x_case_array_find_first(cf_x_case_array_t *array);

void *cf_x_case_array_find_last(cf_x_case_array_t *array);

void **cf_x_case_array_get_array(cf_x_case_array_t *array);

char *cf_x_case_array_get_as_delimited_string(cf_x_case_array_t *array,
    cf_x_core_get_as_string_f get_as_string, char *delimiter);

cf_x_core_compare_f cf_x_case_array_get_compare(cf_x_case_array_t *array);

cf_x_core_copy_f cf_x_case_array_get_copy(cf_x_case_array_t *array);

cf_x_core_destroy_f cf_x_case_array_get_destroy(cf_x_case_array_t *array);

unsigned long cf_x_case_array_get_size(cf_x_case_array_t *array);

void *cf_x_case_array_iterate_next(cf_x_case_array_t *array);

void cf_x_case_array_iterate_remove(cf_x_case_array_t *array);

void cf_x_case_array_iterate_start(cf_x_case_array_t *array);

void cf_x_case_array_print(cf_x_case_array_t *array);

void cf_x_case_array_remove(cf_x_case_array_t *array,
    unsigned long index);

void cf_x_case_array_replace(cf_x_case_array_t *array,
    unsigned long index, void *object);

void cf_x_case_array_replace_random(cf_x_case_array_t *array,
    void *object);

cf_x_core_bool_t cf_x_case_array_resize(cf_x_case_array_t *array,
    unsigned long new_size);

void cf_x_case_array_sort(cf_x_case_array_t *array);

#endif
