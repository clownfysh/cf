#ifndef x_core_qutex_h
#define x_core_qutex_h

#include "x/core/bool.h"
#include "x/core/mutex.h"

struct x_core_qutex_t;
typedef struct x_core_qutex_t x_core_qutex_t;

x_core_qutex_t *x_core_qutex_create();

void x_core_qutex_destroy(x_core_qutex_t *qutex);

void x_core_qutex_lock_exclusive(x_core_qutex_t *qutex);

void x_core_qutex_lock_shared(x_core_qutex_t *qutex);

x_core_bool_t x_core_qutex_trylock_exclusive(x_core_qutex_t *qutex);

x_core_bool_t x_core_qutex_trylock_shared(x_core_qutex_t *qutex);

void x_core_qutex_unlock_exclusive(x_core_qutex_t *qutex);

void x_core_qutex_unlock_shared(x_core_qutex_t *qutex);

#endif
