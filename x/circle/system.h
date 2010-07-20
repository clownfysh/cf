#ifndef x_circle_system_h
#define x_circle_system_h

#include "x/core/tools.h"

struct x_circle_system_t;
typedef struct x_circle_system_t x_circle_system_t;

x_circle_system_t *x_circle_system_create();
void x_circle_system_destroy(x_circle_system_t *system);
x_bit_t x_circle_system_get_bit(x_circle_system_t *system);
x_core_bool_t x_circle_system_get_bool(x_circle_system_t *system);
unsigned char x_circle_system_get_char(x_circle_system_t *system);
unsigned long x_circle_system_get_long(x_circle_system_t *system);

#endif
