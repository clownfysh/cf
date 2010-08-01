#ifndef cf_x_net_post_system_h
#define cf_x_net_post_system_h

#include "cf/x/case/list.h"
#include "cf/x/core/message.h"
#include "cf/x/net/post/stats.h"

struct cf_x_net_post_system_t;
typedef struct cf_x_net_post_system_t cf_x_net_post_system_t;

int cf_x_net_post_system_compare(void *post_object_a, void *post_object_b);

void *cf_x_net_post_system_create(int socket);

void *cf_x_net_post_system_create_decoy(int socket);

void cf_x_net_post_system_destroy(void *post_object);

void cf_x_net_post_system_destroy_decoy(void *post_object);

time_t cf_x_net_post_system_get_last_receive_activity_time(void *post_object);

int cf_x_net_post_system_get_socket(void *post_object);

void cf_x_net_post_system_get_stats(void *post_object, cf_x_net_post_stats_t *post_stats);

cf_x_core_bool_t cf_x_net_post_system_is_socket_closed(void *post_object);

void *cf_x_net_post_system_receive_message(void *post_object);

void cf_x_net_post_system_receive_messages(void *post_object);

cf_x_core_bool_t cf_x_net_post_system_send_message(void *post_object, void *message_object);

void cf_x_net_post_system_send_messages(void *post_object);

cf_x_case_list_t *cf_x_net_post_system_take_unsent_messages(cf_x_net_post_system_t *post);

#endif
