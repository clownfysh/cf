#ifndef cf_x_net_ip_address_h
#define cf_x_net_ip_address_h

struct cf_x_net_ip_address_t;
typedef struct cf_x_net_ip_address_t cf_x_net_ip_address_t;

cf_x_net_ip_address_t *cf_x_net_ip_address_create_from_string(char *ip_address);

void cf_x_net_ip_address_destroy();

#endif
