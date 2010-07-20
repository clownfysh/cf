#ifndef x_net_pong_h
#define x_net_pong_h

#include "x/core/message.h"

struct x_net_pong_t;
typedef struct x_net_pong_t x_net_pong_t;

x_core_bool_t x_net_pong_add_to_message(void *pong_object,
    x_core_message_t *message);

x_net_pong_t *x_net_pong_create(char *string);

x_net_pong_t *x_net_pong_create_from_message(x_core_message_t *message);

void x_net_pong_destroy(x_net_pong_t *pong);

#endif
