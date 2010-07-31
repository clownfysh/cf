#ifndef cf_x_sync_mutex_h
#define cf_x_sync_mutex_h

#include "cf/x/core/bool.h"

struct cf_x_sync_mutex_t;
typedef struct cf_x_sync_mutex_t cf_x_sync_mutex_t;

cf_x_sync_mutex_t *cf_x_sync_mutex_create();

void cf_x_sync_mutex_destroy(cf_x_sync_mutex_t *mutex);

void cf_x_sync_mutex_lock(cf_x_sync_mutex_t *mutex);

cf_x_core_bool_t cf_x_sync_mutex_trylock(cf_x_sync_mutex_t *mutex);

void cf_x_sync_mutex_unlock(cf_x_sync_mutex_t *mutex);

#endif
