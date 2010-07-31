#ifndef cf_x_net_hyperpost_h
#define cf_x_net_hyperpost_h

#include "cf/x/core/object.h"
#include "cf/x/core/types.h"
#include "cf/x/net/post_stats.h"

struct cf_x_net_hyperpost_t;
typedef struct cf_x_net_hyperpost_t cf_x_net_hyperpost_t;

int cf_x_net_hyperpost_compare(void *hyperpost_object_a,
    void *hyperpost_object_b);

void *cf_x_net_hyperpost_create(int socket);

void *cf_x_net_hyperpost_create_decoy(int socket);

void cf_x_net_hyperpost_destroy(void *hyperpost_object);

void cf_x_net_hyperpost_destroy_decoy(void *hyperpost_object);

time_t cf_x_net_hyperpost_get_last_receive_activity_time(void *hyperpost_object);

int cf_x_net_hyperpost_get_socket(void *hyperpost_object);

void cf_x_net_hyperpost_get_stats(void *hyperpost_object,
    cf_x_net_post_stats_t *post_stats);

cf_x_core_bool_t cf_x_net_hyperpost_is_socket_closed(void *hyperpost_object);

void *cf_x_net_hyperpost_receive_message(void *hyperpost_object);

void cf_x_net_hyperpost_receive_messages(void *hyperpost_object);

cf_x_core_bool_t cf_x_net_hyperpost_send_message(void *hyperpost_object,
    void *hypermessage_object);

void cf_x_net_hyperpost_send_messages(void *hyperpost_object);

#endif
