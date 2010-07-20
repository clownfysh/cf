#include "x/core/mutex.h"
#include "x/core/qutex.h"
#include "x/core/tools.h"

struct x_core_qutex_t {
  unsigned long shared_user_count;
  x_core_bool_t exclusive_user;
  x_core_mutex_t *mutex;
};

x_core_qutex_t *x_core_qutex_create()
{
  x_core_qutex_t *qutex;

  qutex = malloc(sizeof *qutex);
  if (qutex) {
    qutex->shared_user_count = 0;
    qutex->exclusive_user = x_core_bool_false;
    qutex->mutex = x_core_mutex_create();
    if (!qutex->mutex) {
      x_core_trace("x_core_mutex_create");
      free(qutex);
      qutex = NULL;
    }
  } else {
    x_core_trace("malloc() failed");
  }

  return qutex;
}

void x_core_qutex_destroy(x_core_qutex_t *qutex)
{
  assert(qutex);
  x_core_mutex_destroy(qutex->mutex);
  free(qutex);
}

void x_core_qutex_lock_exclusive(x_core_qutex_t *qutex)
{
  assert(qutex);
  while (!x_core_qutex_trylock_exclusive(qutex)) {}
}

void x_core_qutex_lock_shared(x_core_qutex_t *qutex)
{
  assert(qutex);
  while (!x_core_qutex_trylock_shared(qutex)) {}
}

x_core_bool_t x_core_qutex_trylock_exclusive(x_core_qutex_t *qutex)
{
  assert(qutex);
  x_core_bool_t locked;

  x_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user && (0 == qutex->shared_user_count)) {
      qutex->exclusive_user = x_core_bool_true;
      locked = x_core_bool_true;
    } else {
      locked = x_core_bool_false;
    }
  }
  x_core_mutex_unlock(qutex->mutex);

  return locked;
}

x_core_bool_t x_core_qutex_trylock_shared(x_core_qutex_t *qutex)
{
  assert(qutex);
  x_core_bool_t locked;

  x_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user) {
      qutex->shared_user_count++;
      locked = x_core_bool_true;
    } else {
      locked = x_core_bool_false;
    }
  }
  x_core_mutex_unlock(qutex->mutex);

  return locked;
}

void x_core_qutex_unlock_exclusive(x_core_qutex_t *qutex)
{
  x_core_mutex_lock(qutex->mutex);
  qutex->exclusive_user = x_core_bool_false;
  x_core_mutex_unlock(qutex->mutex);
}

void x_core_qutex_unlock_shared(x_core_qutex_t *qutex)
{
  x_core_mutex_lock(qutex->mutex);
  qutex->shared_user_count--;
  x_core_mutex_unlock(qutex->mutex);
}
