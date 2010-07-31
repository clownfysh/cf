#ifndef cf_x_core_time_h
#define cf_x_core_time_h

#include "cf/x/core/dhms.h"

struct cf_x_core_time_t;
typedef struct cf_x_core_time_t cf_x_core_time_t;

cf_x_core_time_t *cf_x_core_time_create(unsigned long seconds);

void cf_x_core_time_destroy(cf_x_core_time_t *time);

void cf_x_core_time_get_days_hours_minutes_seconds(cf_x_core_time_t *time,
    cf_x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
