#ifndef cf_x_case_shardset_h
#define cf_x_case_shardset_h

#include "cf/x/core/object.h"
#include "cf/x/core/types.h"

#define CF_X_CASE_SHARDSET_MAX_SHARDS 32768

struct cf_x_case_shardset_t;
typedef struct cf_x_case_shardset_t cf_x_case_shardset_t;

cf_x_core_bool_t cf_x_case_shardset_add(cf_x_case_shardset_t *shardset,
    void *object);

cf_x_core_bool_t cf_x_case_shardset_add_replace
(cf_x_case_shardset_t *shardset, void *object);

void cf_x_case_shardset_clear(cf_x_case_shardset_t *shardset);

cf_x_case_shardset_t *cf_x_case_shardset_create(cf_x_core_object_compare_f compare,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy, cf_x_core_object_compare_equal_f equal,
    cf_x_core_object_hash_f hash_object, cf_x_core_object_mod_f mod, unsigned short shard_count);

void cf_x_case_shardset_destroy(cf_x_case_shardset_t *shardset);

void *cf_x_case_shardset_find(cf_x_case_shardset_t *shardset,
    void *object);

void *cf_x_case_shardset_find_copy(cf_x_case_shardset_t *shardset,
    void *object);

unsigned long cf_x_case_shardset_get_size(cf_x_case_shardset_t *shardset);

void *cf_x_case_shardset_iterate_next(cf_x_case_shardset_t *shardset);

void cf_x_case_shardset_lock(cf_x_case_shardset_t *shardset);

void cf_x_case_shardset_iterate_remove(cf_x_case_shardset_t *shardset);

void cf_x_case_shardset_iterate_start(cf_x_case_shardset_t *shardset);

void cf_x_case_shardset_print(cf_x_case_shardset_t *shardset,
    cf_x_core_object_get_as_string_f get_object_as_string);

cf_x_core_bool_t cf_x_case_shardset_remove(cf_x_case_shardset_t *shardset,
    void *object);

unsigned long cf_x_case_shardset_remove_if(cf_x_case_shardset_t *shardset,
    cf_x_core_object_evaluate_condition_f object_condition);

void cf_x_case_shardset_unlock(cf_x_case_shardset_t *shardset);

#endif
