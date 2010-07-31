#ifndef cf_x_net_post_postey_h
#define cf_x_net_post_postey_h

#include "cf/x/core/standard.h"
#include "cf/x/core/object.h"
#include "cf/x/core/types.h"
#include "cf/x/net/post/stats.h"

typedef int (*cf_x_net_post_postey_compare_f)(void *a_object, void *b_object);

typedef void *(*cf_x_net_post_postey_create_f)(int socket);

typedef void *(*cf_x_net_post_postey_create_decoy_f)(int socket);

typedef void (*cf_x_net_post_postey_destroy_f)(void *post_object);

typedef void (*cf_x_net_post_postey_destroy_decoy_f)(void *post_object);

typedef time_t (*cf_x_net_post_postey_get_last_receive_activity_time_f)
(void *post_object);

typedef int (*cf_x_net_post_postey_get_socket_f)(void *post_object);

typedef void (*cf_x_net_post_postey_get_stats_f)(void *post_object,
    cf_x_net_post_stats_t *post_stats);

typedef void *(*cf_x_net_post_postey_receive_message_f)(void *post_object);

typedef void (*cf_x_net_post_postey_receive_messages_f)(void *post_object);

typedef cf_x_core_bool_t (*cf_x_net_post_postey_send_message_f)(void *post_object,
    void *message_object);

typedef void (*cf_x_net_post_postey_send_messages_f)(void *post_object);

typedef cf_x_core_bool_t (*cf_x_net_post_postey_socket_closed_f)(void *post_object);

struct cf_x_net_post_postey_t {
  cf_x_net_post_postey_compare_f compare;
  cf_x_net_post_postey_create_f create;
  cf_x_net_post_postey_create_decoy_f create_decoy;
  cf_x_net_post_postey_destroy_f destroy;
  cf_x_net_post_postey_destroy_decoy_f destroy_decoy;

  cf_x_net_post_postey_get_last_receive_activity_time_f
  get_last_receive_activity_time;

  cf_x_net_post_postey_get_socket_f get_socket;
  cf_x_net_post_postey_get_stats_f get_stats;
  cf_x_net_post_postey_receive_message_f receive_message;
  cf_x_net_post_postey_receive_messages_f receive_messages;
  cf_x_net_post_postey_send_message_f send_message;
  cf_x_net_post_postey_send_messages_f send_messages;
  cf_x_net_post_postey_socket_closed_f socket_closed;
};
typedef struct cf_x_net_post_postey_t cf_x_net_post_postey_t;

void cf_x_net_post_postey_init(cf_x_net_post_postey_t *postey, cf_x_net_post_postey_compare_f compare,
    cf_x_net_post_postey_create_f create, cf_x_net_post_postey_create_decoy_f create_decoy,
    cf_x_net_post_postey_destroy_f destroy, cf_x_net_post_postey_destroy_decoy_f destroy_decoy,

    cf_x_net_post_postey_get_last_receive_activity_time_f
    get_last_receive_activity_time,

    cf_x_net_post_postey_get_socket_f get_socket, cf_x_net_post_postey_get_stats_f get_stats,
    cf_x_net_post_postey_receive_message_f receive_message,
    cf_x_net_post_postey_receive_messages_f receive_messages,
    cf_x_net_post_postey_send_message_f send_message,
    cf_x_net_post_postey_send_messages_f send_messages,
    cf_x_net_post_postey_socket_closed_f socket_closed);

#endif
