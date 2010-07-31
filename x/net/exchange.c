#include "cf/x/case/set.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/exchange.h"

struct cf_x_net_exchange_t {
  cf_x_case_set_t *posts;
  cf_x_net_post_postey_t *postey;
  cf_x_core_objectey_t postey_objectey;
};

cf_x_net_exchange_t *cf_x_net_exchange_create(cf_x_net_post_postey_t *postey)
{
  assert(postey);
  cf_x_net_exchange_t *exchange;
  cf_x_core_bool_t success;

  exchange = malloc(sizeof *exchange);
  if (exchange) {
    success = cf_x_core_bool_true;
    exchange->postey = postey;
    cf_x_core_objectey_init(&exchange->postey_objectey, postey->compare,
        CF_X_CORE_OBJECT_NO_COMPARE_EQUAL_F, CF_X_CORE_OBJECT_NO_COPY_F,
        CF_X_CORE_OBJECT_NO_DESTROY_F, CF_X_CORE_OBJECT_NO_GET_AS_STRING_F,
        CF_X_CORE_OBJECT_NO_MOD_F);
    exchange->posts = cf_x_case_set_create(&exchange->postey_objectey);
    if (!exchange->posts) {
      cf_x_core_trace("x_case_set_create");
      success = cf_x_core_bool_false;
    }
  } else {
    cf_x_core_trace("malloc");
    success = cf_x_core_bool_false;
  }

  if (!success && exchange) {
    cf_x_case_set_destroy(exchange->posts);
    free(exchange);
    exchange = NULL;
  }

  return exchange;
}

void cf_x_net_exchange_destroy(cf_x_net_exchange_t *exchange)
{
  cf_x_case_set_destroy(exchange->posts);
  free(exchange);
}

unsigned long cf_x_net_exchange_get_post_count(cf_x_net_exchange_t *exchange)
{
  return cf_x_case_set_get_size(exchange->posts);
}

cf_x_core_bool_t cf_x_net_exchange_register_post(cf_x_net_exchange_t *exchange,
    void *post_object)
{
  return cf_x_case_set_add(exchange->posts, post_object);
}

void cf_x_net_exchange_send_and_receive_messages(cf_x_net_exchange_t *exchange)
{
  fd_set read_sockets;
  fd_set write_sockets;
  int max_socket;
  void *post_object;
  int socket;
  struct timeval select_timeout;

  FD_ZERO(&read_sockets);
  FD_ZERO(&write_sockets);
  max_socket = 0;
  select_timeout.tv_sec = 0;
  select_timeout.tv_usec = 0;

  cf_x_case_set_iterate_start(exchange->posts);
  while ((post_object = cf_x_case_set_iterate_next(exchange->posts))) {
    socket = exchange->postey->get_socket(post_object);
    FD_SET(socket, &read_sockets);
    FD_SET(socket, &write_sockets);
    if (socket > max_socket) {
      max_socket = socket;
    }
  }

  if (select(max_socket + 1, &read_sockets, &write_sockets, NULL,
          &select_timeout) > 0) {
    cf_x_case_set_iterate_start(exchange->posts);
    while ((post_object = cf_x_case_set_iterate_next(exchange->posts))) {
      socket = exchange->postey->get_socket(post_object);
      if (FD_ISSET(socket, &read_sockets)) {
        exchange->postey->receive_messages(post_object);
      }
      if (FD_ISSET(socket, &write_sockets)) {
        exchange->postey->send_messages(post_object);
      }
    }
  }
}

cf_x_core_bool_t cf_x_net_exchange_unregister_post(cf_x_net_exchange_t *exchange,
    int socket)
{
  void *decoy_post_object;
  cf_x_core_bool_t success;

  decoy_post_object = exchange->postey->create_decoy(socket);
  if (decoy_post_object) {
    success = cf_x_core_bool_true;
    if (!cf_x_case_set_remove(exchange->posts, decoy_post_object)) {
      cf_x_core_do_nothing();
      cf_x_core_trace("x_case_set_remove");
      success = cf_x_core_bool_false;
    }
    exchange->postey->destroy_decoy(decoy_post_object);
  } else {
    cf_x_core_trace("postey->create_decoy");
    success = cf_x_core_bool_false;
  }

  return success;
}
