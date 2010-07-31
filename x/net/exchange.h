#ifndef cf_x_net_exchange_h
#define cf_x_net_exchange_h

#include "cf/x/net/post/ipost.h"

struct cf_x_net_exchange_t;
typedef struct cf_x_net_exchange_t cf_x_net_exchange_t;

cf_x_net_exchange_t *cf_x_net_exchange_create(cf_x_net_post_ipost_t *postey);

void cf_x_net_exchange_destroy(cf_x_net_exchange_t *exchange);

unsigned long cf_x_net_exchange_get_post_count(cf_x_net_exchange_t *exchange);

cf_x_core_bool_t cf_x_net_exchange_register_post(cf_x_net_exchange_t *exchange,
    void *post_object);

void cf_x_net_exchange_send_and_receive_messages(cf_x_net_exchange_t *exchange);

cf_x_core_bool_t cf_x_net_exchange_unregister_post(cf_x_net_exchange_t *exchange,
    int socket);

#endif
