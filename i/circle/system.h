#ifndef i_circle_system_h
#define i_circle_system_h

#include "i/tools.h"

struct i_circle_system_t;
typedef struct i_circle_system_t i_circle_system_t;

i_circle_system_t *i_circle_system_create();
void i_circle_system_destroy(i_circle_system_t *system);
i_bit_t i_circle_system_get_bit(i_circle_system_t *system);
i_bool_t i_circle_system_get_bool(i_circle_system_t *system);
unsigned char i_circle_system_get_char(i_circle_system_t *system);
unsigned long i_circle_system_get_long(i_circle_system_t *system);

#endif
