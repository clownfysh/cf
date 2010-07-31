#ifndef x_case_list_h
#define x_case_list_h

#include "x/case/declarations.h"
#include "x/core/message.h"
#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/types.h"

#define X_CASE_LIST_NO_SIZE_LIMIT 0

struct x_case_list_t;
typedef struct x_case_list_t x_case_list_t;

x_core_bool_t x_case_list_absorb(x_case_list_t *list,
    x_case_list_t *absorb_these);

x_core_bool_t x_case_list_add_to_message(x_case_list_t *list,
    x_core_message_t *message, x_core_message_add_to_message_f add_to_message);

x_core_bool_t x_case_list_add_first(x_case_list_t *list,
    void *object);

x_core_bool_t x_case_list_add_last(x_case_list_t *list,
    void *object);

void x_case_list_clear(x_case_list_t *list);

int x_case_list_compare(void *list_object_a, void *list_object_b);

void *x_case_list_copy(void *list_object);

x_case_list_t *x_case_list_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy);

x_case_list_t *x_case_list_create_from_array_n
(x_case_array_t *array, unsigned long max_size);

x_case_list_t *x_case_list_create_from_message
(x_core_compare_f compare, x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message);

x_case_list_t *x_case_list_create_strings_from_string(char *string,
    const char *separators);

void x_case_list_destroy(void *list_object);

void x_case_list_dont_destroy_objects(x_case_list_t *list);

void *x_case_list_find_at(x_case_list_t *list, unsigned long index);

void *x_case_list_find_first(x_case_list_t *list);

void *x_case_list_find_last(x_case_list_t *list);

void *x_case_list_find_random(x_case_list_t *list);

char *x_case_list_get_as_delimited_string(x_case_list_t *list,
    x_core_get_as_string_f get_as_string, const char *delimiter);

char *x_case_list_get_as_string(void *list_object);

unsigned long x_case_list_get_size(x_case_list_t *list);

void x_case_list_init_objectey(x_core_objectey_t *objectey);

void *x_case_list_iterate_next(x_case_list_t *list);

void x_case_list_iterate_remove(x_case_list_t *list);

void x_case_list_iterate_start(x_case_list_t *list);

void x_case_list_lock(x_case_list_t *list);

void x_case_list_print(void *list_object);

x_core_bool_t x_case_list_remove_first(x_case_list_t *list);

x_core_bool_t x_case_list_remove_last(x_case_list_t *list);

void x_case_list_set_size_limit(x_case_list_t *list,
    unsigned long size_limit);

void x_case_list_unlock(x_case_list_t *list);

#endif
