#ifndef x_core_period_h
#define x_core_period_h

#include "x/core/types.h"

struct x_core_period_t;
typedef struct x_core_period_t x_core_period_t;

x_core_period_t *x_core_period_create(unsigned long seconds);

void x_core_period_destroy(x_core_period_t *period);

x_core_bool_t x_core_period_once(x_core_period_t *period);

#endif
