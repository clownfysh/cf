#ifndef cf_x_net_testmessage_h
#define cf_x_net_testmessage_h

#include "cf/x/net/ping.h"

#define CF_X_NET_TESTMESSAGE_TYPE_COUNT 3
enum cf_x_net_testmessage_t {
  CF_X_NET_TESTMESSAGE_UNKNOWN = 0,
  CF_X_NET_TESTMESSAGE_PING = 1,
  CF_X_NET_TESTMESSAGE_PONG = 2
};
typedef enum cf_x_net_testmessage_t cf_x_net_testmessage_t;

cf_x_core_message_t *cf_x_net_testmessage_create(int client_socket,
    unsigned long message_type);

#endif
