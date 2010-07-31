#ifndef cf_x_net_ping_h
#define cf_x_net_ping_h

#include "cf/x/core/message.h"

struct cf_x_net_ping_t;
typedef struct cf_x_net_ping_t cf_x_net_ping_t;

cf_x_core_bool_t cf_x_net_ping_add_to_message(void *ping_object,
    cf_x_core_message_t *message);

cf_x_net_ping_t *cf_x_net_ping_create(const char *string);

cf_x_net_ping_t *cf_x_net_ping_create_from_message(cf_x_core_message_t *message);

void cf_x_net_ping_destroy(cf_x_net_ping_t *ping);

char *cf_x_net_ping_get_string(cf_x_net_ping_t *ping);

#endif
