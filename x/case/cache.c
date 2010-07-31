#include "cf/x/case/cache.h"
#include "cf/x/case/set.h"
#include "cf/x/core/tools.h"

struct cf_x_case_cache_t {
  cf_x_case_set_t *objects;
  cf_x_sync_qutex_t *objects_qutex;
  cf_x_core_object_evaluate_condition_f remove_condition;
  cf_x_core_objectey_t objects_objectey;
};

cf_x_core_bool_t cf_x_case_cache_add(cf_x_case_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  cf_x_core_bool_t success;

  cf_x_sync_qutex_lock_exclusive(cache->objects_qutex);
  success = cf_x_case_set_add(cache->objects, object);
  cf_x_sync_qutex_unlock_exclusive(cache->objects_qutex);

  if (!success) {
    cf_x_core_trace("x_case_set_add");
  }

  return success;
}

void cf_x_case_cache_clear(cf_x_case_cache_t *cache)
{
  cf_x_sync_qutex_lock_exclusive(cache->objects_qutex);
  cf_x_case_set_clear(cache->objects);
  cf_x_sync_qutex_unlock_exclusive(cache->objects_qutex);
}

cf_x_case_cache_t *cf_x_case_cache_create(cf_x_core_object_compare_f compare,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    cf_x_core_object_evaluate_condition_f remove_condition)
{
  cf_x_case_cache_t *cache;
  cf_x_core_bool_t so_far_so_good;

  cache = malloc(sizeof *cache);
  if (cache) {
    cache->remove_condition = remove_condition;
    cache->objects_qutex = NULL;
    cf_x_core_objectey_init(&cache->objects_objectey, compare, copy, destroy,
        CF_X_CORE_OBJECT_NO_EQUAL_F, CF_X_CORE_OBJECT_NO_GET_AS_STRING_F,
        CF_X_CORE_OBJECT_NO_MOD_F);
    cache->objects = cf_x_case_set_create(&cache->objects_objectey);
    if (cache->objects) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_case_set_create");
    }
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc() failed");
  }

  if (so_far_so_good) {
    cache->objects_qutex = cf_x_sync_qutex_create();
    if (!cache->objects_qutex) {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("x_core_qutex_create");
    }
  }

  if (!so_far_so_good) {
    if (cache->objects) {
      cf_x_case_set_destroy(cache->objects);
    }
    if (cache->objects_qutex) {
      cf_x_sync_qutex_destroy(cache->objects_qutex);
    }
    free(cache);
    cache = NULL;
  }

  return cache;
}

void cf_x_case_cache_destroy(cf_x_case_cache_t *cache)
{
  assert(cache);
  cf_x_case_set_destroy(cache->objects);
  cf_x_sync_qutex_destroy(cache->objects_qutex);
}

void *cf_x_case_cache_find(cf_x_case_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *found_object;

  cf_x_sync_qutex_lock_shared(cache->objects_qutex);
  found_object = cf_x_case_set_find(cache->objects, object);
  cf_x_sync_qutex_unlock_shared(cache->objects_qutex);

  return found_object;
}

void *cf_x_case_cache_find_copy(cf_x_case_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *object_copy;

  cf_x_sync_qutex_lock_shared(cache->objects_qutex);
  object_copy = cf_x_case_set_find_copy(cache->objects, object);
  cf_x_sync_qutex_unlock_shared(cache->objects_qutex);

  return object_copy;
}

unsigned long cf_x_case_cache_get_size(cf_x_case_cache_t *cache)
{
  assert(cache);
  unsigned long size;

  cf_x_sync_qutex_lock_shared(cache->objects_qutex);
  size = cf_x_case_set_get_size(cache->objects);
  cf_x_sync_qutex_unlock_shared(cache->objects_qutex);

  return size;
}

void cf_x_case_cache_remove_objects(cf_x_case_cache_t *cache)
{
  assert(cache);
  void *object;

  cf_x_sync_qutex_lock_shared(cache->objects_qutex);
  {
    cf_x_case_set_iterate_start(cache->objects);
    while ((object = cf_x_case_set_iterate_next(cache->objects))) {
      if (cache->remove_condition(object)) {
        cf_x_case_set_iterate_remove(cache->objects);
      }
    }
  }
  cf_x_sync_qutex_unlock_shared(cache->objects_qutex);
}
