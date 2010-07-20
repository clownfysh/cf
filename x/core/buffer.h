#ifndef x_core_buffer_h
#define x_core_buffer_h

#include "x/core/bool.h"

struct x_core_buffer_t;
typedef struct x_core_buffer_t x_core_buffer_t;

x_core_bool_t x_core_buffer_append_char(x_core_buffer_t *buffer, char c);

x_core_bool_t x_core_buffer_append_string(x_core_buffer_t *buffer,
    char *string, unsigned long string_size);

void x_core_buffer_clear(x_core_buffer_t *buffer);

x_core_buffer_t *x_core_buffer_create(char *string, unsigned long string_size);

void x_core_buffer_destroy(x_core_buffer_t *buffer);

char *x_core_buffer_get_string(x_core_buffer_t *buffer,
    unsigned long *string_size);

char *x_core_buffer_take_string(x_core_buffer_t *buffer,
    unsigned long *string_size);

#endif
