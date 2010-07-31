#ifndef cf_x_net_clientsocket_h
#define cf_x_net_clientsocket_h

int cf_x_net_clientsocket_create(char *server_ip_address,
    unsigned short server_port);

void cf_x_net_clientsocket_destroy(int clientsocket);

#endif
