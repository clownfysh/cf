#include "cf/x/net/http/version.h"

const char *cf_x_net_http_version_get_name(cf_x_net_http_version_t hyperversion)
{
  const char *name;

  switch (hyperversion) {

    case CF_X_NET_HTTP_VERSION_1_0:
      name = "HTTP/1.0";
      break;

    case CF_X_NET_HTTP_VERSION_1_1:
      name = "HTTP/1.1";
      break;

    case CF_X_NET_HTTP_VERSION_UNKNOWN:
    default:
      name = "";
      break;
  }

  return name;
}
