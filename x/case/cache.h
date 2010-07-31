#ifndef cf_x_case_cache_h
#define cf_x_case_cache_h

#include "cf/x/core/object.h"
#include "cf/x/sync/qutex.h"

struct cf_x_case_cache_t;
typedef struct cf_x_case_cache_t cf_x_case_cache_t;

cf_x_core_bool_t cf_x_case_cache_add(cf_x_case_cache_t *cache, void *object);

void cf_x_case_cache_clear(cf_x_case_cache_t *cache);

cf_x_case_cache_t *cf_x_case_cache_create(cf_x_core_object_compare_f compare,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_object_evaluate_condition_f remove_condition);

void cf_x_case_cache_destroy(cf_x_case_cache_t *cache);

void *cf_x_case_cache_find(cf_x_case_cache_t *cache, void *object);

void *cf_x_case_cache_find_copy(cf_x_case_cache_t *cache, void *object);

unsigned long cf_x_case_cache_get_size(cf_x_case_cache_t *cache);

void cf_x_case_cache_remove_objects(cf_x_case_cache_t *cache);

#endif
