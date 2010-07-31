#include "x/case/cache.h"
#include "x/core/constants.h"
#include "x/core/tools.h"
#include "x/xcache/item.h"
#include "x/xcache/system.h"

#define WAIT_ON_OBJECT_SLEEP_MICROSECONDS X_CORE_STANDARD_SLEEP_MICROSECONDS

struct x_xcache_system_t {
  x_case_cache_t *container_cache;
  pthread_mutex_t mutex;
};

static void *find_copy_expire(x_xcache_system_t *system,
    x_core_uuid_t *object_uuid);

void *find_copy_expire(x_xcache_system_t *system,
    x_core_uuid_t *object_uuid)
{
  assert(system);
  assert(object_uuid);
  x_xcache_item_t *item_decoy;
  x_xcache_item_t *item;
  void *object_copy;

  object_copy = NULL;

  item_decoy = x_xcache_item_create_decoy(object_uuid);
  if (item_decoy) {

    pthread_mutex_lock(&system->mutex);
    {
      item = x_case_cache_find(system->container_cache, item_decoy);
      if (item) {
        object_copy = x_xcache_item_get_object_copy(item);
        if (!object_copy) {
          x_core_trace("x_xcache_item_get_object_copy");
        }
        x_xcache_item_expire(item);
      }
    }
    pthread_mutex_unlock(&system->mutex);

    x_xcache_item_destroy_decoy(item_decoy);
  } else {
    x_core_trace("x_xcache_item_create_decoy");
  }

  return object_copy;
}

x_core_bool_t x_xcache_system_add(x_xcache_system_t *system,
    x_core_uuid_t *object_uuid, void *object,
    x_core_copy_f copy, x_core_destroy_f destroy,
    unsigned long lifetime_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  x_core_bool_t success;
  x_xcache_item_t *item;

  success = x_core_bool_false;

  item = x_xcache_item_create(object_uuid, object, copy, destroy,
      lifetime_seconds);
  if (item) {
    success = x_case_cache_add(system->container_cache, item);
    if (!success) {
      x_xcache_item_destroy(item);
      x_core_trace("x_case_cache_add");
    }
  } else {
    x_core_trace("x_xcache_item_create");
  }

  return success;
}

void x_xcache_system_clear(x_xcache_system_t *system)
{
  x_case_cache_clear(system->container_cache);
}

x_xcache_system_t *x_xcache_system_create()
{
  x_xcache_system_t *system;
  x_core_bool_t so_far_so_good;
  x_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = x_core_bool_false;

  system = malloc(sizeof *system);
  if (system) {
    system->container_cache = x_case_cache_create(x_xcache_item_compare,
        x_xcache_item_copy, x_xcache_item_destroy, x_xcache_item_is_expired);
    if (system->container_cache) {
      so_far_so_good = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("new");
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&system->mutex, NULL)) {
      mutex_needs_destroy = x_core_bool_true;
    } else {
      so_far_so_good = x_core_bool_false;
      x_core_trace("pthread_mutex_init");
    }
  }

  if (system && !so_far_so_good) {
    if (system->container_cache) {
      x_case_cache_destroy(system->container_cache);
    }
    if (mutex_needs_destroy) {
      if (0 != pthread_mutex_destroy(&system->mutex)) {
        x_core_trace("pthread_mutex_destroy");
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void x_xcache_system_destroy(x_xcache_system_t *system)
{
  assert(system);
  x_case_cache_destroy(system->container_cache);
  if (0 != pthread_mutex_destroy(&system->mutex)) {
    x_core_trace("pthread_mutex_destroy");
  }
  free(system);
}

void *x_xcache_system_find_wait_copy_expire
(x_xcache_system_t *system, x_core_uuid_t *object_uuid,
    unsigned long max_wait_seconds)
{
  assert(system);
  assert(object_uuid);
  void *object_copy;
  time_t start_wait_time;
  time_t current_time;
  unsigned long wait_time_seconds;

  start_wait_time = time(NULL);

  do {
    object_copy = find_copy_expire(system, object_uuid);
    if (!object_copy) {
      usleep(WAIT_ON_OBJECT_SLEEP_MICROSECONDS);
      current_time = time(NULL);
      wait_time_seconds = current_time - start_wait_time;
    }
  } while (!object_copy && (wait_time_seconds < max_wait_seconds));

  return object_copy;
}

unsigned long x_xcache_system_get_size(x_xcache_system_t *system)
{
  return x_case_cache_get_size(system->container_cache);
}

void x_xcache_system_remove_objects(x_xcache_system_t *system)
{
  pthread_mutex_lock(&system->mutex);
  x_case_cache_remove_objects(system->container_cache);
  pthread_mutex_unlock(&system->mutex);
}
