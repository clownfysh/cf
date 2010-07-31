#include "cf/x/sync/mutex.h"
#include "cf/x/sync/qutex.h"
#include "cf/x/core/tools.h"

struct cf_x_sync_qutex_t {
  unsigned long shared_user_count;
  cf_x_core_bool_t exclusive_user;
  cf_x_sync_mutex_t *mutex;
};

cf_x_sync_qutex_t *cf_x_sync_qutex_create()
{
  cf_x_sync_qutex_t *qutex;

  qutex = malloc(sizeof *qutex);
  if (qutex) {
    qutex->shared_user_count = 0;
    qutex->exclusive_user = cf_x_core_bool_false;
    qutex->mutex = cf_x_sync_mutex_create();
    if (!qutex->mutex) {
      cf_x_core_trace("x_core_mutex_create");
      free(qutex);
      qutex = NULL;
    }
  } else {
    cf_x_core_trace("malloc() failed");
  }

  return qutex;
}

void cf_x_sync_qutex_destroy(cf_x_sync_qutex_t *qutex)
{
  assert(qutex);
  cf_x_sync_mutex_destroy(qutex->mutex);
  free(qutex);
}

void cf_x_sync_qutex_lock_exclusive(cf_x_sync_qutex_t *qutex)
{
  assert(qutex);
  while (!cf_x_sync_qutex_trylock_exclusive(qutex)) {}
}

void cf_x_sync_qutex_lock_shared(cf_x_sync_qutex_t *qutex)
{
  assert(qutex);
  while (!cf_x_sync_qutex_trylock_shared(qutex)) {}
}

cf_x_core_bool_t cf_x_sync_qutex_trylock_exclusive(cf_x_sync_qutex_t *qutex)
{
  assert(qutex);
  cf_x_core_bool_t locked;

  cf_x_sync_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user && (0 == qutex->shared_user_count)) {
      qutex->exclusive_user = cf_x_core_bool_true;
      locked = cf_x_core_bool_true;
    } else {
      locked = cf_x_core_bool_false;
    }
  }
  cf_x_sync_mutex_unlock(qutex->mutex);

  return locked;
}

cf_x_core_bool_t cf_x_sync_qutex_trylock_shared(cf_x_sync_qutex_t *qutex)
{
  assert(qutex);
  cf_x_core_bool_t locked;

  cf_x_sync_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user) {
      qutex->shared_user_count++;
      locked = cf_x_core_bool_true;
    } else {
      locked = cf_x_core_bool_false;
    }
  }
  cf_x_sync_mutex_unlock(qutex->mutex);

  return locked;
}

void cf_x_sync_qutex_unlock_exclusive(cf_x_sync_qutex_t *qutex)
{
  cf_x_sync_mutex_lock(qutex->mutex);
  qutex->exclusive_user = cf_x_core_bool_false;
  cf_x_sync_mutex_unlock(qutex->mutex);
}

void cf_x_sync_qutex_unlock_shared(cf_x_sync_qutex_t *qutex)
{
  cf_x_sync_mutex_lock(qutex->mutex);
  qutex->shared_user_count--;
  cf_x_sync_mutex_unlock(qutex->mutex);
}
