#include "x/net/postey.h"

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
    x_net_postey_socket_closed_f socket_closed)
{
  assert(postey);

  postey->compare = compare;
  postey->create = create;
  postey->create_decoy = create_decoy;
  postey->destroy = destroy;
  postey->destroy_decoy = destroy_decoy;
  postey->get_last_receive_activity_time = get_last_receive_activity_time;
  postey->get_socket = get_socket;
  postey->get_stats = get_stats;
  postey->receive_message = receive_message;
  postey->receive_messages = receive_messages;
  postey->send_message = send_message;
  postey->send_messages = send_messages;
  postey->socket_closed = socket_closed;
}
