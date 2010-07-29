#include "x/container/set.h"
#include "x/core/tools.h"
#include "x/net/exchange.h"

struct x_net_exchange_t {
  x_container_set_t *posts;
  x_net_postey_t *postey;
  x_core_objectey_t postey_objectey;
};

x_net_exchange_t *x_net_exchange_create(x_net_postey_t *postey)
{
  assert(postey);
  x_net_exchange_t *exchange;
  x_core_bool_t success;

  exchange = malloc(sizeof *exchange);
  if (exchange) {
    success = x_core_bool_true;
    exchange->postey = postey;
    x_core_objectey_init(&exchange->postey_objectey, postey->compare,
        X_CORE_NO_COPY_FUNCTION, X_CORE_NO_DESTROY_FUNCTION,
        X_CORE_NO_EQUAL_FUNCTION, X_CORE_NO_GET_AS_STRING_FUNCTION,
        X_CORE_NO_MOD_FUNCTION);
    exchange->posts = x_container_set_create(&exchange->postey_objectey);
    if (!exchange->posts) {
      x_core_trace("x_container_set_create");
      success = x_core_bool_false;
    }
  } else {
    x_core_trace("malloc");
    success = x_core_bool_false;
  }

  if (!success && exchange) {
    x_container_set_destroy(exchange->posts);
    free(exchange);
    exchange = NULL;
  }

  return exchange;
}

void x_net_exchange_destroy(x_net_exchange_t *exchange)
{
  x_container_set_destroy(exchange->posts);
  free(exchange);
}

unsigned long x_net_exchange_get_post_count(x_net_exchange_t *exchange)
{
  return x_container_set_get_size(exchange->posts);
}

x_core_bool_t x_net_exchange_register_post(x_net_exchange_t *exchange,
    void *post_object)
{
  return x_container_set_add(exchange->posts, post_object);
}

void x_net_exchange_send_and_receive_messages(x_net_exchange_t *exchange)
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

  x_container_set_iterate_start(exchange->posts);
  while ((post_object = x_container_set_iterate_next(exchange->posts))) {
    socket = exchange->postey->get_socket(post_object);
    FD_SET(socket, &read_sockets);
    FD_SET(socket, &write_sockets);
    if (socket > max_socket) {
      max_socket = socket;
    }
  }

  if (select(max_socket + 1, &read_sockets, &write_sockets, NULL,
          &select_timeout) > 0) {
    x_container_set_iterate_start(exchange->posts);
    while ((post_object = x_container_set_iterate_next(exchange->posts))) {
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

x_core_bool_t x_net_exchange_unregister_post(x_net_exchange_t *exchange,
    int socket)
{
  void *decoy_post_object;
  x_core_bool_t success;

  decoy_post_object = exchange->postey->create_decoy(socket);
  if (decoy_post_object) {
    success = x_core_bool_true;
    if (!x_container_set_remove(exchange->posts, decoy_post_object)) {
      x_core_do_nothing();
      x_core_trace("x_container_set_remove");
      success = x_core_bool_false;
    }
    exchange->postey->destroy_decoy(decoy_post_object);
  } else {
    x_core_trace("postey->create_decoy");
    success = x_core_bool_false;
  }

  return success;
}
