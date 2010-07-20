#ifndef x_net_testmessage_h
#define x_net_testmessage_h

#include "x/net/ping.h"

#define X_NET_TESTMESSAGE_TYPE_COUNT 3
enum x_net_testmessage_t {
  X_NET_TESTMESSAGE_UNKNOWN = 0,
  X_NET_TESTMESSAGE_PING = 1,
  X_NET_TESTMESSAGE_PONG = 2
};
typedef enum x_net_testmessage_t x_net_testmessage_t;

x_core_message_t *x_net_testmessage_create(int client_socket,
    unsigned long message_type);

#endif
