#ifndef x_container_shardset_h
#define x_container_shardset_h

#include "x/core/object.h"
#include "x/core/types.h"

#define X_CONTAINER_SHARDSET_MAX_SHARDS 32768

struct x_container_shardset_t;
typedef struct x_container_shardset_t x_container_shardset_t;

x_core_bool_t x_container_shardset_add(x_container_shardset_t *shardset,
    void *object);

x_core_bool_t x_container_shardset_add_replace
(x_container_shardset_t *shardset, void *object);

void x_container_shardset_clear(x_container_shardset_t *shardset);

x_container_shardset_t *x_container_shardset_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy, x_core_hash_f hash_object,
    unsigned short shard_count);

void x_container_shardset_destroy(x_container_shardset_t *shardset);

void *x_container_shardset_find(x_container_shardset_t *shardset,
    void *object);

void *x_container_shardset_find_copy(x_container_shardset_t *shardset,
    void *object);

unsigned long x_container_shardset_get_size(x_container_shardset_t *shardset);

void *x_container_shardset_iterate_next(x_container_shardset_t *shardset);

void x_container_shardset_lock(x_container_shardset_t *shardset);

void x_container_shardset_iterate_remove(x_container_shardset_t *shardset);

void x_container_shardset_iterate_start(x_container_shardset_t *shardset);

void x_container_shardset_print(x_container_shardset_t *shardset,
    x_core_get_as_string_f get_object_as_string);

x_core_bool_t x_container_shardset_remove(x_container_shardset_t *shardset,
    void *object);

unsigned long x_container_shardset_remove_if(x_container_shardset_t *shardset,
    x_core_condition_f object_condition);

void x_container_shardset_unlock(x_container_shardset_t *shardset);

#endif
