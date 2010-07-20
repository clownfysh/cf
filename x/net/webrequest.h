#ifndef x_net_webrequest_h
#define x_net_webrequest_h

struct x_net_webrequest_t;
typedef struct x_net_webrequest_t x_net_webrequest_t;

x_net_webrequest_t *x_net_webrequest_create();

void x_net_webrequest_destroy(x_net_webrequest_t *webrequest);

#endif
