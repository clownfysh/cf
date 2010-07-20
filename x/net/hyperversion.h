#ifndef x_net_hyperversion_h
#define x_net_hyperversion_h

enum x_net_hyperversion_t {
  X_NET_HYPERVERSION_UNKNOWN,
  X_NET_HYPERVERSION_1_0,
  X_NET_HYPERVERSION_1_1,
};
typedef enum x_net_hyperversion_t x_net_hyperversion_t;

const char *x_net_hyperversion_get_name(x_net_hyperversion_t hyperversion);

#endif
