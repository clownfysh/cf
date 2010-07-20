#include "x/net/hyperversion.h"

const char *x_net_hyperversion_get_name(x_net_hyperversion_t hyperversion)
{
  const char *name;

  switch (hyperversion) {

    case X_NET_HYPERVERSION_1_0:
      name = "HTTP/1.0";
      break;

    case X_NET_HYPERVERSION_1_1:
      name = "HTTP/1.1";
      break;

    case X_NET_HYPERVERSION_UNKNOWN:
    default:
      name = "";
      break;
  }

  return name;
}
