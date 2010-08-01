#include "cf/x/net/post/ipost.h"

void cf_x_net_post_ipost_init(cf_x_net_post_ipost_t *ipost, cf_x_net_post_ipost_compare_f compare,
    cf_x_net_post_ipost_create_f create, cf_x_net_post_ipost_create_decoy_f create_decoy,
    cf_x_net_post_ipost_destroy_f destroy, cf_x_net_post_ipost_destroy_decoy_f destroy_decoy,

    cf_x_net_post_ipost_get_last_receive_activity_time_f
    get_last_receive_activity_time,

    cf_x_net_post_ipost_get_socket_f get_socket, cf_x_net_post_ipost_get_stats_f get_stats,
    cf_x_net_post_ipost_receive_message_f receive_message,
    cf_x_net_post_ipost_receive_messages_f receive_messages,
    cf_x_net_post_ipost_send_message_f send_message,
    cf_x_net_post_ipost_send_messages_f send_messages,
    cf_x_net_post_ipost_socket_closed_f socket_closed)
{
  assert(ipost);

  ipost->compare = compare;
  ipost->create = create;
  ipost->create_decoy = create_decoy;
  ipost->destroy = destroy;
  ipost->destroy_decoy = destroy_decoy;
  ipost->get_last_receive_activity_time = get_last_receive_activity_time;
  ipost->get_socket = get_socket;
  ipost->get_stats = get_stats;
  ipost->receive_message = receive_message;
  ipost->receive_messages = receive_messages;
  ipost->send_message = send_message;
  ipost->send_messages = send_messages;
  ipost->socket_closed = socket_closed;
}
