#include "x/core/mutex.h"
#include "x/core/tools.h"

struct x_core_mutex_t {
  pthread_mutex_t mutex;
};

x_core_mutex_t *x_core_mutex_create()
{
  x_core_mutex_t *mutex;

  mutex = malloc(sizeof *mutex);
  if (mutex) {
    if (0 != pthread_mutex_init(&mutex->mutex, NULL)) {
      x_core_trace("pthread_mutex_init");
      free(mutex);
      mutex = NULL;
    }
  } else {
    x_core_trace("malloc() failed");
  }

  return mutex;
}

void x_core_mutex_destroy(x_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_destroy(&mutex->mutex)) {
    x_core_trace("pthread_mutex_destroy");
  }
  free(mutex);
}

void x_core_mutex_lock(x_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_lock(&mutex->mutex)) {
    x_core_trace("pthread_mutex_lock");
  }
}

x_core_bool_t x_core_mutex_trylock(x_core_mutex_t *mutex)
{
  assert(mutex);
  x_core_bool_t success;

  if (0 == pthread_mutex_lock(&mutex->mutex)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

void x_core_mutex_unlock(x_core_mutex_t *mutex)
{
  if (0 != pthread_mutex_unlock(&mutex->mutex)) {
    x_core_trace("pthread_mutex_unlock");
  }
}
