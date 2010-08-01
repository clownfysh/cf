#ifndef cf_x_case_list_h
#define cf_x_case_list_h

#include "cf/x/case/declarations.h"
#include "cf/x/core/iobject.h"
#include "cf/x/core/message.h"
#include "cf/x/core/object.h"

#define CF_X_CASE_LIST_NO_SIZE_LIMIT 0

struct cf_x_case_list_t;
typedef struct cf_x_case_list_t cf_x_case_list_t;

cf_x_core_bool_t cf_x_case_list_absorb(cf_x_case_list_t *list,
    cf_x_case_list_t *absorb_these);

cf_x_core_bool_t cf_x_case_list_add_to_message(cf_x_case_list_t *list,
    cf_x_core_message_t *message, cf_x_core_message_add_to_message_f add_to_message);

cf_x_core_bool_t cf_x_case_list_add_first(cf_x_case_list_t *list,
    void *object);

cf_x_core_bool_t cf_x_case_list_add_last(cf_x_case_list_t *list,
    void *object);

void cf_x_case_list_clear(cf_x_case_list_t *list);

int cf_x_case_list_compare(void *list_object_a, void *list_object_b);

void *cf_x_case_list_copy(void *list_object);

cf_x_case_list_t *cf_x_case_list_create(cf_x_core_object_compare_f compare,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy);

cf_x_case_list_t *cf_x_case_list_create_from_array_n
(cf_x_case_array_t *array, unsigned long max_size);

cf_x_case_list_t *cf_x_case_list_create_from_message
(cf_x_core_object_compare_f compare, cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_message_t *message,
    cf_x_core_message_create_from_message_f create_from_message);

cf_x_case_list_t *cf_x_case_list_create_strings_from_string(char *string,
    const char *separators);

void cf_x_case_list_destroy(void *list_object);

void cf_x_case_list_dont_destroy_objects(cf_x_case_list_t *list);

void *cf_x_case_list_find_at(cf_x_case_list_t *list, unsigned long index);

void *cf_x_case_list_find_first(cf_x_case_list_t *list);

void *cf_x_case_list_find_last(cf_x_case_list_t *list);

void *cf_x_case_list_find_random(cf_x_case_list_t *list);

char *cf_x_case_list_get_as_delimited_string(cf_x_case_list_t *list,
    cf_x_core_object_get_as_string_f get_as_string, const char *delimiter);

char *cf_x_case_list_get_as_string(void *list_object);

unsigned long cf_x_case_list_get_size(cf_x_case_list_t *list);

void cf_x_case_list_init_iobject(cf_x_core_iobject_t *iobject);

void *cf_x_case_list_iterate_next(cf_x_case_list_t *list);

void cf_x_case_list_iterate_remove(cf_x_case_list_t *list);

void cf_x_case_list_iterate_start(cf_x_case_list_t *list);

void cf_x_case_list_lock(cf_x_case_list_t *list);

void cf_x_case_list_print(void *list_object);

cf_x_core_bool_t cf_x_case_list_remove_first(cf_x_case_list_t *list);

cf_x_core_bool_t cf_x_case_list_remove_last(cf_x_case_list_t *list);

void cf_x_case_list_set_size_limit(cf_x_case_list_t *list,
    unsigned long size_limit);

void cf_x_case_list_unlock(cf_x_case_list_t *list);

#endif
