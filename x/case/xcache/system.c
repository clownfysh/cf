#include "cf/x/case/cache.h"
#include "cf/x/core/tools.h"
#include "cf/x/case/xcache/item.h"
#include "cf/x/case/xcache/system.h"

#define WAIT_ON_OBJECT_SLEEP_MICROSECONDS CF_X_CORE_STANDARD_SLEEP_MICROSECONDS

struct cf_x_case_xcache_system_t {
  cf_x_case_cache_t *container_cache;
  pthread_mutex_t mutex;
};

static void *find_copy_expire(cf_x_case_xcache_system_t *system,
    cf_x_core_uuid_t *object_uuid);

void *find_copy_expire(cf_x_case_xcache_system_t *system,
    cf_x_core_uuid_t *object_uuid)
{
  assert(system);
  assert(object_uuid);
  cf_x_case_xcache_item_t *item_decoy;
  cf_x_case_xcache_item_t *item;
  void *object_copy;

  object_copy = NULL;

  item_decoy = cf_x_case_xcache_item_create_decoy(object_uuid);
  if (item_decoy) {

    pthread_mutex_lock(&system->mutex);
    {
      item = cf_x_case_cache_find(system->container_cache, item_decoy);
      if (item) {
        object_copy = cf_x_case_xcache_item_get_object_copy(item);
        if (!object_copy) {
          cf_x_core_trace("x_xcache_item_get_object_copy");
        }
        cf_x_case_xcache_item_expire(item);
      }
    }
    pthread_mutex_unlock(&system->mutex);

    cf_x_case_xcache_item_destroy_decoy(item_decoy);
  } else {
    cf_x_core_trace("x_xcache_item_create_decoy");
  }

  return object_copy;
}

cf_x_core_bool_t cf_x_case_xcache_system_add(cf_x_case_xcache_system_t *system,
    cf_x_core_uuid_t *object_uuid, void *object,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    unsigned long lifetime_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  cf_x_core_bool_t success;
  cf_x_case_xcache_item_t *item;

  success = cf_x_core_bool_false;

  item = cf_x_case_xcache_item_create(object_uuid, object, copy, destroy,
      lifetime_seconds);
  if (item) {
    success = cf_x_case_cache_add(system->container_cache, item);
    if (!success) {
      cf_x_case_xcache_item_destroy(item);
      cf_x_core_trace("x_case_cache_add");
    }
  } else {
    cf_x_core_trace("x_xcache_item_create");
  }

  return success;
}

void cf_x_case_xcache_system_clear(cf_x_case_xcache_system_t *system)
{
  cf_x_case_cache_clear(system->container_cache);
}

cf_x_case_xcache_system_t *cf_x_case_xcache_system_create()
{
  cf_x_case_xcache_system_t *system;
  cf_x_core_bool_t so_far_so_good;
  cf_x_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = cf_x_core_bool_false;

  system = malloc(sizeof *system);
  if (system) {
    system->container_cache = cf_x_case_cache_create(cf_x_case_xcache_item_compare,
        cf_x_case_xcache_item_copy, cf_x_case_xcache_item_destroy, cf_x_case_xcache_item_is_expired);
    if (system->container_cache) {
      so_far_so_good = cf_x_core_bool_true;
    } else {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("new");
    }
  } else {
    so_far_so_good = cf_x_core_bool_false;
    cf_x_core_trace("malloc");
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&system->mutex, NULL)) {
      mutex_needs_destroy = cf_x_core_bool_true;
    } else {
      so_far_so_good = cf_x_core_bool_false;
      cf_x_core_trace("pthread_mutex_init");
    }
  }

  if (system && !so_far_so_good) {
    if (system->container_cache) {
      cf_x_case_cache_destroy(system->container_cache);
    }
    if (mutex_needs_destroy) {
      if (0 != pthread_mutex_destroy(&system->mutex)) {
        cf_x_core_trace("pthread_mutex_destroy");
      }
    }
    free(system);
    system = NULL;
  }

  return system;
}

void cf_x_case_xcache_system_destroy(cf_x_case_xcache_system_t *system)
{
  assert(system);
  cf_x_case_cache_destroy(system->container_cache);
  if (0 != pthread_mutex_destroy(&system->mutex)) {
    cf_x_core_trace("pthread_mutex_destroy");
  }
  free(system);
}

void *cf_x_case_xcache_system_find_wait_copy_expire
(cf_x_case_xcache_system_t *system, cf_x_core_uuid_t *object_uuid,
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

unsigned long cf_x_case_xcache_system_get_size(cf_x_case_xcache_system_t *system)
{
  return cf_x_case_cache_get_size(system->container_cache);
}

void cf_x_case_xcache_system_remove_objects(cf_x_case_xcache_system_t *system)
{
  pthread_mutex_lock(&system->mutex);
  cf_x_case_cache_remove_objects(system->container_cache);
  pthread_mutex_unlock(&system->mutex);
}
