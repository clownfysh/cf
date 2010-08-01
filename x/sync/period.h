#ifndef cf_x_sync_period_h
#define cf_x_sync_period_h

#include "cf/x/core/bool.h"

struct cf_x_sync_period_t;
typedef struct cf_x_sync_period_t cf_x_sync_period_t;

cf_x_sync_period_t *cf_x_sync_period_create(unsigned long seconds);

void cf_x_sync_period_destroy(cf_x_sync_period_t *period);

cf_x_core_bool_t cf_x_sync_period_once(cf_x_sync_period_t *period);

#endif
