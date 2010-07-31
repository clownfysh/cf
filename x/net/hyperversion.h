#ifndef cf_x_net_hyperversion_h
#define cf_x_net_hyperversion_h

enum cf_x_net_hyperversion_t {
  CF_X_NET_HYPERVERSION_UNKNOWN,
  CF_X_NET_HYPERVERSION_1_0,
  CF_X_NET_HYPERVERSION_1_1,
};
typedef enum cf_x_net_hyperversion_t cf_x_net_hyperversion_t;

const char *cf_x_net_hyperversion_get_name(cf_x_net_hyperversion_t hyperversion);

#endif
