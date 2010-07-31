#ifndef cf_x_core_buffer_h
#define cf_x_core_buffer_h

#include "cf/x/core/bool.h"

struct cf_x_core_buffer_t;
typedef struct cf_x_core_buffer_t cf_x_core_buffer_t;

cf_x_core_bool_t cf_x_core_buffer_append_char(cf_x_core_buffer_t *buffer, char c);

cf_x_core_bool_t cf_x_core_buffer_append_string(cf_x_core_buffer_t *buffer,
    char *string, unsigned long string_size);

void cf_x_core_buffer_clear(cf_x_core_buffer_t *buffer);

cf_x_core_buffer_t *cf_x_core_buffer_create(char *string, unsigned long string_size);

void cf_x_core_buffer_destroy(cf_x_core_buffer_t *buffer);

char *cf_x_core_buffer_get_string(cf_x_core_buffer_t *buffer,
    unsigned long *string_size);

char *cf_x_core_buffer_take_string(cf_x_core_buffer_t *buffer,
    unsigned long *string_size);

#endif
