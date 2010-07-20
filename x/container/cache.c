#include "x/container/cache.h"
#include "x/container/set.h"
#include "x/core/tools.h"

struct x_container_cache_t {
  x_container_set_t *objects;
  x_core_qutex_t *objects_qutex;
  x_core_condition_f remove_condition;
};

x_core_bool_t x_container_cache_add(x_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  x_core_bool_t success;

  x_core_qutex_lock_exclusive(cache->objects_qutex);
  success = x_container_set_add(cache->objects, object);
  x_core_qutex_unlock_exclusive(cache->objects_qutex);

  if (!success) {
    x_core_trace("x_container_set_add");
  }

  return success;
}

void x_container_cache_clear(x_container_cache_t *cache)
{
  x_core_qutex_lock_exclusive(cache->objects_qutex);
  x_container_set_clear(cache->objects);
  x_core_qutex_unlock_exclusive(cache->objects_qutex);
}

x_container_cache_t *x_container_cache_create
(x_core_compare_f compare, x_core_copy_f copy,
    x_core_destroy_f destroy,
    x_core_condition_f remove_condition)
{
  x_container_cache_t *cache;
  x_core_bool_t so_far_so_good;

  cache = malloc(sizeof *cache);
  if (cache) {
    cache->remove_condition = remove_condition;
    cache->objects_qutex = NULL;
    cache->objects = x_container_set_create(compare, copy,
        destroy);
    if (cache->objects) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_container_set_create");
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc() failed");
  }

  if (so_far_so_good) {
    cache->objects_qutex = x_core_qutex_create();
    if (!cache->objects_qutex) {
      so_far_so_good = x_core_bool_false;
      x_core_trace("x_core_qutex_create");
    }
  }

  if (!so_far_so_good) {
    if (cache->objects) {
      x_container_set_destroy(cache->objects);
    }
    if (cache->objects_qutex) {
      x_core_qutex_destroy(cache->objects_qutex);
    }
    free(cache);
    cache = NULL;
  }

  return cache;
}

void x_container_cache_destroy(x_container_cache_t *cache)
{
  assert(cache);
  x_container_set_destroy(cache->objects);
  x_core_qutex_destroy(cache->objects_qutex);
}

void *x_container_cache_find(x_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *found_object;

  x_core_qutex_lock_shared(cache->objects_qutex);
  found_object = x_container_set_find(cache->objects, object);
  x_core_qutex_unlock_shared(cache->objects_qutex);

  return found_object;
}

void *x_container_cache_find_copy(x_container_cache_t *cache,
    void *object)
{
  assert(cache);
  assert(object);
  void *object_copy;

  x_core_qutex_lock_shared(cache->objects_qutex);
  object_copy = x_container_set_find_copy(cache->objects, object);
  x_core_qutex_unlock_shared(cache->objects_qutex);

  return object_copy;
}

unsigned long x_container_cache_get_size(x_container_cache_t *cache)
{
  assert(cache);
  unsigned long size;

  x_core_qutex_lock_shared(cache->objects_qutex);
  size = x_container_set_get_size(cache->objects);
  x_core_qutex_unlock_shared(cache->objects_qutex);

  return size;
}

void x_container_cache_remove_objects(x_container_cache_t *cache)
{
  assert(cache);
  void *object;

  x_core_qutex_lock_shared(cache->objects_qutex);
  {
    x_container_set_iterate_start(cache->objects);
    while ((object = x_container_set_iterate_next(cache->objects))) {
      if (cache->remove_condition(object)) {
        x_container_set_iterate_remove(cache->objects);
      }
    }
  }
  x_core_qutex_unlock_shared(cache->objects_qutex);
}
