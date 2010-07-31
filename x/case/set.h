#ifndef cf_x_case_set_h
#define cf_x_case_set_h

#include "cf/x/case/list.h"
#include "cf/x/core/declarations.h"
#include "cf/x/core/message.h"
#include "cf/x/core/object.h"
#include "cf/x/core/types.h"

cf_x_core_bool_t cf_x_case_set_absorb(cf_x_case_set_t *set,
    cf_x_case_set_t *absorb_these);

cf_x_core_bool_t cf_x_case_set_absorb_array(cf_x_case_set_t *set,
    cf_x_case_array_t *absorb_these);

cf_x_core_bool_t cf_x_case_set_absorb_copy(cf_x_case_set_t *set,
    cf_x_case_set_t *absorb_these);

cf_x_core_bool_t cf_x_case_set_absorb_list_copy(cf_x_case_set_t *set,
    cf_x_case_list_t *absorb_these);

cf_x_core_bool_t cf_x_case_set_add(cf_x_case_set_t *set, void *object);

cf_x_core_bool_t cf_x_case_set_add_replace(cf_x_case_set_t *set,
    void *object);

cf_x_core_bool_t cf_x_case_set_add_to_message(cf_x_case_set_t *set,
    cf_x_core_message_t *message, cf_x_core_message_add_to_message_f add_to_message);

void cf_x_case_set_clear(cf_x_case_set_t *set);

int cf_x_case_set_compare(void *set_object_a, void *set_object_b);

cf_x_core_bool_t cf_x_case_set_contains(cf_x_case_set_t *superset,
    cf_x_case_set_t *subset);

void *cf_x_case_set_copy(void *set_object);

cf_x_case_set_t *cf_x_case_set_create(cf_x_core_objectey_t *objectey);

cf_x_case_set_t *cf_x_case_set_create_from_message
(cf_x_core_objectey_t *objectey, cf_x_core_message_t *message,
    cf_x_core_message_create_from_message_f create_from_message);

cf_x_case_set_t *cf_x_case_set_create_from_union(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b);

void cf_x_case_set_destroy(void *set_object);

/*
  rename as a verb?
*/
cf_x_case_set_t *cf_x_case_set_difference(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b);

void cf_x_case_set_dont_destroy_objects(cf_x_case_set_t *set);

cf_x_core_bool_t cf_x_case_set_expunge(cf_x_case_set_t *set,
    cf_x_case_set_t *expunge_these);

void *cf_x_case_set_find(cf_x_case_set_t *set, void *decoy_object);

void *cf_x_case_set_find_any(cf_x_case_set_t *set);

void *cf_x_case_set_find_copy(cf_x_case_set_t *set, void *decoy_object);

void *cf_x_case_set_find_take(cf_x_case_set_t *set, void *decoy_object);

cf_x_case_array_t *cf_x_case_set_get_as_array(cf_x_case_set_t *set);

cf_x_case_list_t *cf_x_case_set_get_as_list(cf_x_case_set_t *set);

char *cf_x_case_set_get_as_delimited_string(cf_x_case_set_t *set,
    cf_x_core_object_get_as_string_f get_as_string, const char *delimiter);

cf_x_core_object_compare_f cf_x_case_set_get_compare_function(cf_x_case_set_t *set);

cf_x_core_object_copy_f cf_x_case_set_get_copy_function(cf_x_case_set_t *set);

cf_x_core_object_destroy_f cf_x_case_set_get_destroy_function(cf_x_case_set_t *set);

cf_x_core_objectey_t *cf_x_case_set_get_objectey(cf_x_case_set_t *set);

unsigned long cf_x_case_set_get_size(cf_x_case_set_t *set);

void *cf_x_case_set_iterate_next(cf_x_case_set_t *set);

void cf_x_case_set_iterate_remove(cf_x_case_set_t *set);

void cf_x_case_set_iterate_start(cf_x_case_set_t *set);

void cf_x_case_set_lock(cf_x_case_set_t *set);

cf_x_core_bool_t cf_x_case_set_overlaps(cf_x_case_set_t *set_a,
    cf_x_case_set_t *set_b);

void cf_x_case_set_print(cf_x_case_set_t *set,
    cf_x_core_object_get_as_string_f get_as_string);

cf_x_core_bool_t cf_x_case_set_remove(cf_x_case_set_t *set, void *object);

void cf_x_case_set_unlock(cf_x_case_set_t *set);

#endif
