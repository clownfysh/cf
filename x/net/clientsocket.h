#ifndef x_net_clientsocket_h
#define x_net_clientsocket_h

int x_net_clientsocket_create(char *server_ip_address,
    unsigned short server_port);

void x_net_clientsocket_destroy(int clientsocket);

#endif
