#ifndef cf_x_circle_system_h
#define cf_x_circle_system_h

#include "cf/x/core/tools.h"

struct cf_x_circle_system_t;
typedef struct cf_x_circle_system_t cf_x_circle_system_t;

cf_x_circle_system_t *cf_x_circle_system_create();
void cf_x_circle_system_destroy(cf_x_circle_system_t *system);
cf_x_core_bit_t cf_x_circle_system_get_bit(cf_x_circle_system_t *system);
cf_x_core_bool_t cf_x_circle_system_get_bool(cf_x_circle_system_t *system);
unsigned char cf_x_circle_system_get_char(cf_x_circle_system_t *system);
unsigned long cf_x_circle_system_get_long(cf_x_circle_system_t *system);

#endif
