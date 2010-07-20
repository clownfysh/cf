#ifndef x_net_serversocket_h
#define x_net_serversocket_h

#include "x/core/standard.h"

int x_net_serversocket_accept(int serversocket,
    struct sockaddr_in *client_address, socklen_t *client_address_size);

int x_net_serversocket_create(unsigned short port);

void x_net_serversocket_destroy(int serversocket);

#endif
