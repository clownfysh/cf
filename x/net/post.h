#ifndef x_net_post_h
#define x_net_post_h

#include "x/container/list.h"
#include "x/core/message.h"
#include "x/core/types.h"
#include "x/net/post_stats.h"

struct x_net_post_t;
typedef struct x_net_post_t x_net_post_t;

int x_net_post_compare(void *post_object_a, void *post_object_b);

void *x_net_post_create(int socket);

void *x_net_post_create_decoy(int socket);

void x_net_post_destroy(void *post_object);

void x_net_post_destroy_decoy(void *post_object);

time_t x_net_post_get_last_receive_activity_time(void *post_object);

int x_net_post_get_socket(void *post_object);

void x_net_post_get_stats(void *post_object, x_net_post_stats_t *post_stats);

x_core_bool_t x_net_post_is_socket_closed(void *post_object);

void *x_net_post_receive_message(void *post_object);

void x_net_post_receive_messages(void *post_object);

x_core_bool_t x_net_post_send_message(void *post_object, void *message_object);

void x_net_post_send_messages(void *post_object);

x_container_list_t *x_net_post_take_unsent_messages(x_net_post_t *post);

#endif
