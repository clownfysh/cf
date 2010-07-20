#ifndef x_pingpong_pong_h
#define x_pingpong_pong_h

#include "x/core/bool.h"
#include "x/core/message.h"

struct x_pingpong_pong_t;
typedef struct x_pingpong_pong_t x_pingpong_pong_t;

x_core_bool_t x_pingpong_pong_add_to_message(void *pong_object,
    x_core_message_t *message);

x_pingpong_pong_t *x_pingpong_pong_create(char *string);

x_pingpong_pong_t *x_pingpong_pong_create_from_message(x_core_message_t *message);

void x_pingpong_pong_destroy(x_pingpong_pong_t *pong);

#endif
