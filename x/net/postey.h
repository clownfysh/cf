#ifndef x_net_postey_h
#define x_net_postey_h

#include "x/core/standard.h"
#include "x/core/object.h"
#include "x/core/types.h"
#include "x/net/post_stats.h"

typedef int (*x_net_postey_compare_f)(void *a_object, void *b_object);

typedef void *(*x_net_postey_create_f)(int socket);

typedef void *(*x_net_postey_create_decoy_f)(int socket);

typedef void (*x_net_postey_destroy_f)(void *post_object);

typedef void (*x_net_postey_destroy_decoy_f)(void *post_object);

typedef time_t (*x_net_postey_get_last_receive_activity_time_f)
(void *post_object);

typedef int (*x_net_postey_get_socket_f)(void *post_object);

typedef void (*x_net_postey_get_stats_f)(void *post_object,
    x_net_post_stats_t *post_stats);

typedef void *(*x_net_postey_receive_message_f)(void *post_object);

typedef void (*x_net_postey_receive_messages_f)(void *post_object);

typedef x_core_bool_t (*x_net_postey_send_message_f)(void *post_object,
    void *message_object);

typedef void (*x_net_postey_send_messages_f)(void *post_object);

typedef x_core_bool_t (*x_net_postey_socket_closed_f)(void *post_object);

struct x_net_postey_t {
  x_net_postey_compare_f compare;
  x_net_postey_create_f create;
  x_net_postey_create_decoy_f create_decoy;
  x_net_postey_destroy_f destroy;
  x_net_postey_destroy_decoy_f destroy_decoy;

  x_net_postey_get_last_receive_activity_time_f
  get_last_receive_activity_time;

  x_net_postey_get_socket_f get_socket;
  x_net_postey_get_stats_f get_stats;
  x_net_postey_receive_message_f receive_message;
  x_net_postey_receive_messages_f receive_messages;
  x_net_postey_send_message_f send_message;
  x_net_postey_send_messages_f send_messages;
  x_net_postey_socket_closed_f socket_closed;
};
typedef struct x_net_postey_t x_net_postey_t;

void x_net_postey_init(x_net_postey_t *postey, x_net_postey_compare_f compare,
    x_net_postey_create_f create, x_net_postey_create_decoy_f create_decoy,
    x_net_postey_destroy_f destroy, x_net_postey_destroy_decoy_f destroy_decoy,

    x_net_postey_get_last_receive_activity_time_f
    get_last_receive_activity_time,

    x_net_postey_get_socket_f get_socket, x_net_postey_get_stats_f get_stats,
    x_net_postey_receive_message_f receive_message,
    x_net_postey_receive_messages_f receive_messages,
    x_net_postey_send_message_f send_message,
    x_net_postey_send_messages_f send_messages,
    x_net_postey_socket_closed_f socket_closed);

#endif
