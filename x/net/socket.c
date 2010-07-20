#include "x/core/tools.h"
#include "x/net/socket.h"

ssize_t x_net_socket_receive(int socket, void *data, size_t data_size)
{
  return recv(socket, data, data_size, 0);
}

ssize_t x_net_socket_send(int socket, void *data, size_t data_size)
{
  return send(socket, data, data_size, 0);
}
