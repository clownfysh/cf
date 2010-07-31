#ifndef cf_x_net_http_version_h
#define cf_x_net_http_version_h

enum cf_x_net_http_version_t {
  CF_X_NET_HTTP_VERSION_UNKNOWN,
  CF_X_NET_HTTP_VERSION_1_0,
  CF_X_NET_HTTP_VERSION_1_1,
};
typedef enum cf_x_net_http_version_t cf_x_net_http_version_t;

const char *cf_x_net_http_version_get_name(cf_x_net_http_version_t hyperversion);

#endif
