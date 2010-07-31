#ifndef x_case_cache_h
#define x_case_cache_h

#include "x/core/object.h"
#include "x/core/qutex.h"

struct x_case_cache_t;
typedef struct x_case_cache_t x_case_cache_t;

x_core_bool_t x_case_cache_add(x_case_cache_t *cache, void *object);

void x_case_cache_clear(x_case_cache_t *cache);

x_case_cache_t *x_case_cache_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy,
    x_core_condition_f remove_condition);

void x_case_cache_destroy(x_case_cache_t *cache);

void *x_case_cache_find(x_case_cache_t *cache, void *object);

void *x_case_cache_find_copy(x_case_cache_t *cache, void *object);

unsigned long x_case_cache_get_size(x_case_cache_t *cache);

void x_case_cache_remove_objects(x_case_cache_t *cache);

#endif
