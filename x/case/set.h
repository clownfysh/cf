#ifndef x_case_set_h
#define x_case_set_h

#include "x/case/list.h"
#include "x/core/declarations.h"
#include "x/core/message.h"
#include "x/core/object.h"
#include "x/core/types.h"

x_core_bool_t x_case_set_absorb(x_case_set_t *set,
    x_case_set_t *absorb_these);

x_core_bool_t x_case_set_absorb_array(x_case_set_t *set,
    x_case_array_t *absorb_these);

x_core_bool_t x_case_set_absorb_copy(x_case_set_t *set,
    x_case_set_t *absorb_these);

x_core_bool_t x_case_set_absorb_list_copy(x_case_set_t *set,
    x_case_list_t *absorb_these);

x_core_bool_t x_case_set_add(x_case_set_t *set, void *object);

x_core_bool_t x_case_set_add_replace(x_case_set_t *set,
    void *object);

x_core_bool_t x_case_set_add_to_message(x_case_set_t *set,
    x_core_message_t *message, x_core_message_add_to_message_f add_to_message);

void x_case_set_clear(x_case_set_t *set);

int x_case_set_compare(void *set_object_a, void *set_object_b);

x_core_bool_t x_case_set_contains(x_case_set_t *superset,
    x_case_set_t *subset);

void *x_case_set_copy(void *set_object);

x_case_set_t *x_case_set_create(x_core_objectey_t *objectey);

x_case_set_t *x_case_set_create_from_message
(x_core_objectey_t *objectey, x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message);

x_case_set_t *x_case_set_create_from_union(x_case_set_t *set_a,
    x_case_set_t *set_b);

void x_case_set_destroy(void *set_object);

/*
  rename as a verb?
*/
x_case_set_t *x_case_set_difference(x_case_set_t *set_a,
    x_case_set_t *set_b);

void x_case_set_dont_destroy_objects(x_case_set_t *set);

x_core_bool_t x_case_set_expunge(x_case_set_t *set,
    x_case_set_t *expunge_these);

void *x_case_set_find(x_case_set_t *set, void *decoy_object);

void *x_case_set_find_any(x_case_set_t *set);

void *x_case_set_find_copy(x_case_set_t *set, void *decoy_object);

void *x_case_set_find_take(x_case_set_t *set, void *decoy_object);

x_case_array_t *x_case_set_get_as_array(x_case_set_t *set);

x_case_list_t *x_case_set_get_as_list(x_case_set_t *set);

char *x_case_set_get_as_delimited_string(x_case_set_t *set,
    x_core_get_as_string_f get_as_string, const char *delimiter);

x_core_compare_f x_case_set_get_compare_function(x_case_set_t *set);

x_core_copy_f x_case_set_get_copy_function(x_case_set_t *set);

x_core_destroy_f x_case_set_get_destroy_function(x_case_set_t *set);

x_core_objectey_t *x_case_set_get_objectey(x_case_set_t *set);

unsigned long x_case_set_get_size(x_case_set_t *set);

void *x_case_set_iterate_next(x_case_set_t *set);

void x_case_set_iterate_remove(x_case_set_t *set);

void x_case_set_iterate_start(x_case_set_t *set);

void x_case_set_lock(x_case_set_t *set);

x_core_bool_t x_case_set_overlaps(x_case_set_t *set_a,
    x_case_set_t *set_b);

void x_case_set_print(x_case_set_t *set,
    x_core_get_as_string_f get_as_string);

x_core_bool_t x_case_set_remove(x_case_set_t *set, void *object);

void x_case_set_unlock(x_case_set_t *set);

#endif
