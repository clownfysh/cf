#ifndef x_net_socket_h
#define x_net_socket_h

ssize_t x_net_socket_receive(int client_socket, void *data, size_t data_size);

ssize_t x_net_socket_send(int client_socket, void *data, size_t data_size);

#endif
