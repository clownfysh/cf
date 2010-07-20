#ifndef x_net_engine_h
#define x_net_engine_h

#define X_NET_ENGINE_NO_GET_NAME_FUNCTION NULL

#define X_NET_ENGINE_TYPE_COUNT 5
enum x_net_engine_id_t {
  X_NET_ENGINE_UNKNOWN = 0,
  X_NET_ENGINE_TEST = 1,
  X_NET_ENGINE_PING = 2,
  X_NET_ENGINE_SHELL = 3,
  X_NET_ENGINE_HYPER = 4,
  X_NET_ENGINE_MAX = 31,
};
typedef enum x_net_engine_id_t x_net_engine_id_t;

typedef char *(*x_net_engine_get_name_f)(unsigned long engine_id);

char *x_net_engine_get_name(unsigned long engine_id,
    x_net_engine_get_name_f get_engine_name);

#endif
