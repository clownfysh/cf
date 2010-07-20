#ifndef x_net_exchange_h
#define x_net_exchange_h

#include "x/net/postey.h"

struct x_net_exchange_t;
typedef struct x_net_exchange_t x_net_exchange_t;

x_net_exchange_t *x_net_exchange_create(x_net_postey_t *postey);

void x_net_exchange_destroy(x_net_exchange_t *exchange);

unsigned long x_net_exchange_get_post_count(x_net_exchange_t *exchange);

x_core_bool_t x_net_exchange_register_post(x_net_exchange_t *exchange,
    void *post_object);

void x_net_exchange_send_and_receive_messages(x_net_exchange_t *exchange);

x_core_bool_t x_net_exchange_unregister_post(x_net_exchange_t *exchange,
    int socket);

#endif
