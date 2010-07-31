#include "cf/x/core/mutex.h"
#include "cf/x/core/tools.h"

struct cf_x_core_mutex_t {
  pthread_mutex_t mutex;
};

cf_x_core_mutex_t *cf_x_core_mutex_create()
{
  cf_x_core_mutex_t *mutex;

  mutex = malloc(sizeof *mutex);
  if (mutex) {
    if (0 != pthread_mutex_init(&mutex->mutex, NULL)) {
      cf_x_core_trace("pthread_mutex_init");
      free(mutex);
      mutex = NULL;
    }
  } else {
    cf_x_core_trace("malloc() failed");
  }

  return mutex;
}

void cf_x_core_mutex_destroy(cf_x_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_destroy(&mutex->mutex)) {
    cf_x_core_trace("pthread_mutex_destroy");
  }
  free(mutex);
}

void cf_x_core_mutex_lock(cf_x_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_lock(&mutex->mutex)) {
    cf_x_core_trace("pthread_mutex_lock");
  }
}

cf_x_core_bool_t cf_x_core_mutex_trylock(cf_x_core_mutex_t *mutex)
{
  assert(mutex);
  cf_x_core_bool_t success;

  if (0 == pthread_mutex_lock(&mutex->mutex)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

void cf_x_core_mutex_unlock(cf_x_core_mutex_t *mutex)
{
  if (0 != pthread_mutex_unlock(&mutex->mutex)) {
    cf_x_core_trace("pthread_mutex_unlock");
  }
}
