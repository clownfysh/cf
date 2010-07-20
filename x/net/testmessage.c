#include "x/core/engine.h"
#include "x/core/message.h"
#include "x/core/messagey.h"
#include "x/core/tools.h"
#include "x/net/testengine.h"
#include "x/net/testmessage.h"

x_core_message_t *x_net_testmessage_create(int client_socket,
    unsigned long message_type)
{
  return x_core_message_create(client_socket, X_NET_ENGINE_TEST, message_type,
      X_CORE_MESSAGEY_NULL_DATA, X_CORE_MESSAGEY_ZERO_DATA_SIZE);
}
