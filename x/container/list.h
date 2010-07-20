#ifndef x_container_list_h
#define x_container_list_h

#include "x/container/declarations.h"
#include "x/core/message.h"
#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/types.h"

#define X_CONTAINER_LIST_NO_SIZE_LIMIT 0

struct x_container_list_t;
typedef struct x_container_list_t x_container_list_t;

x_core_bool_t x_container_list_absorb(x_container_list_t *list,
    x_container_list_t *absorb_these);

x_core_bool_t x_container_list_add_to_message(x_container_list_t *list,
    x_core_message_t *message, x_core_message_add_to_message_f add_to_message);

x_core_bool_t x_container_list_add_first(x_container_list_t *list,
    void *object);

x_core_bool_t x_container_list_add_last(x_container_list_t *list,
    void *object);

void x_container_list_clear(x_container_list_t *list);

int x_container_list_compare(void *list_object_a, void *list_object_b);

void *x_container_list_copy(void *list_object);

x_container_list_t *x_container_list_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy);

x_container_list_t *x_container_list_create_from_array_n
(x_container_array_t *array, unsigned long max_size);

x_container_list_t *x_container_list_create_from_message
(x_core_compare_f compare, x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message);

x_container_list_t *x_container_list_create_strings_from_string(char *string,
    const char *separators);

void x_container_list_destroy(void *list_object);

void x_container_list_dont_destroys(x_container_list_t *list);

void *x_container_list_find_at(x_container_list_t *list, unsigned long index);

void *x_container_list_find_first(x_container_list_t *list);

void *x_container_list_find_last(x_container_list_t *list);

void *x_container_list_find_random(x_container_list_t *list);

char *x_container_list_get_as_delimited_string(x_container_list_t *list,
    x_core_get_as_string_f get_as_string, const char *delimiter);

char *x_container_list_get_as_string(void *list_object);

unsigned long x_container_list_get_size(x_container_list_t *list);

void x_container_list_init_objectey(x_core_objectey_t *objectey);

void *x_container_list_iterate_next(x_container_list_t *list);

void x_container_list_iterate_remove(x_container_list_t *list);

void x_container_list_iterate_start(x_container_list_t *list);

void x_container_list_lock(x_container_list_t *list);

void x_container_list_print(void *list_object);

x_core_bool_t x_container_list_remove_first(x_container_list_t *list);

x_core_bool_t x_container_list_remove_last(x_container_list_t *list);

void x_container_list_set_size_limit(x_container_list_t *list,
    unsigned long size_limit);

void x_container_list_unlock(x_container_list_t *list);

#endif
