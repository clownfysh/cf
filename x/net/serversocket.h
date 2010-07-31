#ifndef cf_x_net_serversocket_h
#define cf_x_net_serversocket_h

#include "cf/x/core/standard.h"

int cf_x_net_serversocket_accept(int serversocket,
    struct sockaddr_in *client_address, socklen_t *client_address_size);

int cf_x_net_serversocket_create(unsigned short port);

void cf_x_net_serversocket_destroy(int serversocket);

#endif
