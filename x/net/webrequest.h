#ifndef cf_x_net_webrequest_h
#define cf_x_net_webrequest_h

struct cf_x_net_webrequest_t;
typedef struct cf_x_net_webrequest_t cf_x_net_webrequest_t;

cf_x_net_webrequest_t *cf_x_net_webrequest_create();

void cf_x_net_webrequest_destroy(cf_x_net_webrequest_t *webrequest);

#endif
