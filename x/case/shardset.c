#include "x/case/set.h"
#include "x/case/shardset.h"
#include "x/core/tools.h"

struct x_case_shardset_t {
  x_core_hash_f hash_object;
  unsigned short shard_count;
  x_case_set_t *shards[X_CASE_SHARDSET_MAX_SHARDS];
  pthread_mutex_t shard_mutexes[X_CASE_SHARDSET_MAX_SHARDS];

  x_core_bool_t iterate_remove;
  x_core_bool_t iterate_first;
  unsigned short iterator_shard;
  void *iterator;

  pthread_mutex_t mutex;

  x_core_objectey_t set_objectey;
};

static void x_case_shardset_create_rollback
(x_case_shardset_t *shardset);

static unsigned short get_shard_id_for_object(x_case_shardset_t *shardset,
    void *object);

x_core_bool_t x_case_shardset_add(x_case_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  x_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = x_case_set_add(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

x_core_bool_t x_case_shardset_add_replace
(x_case_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  x_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success
    = x_case_set_add_replace(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

void x_case_shardset_clear(x_case_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    x_case_set_clear(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

/*
  TODO: simplify
  TODO: ugh...some rollback code is in the rollback method, some not...fix
*/
x_case_shardset_t *x_case_shardset_create(x_core_compare_f compare,
    x_core_copy_f copy, x_core_destroy_f destroy, x_core_equal_f equal,
    x_core_hash_f hash_object, x_core_mod_f mod, unsigned short shard_count)
{
  assert(shard_count > 0);
  x_case_shardset_t *shardset;
  x_core_bool_t so_far_so_good;
  unsigned short each_shard;
  x_core_bool_t mutex_needs_destroy;

  mutex_needs_destroy = x_core_bool_false;

  if (shard_count > X_CASE_SHARDSET_MAX_SHARDS) {
    shard_count = X_CASE_SHARDSET_MAX_SHARDS;
  }

  shardset = malloc(sizeof *shardset);
  if (shardset) {
    shardset->shard_count = shard_count;
    shardset->hash_object = hash_object;
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard) = NULL;
    }
    so_far_so_good = x_core_bool_true;
  } else {
    x_core_trace("malloc");
    so_far_so_good = x_core_bool_false;
  }

  if (so_far_so_good) {
    x_core_objectey_init(&shardset->set_objectey, compare, copy, destroy,
        equal, X_CORE_NO_GET_AS_STRING_FUNCTION, mod);
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      *(shardset->shards + each_shard)
        = x_case_set_create(&shardset->set_objectey);
      if (!*(shardset->shards + each_shard)) {
        x_core_trace("x_case_set_create");
        so_far_so_good = x_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    for (each_shard = 0; each_shard < shard_count; each_shard++) {
      if (0 != pthread_mutex_init
          (shardset->shard_mutexes + each_shard, NULL)) {
        x_core_trace("pthread_mutex_init");
        so_far_so_good = x_core_bool_false;
        break;
      }
    }
  }

  if (so_far_so_good) {
    if (0 == pthread_mutex_init(&shardset->mutex, NULL)) {
      mutex_needs_destroy = x_core_bool_true;
    } else {
      x_core_trace("pthread_mutex_init");
    }
  }

  if (!so_far_so_good && shardset) {
    x_case_shardset_create_rollback(shardset);
    if (mutex_needs_destroy) {
      if (!pthread_mutex_destroy(&shardset->mutex)) {
        x_core_trace("pthread_mutex_destroy");
      }
    }
    free(shardset);
    shardset = NULL;
  }

  return shardset;
}

void x_case_shardset_create_rollback(x_case_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  if (shardset->shards) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (*(shardset->shards + each_shard)) {
        x_case_set_destroy(*(shardset->shards + each_shard));
      }
    }
    free(shardset->shards);
  }

  if (shardset->shard_mutexes) {
    for (each_shard = 0; each_shard < shardset->shard_count;
         each_shard++) {
      if (0 != pthread_mutex_destroy
          (shardset->shard_mutexes + each_shard)) {
        x_core_trace("pthread_mutex_destroy");
      }
    }
  }
}

void x_case_shardset_destroy(x_case_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    x_case_set_destroy(*(shardset->shards + each_shard));
    if (0 != pthread_mutex_destroy(shardset->shard_mutexes + each_shard)) {
      x_core_trace("pthread_mutex_destroy");
    }
  }

  if (0 != pthread_mutex_destroy(&shardset->mutex)) {
    x_core_trace("pthread_mutex_destroy");
  }

  free(shardset);
}

void *x_case_shardset_find(x_case_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = x_case_set_find(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

void *x_case_shardset_find_copy
(x_case_shardset_t *shardset, void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  void *found_object;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  found_object = x_case_set_find_copy
    (*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return found_object;
}

unsigned long x_case_shardset_get_size(x_case_shardset_t *shardset)
{
  assert(shardset);
  unsigned short each_shard;
  unsigned long total_size;

  total_size = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    total_size += x_case_set_get_size(*(shardset->shards + each_shard));
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }

  return total_size;
}

void *x_case_shardset_iterate_next
(x_case_shardset_t *shardset)
{
  assert(shardset);
  x_case_set_t *set;
  void *next_object;

  if (shardset->iterator) {
    if (shardset->iterate_first) {
      shardset->iterate_first = x_core_bool_false;
      next_object = shardset->iterator;
    } else {
      set = shardset->shards[shardset->iterator_shard];
      if (shardset->iterate_remove) {
        x_case_set_iterate_remove(set);
        shardset->iterate_remove = x_core_bool_false;
      }
      shardset->iterator = x_case_set_iterate_next(set);
      next_object = shardset->iterator;
    }
  } else {
    next_object = NULL;
  }

  /*
    TODO: there may be a cleaner implementation...this block (almost) is
    repeated in x_case_shardset_iterate_start().  this implementation is
    sortof a hybrid of the way x_core_set/list/array do iteration, crossed with
    the changes necessary in this context.  it may be that in this context
    there is an implementation with much less happening in
    x_case_shardset_iterate_start().
  */
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      x_case_set_iterate_start(set);
      shardset->iterator = x_case_set_iterate_next(set);
      next_object = shardset->iterator;
    } else {
      break;
    }
  }

  return next_object;
}

void x_case_shardset_iterate_remove(x_case_shardset_t *shardset)
{
  shardset->iterate_remove = x_core_bool_true;
}

void x_case_shardset_iterate_start(x_case_shardset_t *shardset)
{
  assert(shardset);
  x_case_set_t *set;

  shardset->iterator_shard = 0;
  set = shardset->shards[shardset->iterator_shard];

  pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
  x_case_set_iterate_start(set);
  shardset->iterator = x_case_set_iterate_next(set);
  while (!shardset->iterator) {
    pthread_mutex_unlock(shardset->shard_mutexes + shardset->iterator_shard);
    if (shardset->iterator_shard < (shardset->shard_count - 1)) {
      shardset->iterator_shard++;
      pthread_mutex_lock(shardset->shard_mutexes + shardset->iterator_shard);
      set = shardset->shards[shardset->iterator_shard];
      x_case_set_iterate_start(set);
      shardset->iterator = x_case_set_iterate_next(set);
    } else {
      break;
    }
  }

  shardset->iterate_remove = x_core_bool_false;
  shardset->iterate_first = x_core_bool_true;
}

void x_case_shardset_lock(x_case_shardset_t *shardset)
{
  pthread_mutex_lock(&shardset->mutex);
}

void x_case_shardset_print(x_case_shardset_t *shardset,
    x_core_get_as_string_f get_object_as_string)
{
  assert(shardset);
  unsigned short each_shard;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {
    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    x_case_set_print(*(shardset->shards + each_shard),
        get_object_as_string);
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);
  }
}

x_core_bool_t x_case_shardset_remove(x_case_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned short shard_id;
  x_core_bool_t success;

  shard_id = get_shard_id_for_object(shardset, object);

  pthread_mutex_lock(shardset->shard_mutexes + shard_id);
  success = x_case_set_remove(*(shardset->shards + shard_id), object);
  pthread_mutex_unlock(shardset->shard_mutexes + shard_id);

  return success;
}

unsigned long x_case_shardset_remove_if(x_case_shardset_t *shardset,
    x_core_condition_f object_condition)
{
  assert(shardset);
  unsigned short each_shard;
  x_case_set_t *set;
  void *object;
  unsigned long total_items_removed;

  total_items_removed = 0;

  for (each_shard = 0; each_shard < shardset->shard_count; each_shard++) {

    pthread_mutex_lock(shardset->shard_mutexes + each_shard);
    {
      set = *(shardset->shards + each_shard);
      x_case_set_iterate_start(set);
      while ((object = x_case_set_iterate_next(set))) {
        if (object_condition(object)) {
          x_case_set_iterate_remove(set);
          total_items_removed++;
        }
      }
    }
    pthread_mutex_unlock(shardset->shard_mutexes + each_shard);

  }

  return total_items_removed;
}

void x_case_shardset_unlock(x_case_shardset_t *shardset)
{
  pthread_mutex_unlock(&shardset->mutex);
}

unsigned short get_shard_id_for_object(x_case_shardset_t *shardset,
    void *object)
{
  assert(shardset);
  assert(object);
  unsigned long hash;
  unsigned short id;

  hash = shardset->hash_object(object);
  id = hash % shardset->shard_count;

  return id;
}
