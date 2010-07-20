#ifndef x_net_hyperpost_h
#define x_net_hyperpost_h

#include "x/core/object.h"
#include "x/core/types.h"
#include "x/net/post_stats.h"

struct x_net_hyperpost_t;
typedef struct x_net_hyperpost_t x_net_hyperpost_t;

int x_net_hyperpost_compare(void *hyperpost_object_a,
    void *hyperpost_object_b);

void *x_net_hyperpost_create(int socket);

void *x_net_hyperpost_create_decoy(int socket);

void x_net_hyperpost_destroy(void *hyperpost_object);

void x_net_hyperpost_destroy_decoy(void *hyperpost_object);

time_t x_net_hyperpost_get_last_receive_activity_time(void *hyperpost_object);

int x_net_hyperpost_get_socket(void *hyperpost_object);

void x_net_hyperpost_get_stats(void *hyperpost_object,
    x_net_post_stats_t *post_stats);

x_core_bool_t x_net_hyperpost_is_socket_closed(void *hyperpost_object);

void *x_net_hyperpost_receive_message(void *hyperpost_object);

void x_net_hyperpost_receive_messages(void *hyperpost_object);

x_core_bool_t x_net_hyperpost_send_message(void *hyperpost_object,
    void *hypermessage_object);

void x_net_hyperpost_send_messages(void *hyperpost_object);

#endif
