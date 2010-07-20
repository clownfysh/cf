#ifndef mutex_h
#define mutex_h

#include "x/core/bool.h"

struct x_core_mutex_t;
typedef struct x_core_mutex_t x_core_mutex_t;

x_core_mutex_t *x_core_mutex_create();

void x_core_mutex_destroy(x_core_mutex_t *mutex);

void x_core_mutex_lock(x_core_mutex_t *mutex);

x_core_bool_t x_core_mutex_trylock(x_core_mutex_t *mutex);

void x_core_mutex_unlock(x_core_mutex_t *mutex);

#endif
