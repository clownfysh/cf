#ifndef cf_x_net_client_socket_h
#define cf_x_net_client_socket_h

int cf_x_net_client_socket_create(char *server_ip_address,
    unsigned short server_port);

void cf_x_net_client_socket_destroy(int clientsocket);

#endif
