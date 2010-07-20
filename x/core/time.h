#ifndef x_core_time_h
#define x_core_time_h

#include "x/core/days_hours_minutes_seconds.h"

struct x_core_time_t;
typedef struct x_core_time_t x_core_time_t;

x_core_time_t *x_core_time_create(unsigned long seconds);

void x_core_time_destroy(x_core_time_t *time);

void x_core_time_get_days_hours_minutes_seconds(x_core_time_t *time,
    x_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
