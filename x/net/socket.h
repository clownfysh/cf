#ifndef cf_x_net_socket_h
#define cf_x_net_socket_h

ssize_t cf_x_net_socket_receive(int client_socket, void *data, size_t data_size);

ssize_t cf_x_net_socket_send(int client_socket, void *data, size_t data_size);

#endif
