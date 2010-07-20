#ifndef x_net_ping_h
#define x_net_ping_h

#include "x/core/message.h"

struct x_net_ping_t;
typedef struct x_net_ping_t x_net_ping_t;

x_core_bool_t x_net_ping_add_to_message(void *ping_object,
    x_core_message_t *message);

x_net_ping_t *x_net_ping_create(const char *string);

x_net_ping_t *x_net_ping_create_from_message(x_core_message_t *message);

void x_net_ping_destroy(x_net_ping_t *ping);

char *x_net_ping_get_string(x_net_ping_t *ping);

#endif
