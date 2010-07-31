#ifndef x_case_shardset_h
#define x_case_shardset_h

#include "x/core/object.h"
#include "x/core/types.h"

#define X_CASE_SHARDSET_MAX_SHARDS 32768

struct x_case_shardset_t;
typedef struct x_case_shardset_t x_case_shardset_t;

x_core_bool_t x_case_shardset_add(x_case_shardset_t *shardset,
    void *object);

x_core_bool_t x_case_shardset_add_replace
(x_case_shardset_t *shardset, void *object);

void x_case_shardset_clear(x_case_shardset_t *shardset);

x_case_shardset_t *x_case_shardset_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy, x_core_equal_f equal,
    x_core_hash_f hash_object, x_core_mod_f mod, unsigned short shard_count);

void x_case_shardset_destroy(x_case_shardset_t *shardset);

void *x_case_shardset_find(x_case_shardset_t *shardset,
    void *object);

void *x_case_shardset_find_copy(x_case_shardset_t *shardset,
    void *object);

unsigned long x_case_shardset_get_size(x_case_shardset_t *shardset);

void *x_case_shardset_iterate_next(x_case_shardset_t *shardset);

void x_case_shardset_lock(x_case_shardset_t *shardset);

void x_case_shardset_iterate_remove(x_case_shardset_t *shardset);

void x_case_shardset_iterate_start(x_case_shardset_t *shardset);

void x_case_shardset_print(x_case_shardset_t *shardset,
    x_core_get_as_string_f get_object_as_string);

x_core_bool_t x_case_shardset_remove(x_case_shardset_t *shardset,
    void *object);

unsigned long x_case_shardset_remove_if(x_case_shardset_t *shardset,
    x_core_condition_f object_condition);

void x_case_shardset_unlock(x_case_shardset_t *shardset);

#endif
