#ifndef x_net_ip_address_h
#define x_net_ip_address_h

struct x_net_ip_address_t;
typedef struct x_net_ip_address_t x_net_ip_address_t;

x_net_ip_address_t *x_net_ip_address_create_from_string(char *ip_address);

void x_net_ip_address_destroy();

#endif
