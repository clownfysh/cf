#ifndef x_container_cache_h
#define x_container_cache_h

#include "x/core/object.h"
#include "x/core/qutex.h"

struct x_container_cache_t;
typedef struct x_container_cache_t x_container_cache_t;

x_core_bool_t x_container_cache_add(x_container_cache_t *cache, void *object);

void x_container_cache_clear(x_container_cache_t *cache);

x_container_cache_t *x_container_cache_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_condition_f remove_condition);

void x_container_cache_destroy(x_container_cache_t *cache);

void *x_container_cache_find(x_container_cache_t *cache, void *object);

void *x_container_cache_find_copy(x_container_cache_t *cache, void *object);

unsigned long x_container_cache_get_size(x_container_cache_t *cache);

void x_container_cache_remove_objects(x_container_cache_t *cache);

#endif
