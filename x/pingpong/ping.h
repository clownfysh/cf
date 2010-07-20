#ifndef x_pingpong_ping_h
#define x_pingpong_ping_h

#include "x/core/bool.h"
#include "x/core/message.h"

struct x_pingpong_ping_t;
typedef struct x_pingpong_ping_t x_pingpong_ping_t;

x_core_bool_t x_pingpong_ping_add_to_message(void *ping_object,
    x_core_message_t *message);

x_pingpong_ping_t *x_pingpong_ping_create(char *string);

x_pingpong_ping_t *x_pingpong_ping_create_from_message(x_core_message_t *message);

void x_pingpong_ping_destroy(x_pingpong_ping_t *ping);

char *x_pingpong_ping_get_string(x_pingpong_ping_t *ping);

#endif
