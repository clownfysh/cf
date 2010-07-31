#ifndef mutex_h
#define mutex_h

#include "cf/x/core/bool.h"

struct cf_x_core_mutex_t;
typedef struct cf_x_core_mutex_t cf_x_core_mutex_t;

cf_x_core_mutex_t *cf_x_core_mutex_create();

void cf_x_core_mutex_destroy(cf_x_core_mutex_t *mutex);

void cf_x_core_mutex_lock(cf_x_core_mutex_t *mutex);

cf_x_core_bool_t cf_x_core_mutex_trylock(cf_x_core_mutex_t *mutex);

void cf_x_core_mutex_unlock(cf_x_core_mutex_t *mutex);

#endif
