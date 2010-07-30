#ifndef x_container_set_h
#define x_container_set_h

#include "x/container/list.h"
#include "x/core/declarations.h"
#include "x/core/message.h"
#include "x/core/object.h"
#include "x/core/types.h"

/*
#if defined X_CONTAINER_SET_MBIN_IMPL
#include "x/container/set.mbin.impl.h"
#endif
*/

x_core_bool_t x_container_set_absorb(x_container_set_t *set,
    x_container_set_t *absorb_these);

x_core_bool_t x_container_set_absorb_array(x_container_set_t *set,
    x_container_array_t *absorb_these);

x_core_bool_t x_container_set_absorb_copy(x_container_set_t *set,
    x_container_set_t *absorb_these);

x_core_bool_t x_container_set_absorb_list_copy(x_container_set_t *set,
    x_container_list_t *absorb_these);

x_core_bool_t x_container_set_add(x_container_set_t *set, void *object);

x_core_bool_t x_container_set_add_replace(x_container_set_t *set,
    void *object);

x_core_bool_t x_container_set_add_to_message(x_container_set_t *set,
    x_core_message_t *message, x_core_message_add_to_message_f add_to_message);

void x_container_set_clear(x_container_set_t *set);

int x_container_set_compare(void *set_object_a, void *set_object_b);

x_core_bool_t x_container_set_contains(x_container_set_t *superset,
    x_container_set_t *subset);

void *x_container_set_copy(void *set_object);

x_container_set_t *x_container_set_create(x_core_objectey_t *objectey);

x_container_set_t *x_container_set_create_from_message
(x_core_objectey_t *objectey, x_core_message_t *message,
    x_core_message_create_from_message_f create_from_message);

x_container_set_t *x_container_set_create_from_union(x_container_set_t *set_a,
    x_container_set_t *set_b);

void x_container_set_destroy(void *set_object);

/*
  rename as a verb?
*/
x_container_set_t *x_container_set_difference(x_container_set_t *set_a,
    x_container_set_t *set_b);

void x_container_set_dont_destroy_objects(x_container_set_t *set);

x_core_bool_t x_container_set_expunge(x_container_set_t *set,
    x_container_set_t *expunge_these);

void *x_container_set_find(x_container_set_t *set, void *decoy_object);

void *x_container_set_find_any(x_container_set_t *set);

void *x_container_set_find_copy(x_container_set_t *set, void *decoy_object);

void *x_container_set_find_take(x_container_set_t *set, void *decoy_object);

x_container_array_t *x_container_set_get_as_array(x_container_set_t *set);

x_container_list_t *x_container_set_get_as_list(x_container_set_t *set);

char *x_container_set_get_as_delimited_string(x_container_set_t *set,
    x_core_get_as_string_f get_as_string, const char *delimiter);

x_core_compare_f x_container_set_get_compare_function(x_container_set_t *set);

x_core_copy_f x_container_set_get_copy_function(x_container_set_t *set);

x_core_destroy_f x_container_set_get_destroy_function(x_container_set_t *set);

x_core_objectey_t *x_container_set_get_objectey(x_container_set_t *set);

unsigned long x_container_set_get_size(x_container_set_t *set);

void *x_container_set_iterate_next(x_container_set_t *set);

void x_container_set_iterate_remove(x_container_set_t *set);

void x_container_set_iterate_start(x_container_set_t *set);

void x_container_set_lock(x_container_set_t *set);

x_core_bool_t x_container_set_overlaps(x_container_set_t *set_a,
    x_container_set_t *set_b);

void x_container_set_print(x_container_set_t *set,
    x_core_get_as_string_f get_as_string);

x_core_bool_t x_container_set_remove(x_container_set_t *set, void *object);

void x_container_set_unlock(x_container_set_t *set);

#endif
