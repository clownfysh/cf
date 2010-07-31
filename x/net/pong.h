#ifndef cf_x_net_pong_h
#define cf_x_net_pong_h

#include "cf/x/core/message.h"

struct cf_x_net_pong_t;
typedef struct cf_x_net_pong_t cf_x_net_pong_t;

cf_x_core_bool_t cf_x_net_pong_add_to_message(void *pong_object,
    cf_x_core_message_t *message);

cf_x_net_pong_t *cf_x_net_pong_create(char *string);

cf_x_net_pong_t *cf_x_net_pong_create_from_message(cf_x_core_message_t *message);

void cf_x_net_pong_destroy(cf_x_net_pong_t *pong);

#endif
