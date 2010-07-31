#ifndef cf_x_net_engine_h
#define cf_x_net_engine_h

#define CF_X_NET_ENGINE_NO_GET_NAME_FUNCTION NULL

#define CF_X_NET_ENGINE_TYPE_COUNT 5
enum cf_x_net_engine_id_t {
  CF_X_NET_ENGINE_UNKNOWN = 0,
  CF_X_NET_ENGINE_TEST = 1,
  CF_X_NET_ENGINE_PING = 2,
  CF_X_NET_ENGINE_SHELL = 3,
  CF_X_NET_ENGINE_HYPER = 4,
  CF_X_NET_ENGINE_MAX = 31,
};
typedef enum cf_x_net_engine_id_t cf_x_net_engine_id_t;

typedef char *(*cf_x_net_engine_get_name_f)(unsigned long engine_id);

char *cf_x_net_engine_get_name(unsigned long engine_id,
    cf_x_net_engine_get_name_f get_engine_name);

#endif
