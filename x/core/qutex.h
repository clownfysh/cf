#ifndef cf_x_core_qutex_h
#define cf_x_core_qutex_h

#include "cf/x/core/bool.h"
#include "cf/x/core/mutex.h"

struct cf_x_core_qutex_t;
typedef struct cf_x_core_qutex_t cf_x_core_qutex_t;

cf_x_core_qutex_t *cf_x_core_qutex_create();

void cf_x_core_qutex_destroy(cf_x_core_qutex_t *qutex);

void cf_x_core_qutex_lock_exclusive(cf_x_core_qutex_t *qutex);

void cf_x_core_qutex_lock_shared(cf_x_core_qutex_t *qutex);

cf_x_core_bool_t cf_x_core_qutex_trylock_exclusive(cf_x_core_qutex_t *qutex);

cf_x_core_bool_t cf_x_core_qutex_trylock_shared(cf_x_core_qutex_t *qutex);

void cf_x_core_qutex_unlock_exclusive(cf_x_core_qutex_t *qutex);

void cf_x_core_qutex_unlock_shared(cf_x_core_qutex_t *qutex);

#endif
