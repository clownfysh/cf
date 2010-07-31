#ifndef cf_x_net_post_h
#define cf_x_net_post_h

#include "cf/x/case/list.h"
#include "cf/x/core/message.h"
#include "cf/x/core/types.h"
#include "cf/x/net/post/stats.h"

struct cf_x_net_post_t;
typedef struct cf_x_net_post_t cf_x_net_post_t;

int cf_x_net_post_compare(void *post_object_a, void *post_object_b);

void *cf_x_net_post_create(int socket);

void *cf_x_net_post_create_decoy(int socket);

void cf_x_net_post_destroy(void *post_object);

void cf_x_net_post_destroy_decoy(void *post_object);

time_t cf_x_net_post_get_last_receive_activity_time(void *post_object);

int cf_x_net_post_get_socket(void *post_object);

void cf_x_net_post_get_stats(void *post_object, cf_x_net_post_stats_t *post_stats);

cf_x_core_bool_t cf_x_net_post_is_socket_closed(void *post_object);

void *cf_x_net_post_receive_message(void *post_object);

void cf_x_net_post_receive_messages(void *post_object);

cf_x_core_bool_t cf_x_net_post_send_message(void *post_object, void *message_object);

void cf_x_net_post_send_messages(void *post_object);

cf_x_case_list_t *cf_x_net_post_take_unsent_messages(cf_x_net_post_t *post);

#endif
