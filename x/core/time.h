#ifndef cf_x_core_time_h
#define cf_x_core_time_h

#include "cf/x/core/dayhour.h"

struct cf_x_core_time_t;
typedef struct cf_x_core_time_t cf_x_core_time_t;

cf_x_core_time_t *cf_x_core_time_create(unsigned long seconds);

void cf_x_core_time_destroy(cf_x_core_time_t *time);

void cf_x_core_time_get_dayhour(cf_x_core_time_t *time,
    cf_x_core_dayhour_t *dayhour);

#endif
