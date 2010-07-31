#include "cf/x/core/engine.h"
#include "cf/x/core/message.h"
#include "cf/x/core/messagey.h"
#include "cf/x/core/tools.h"
#include "cf/x/net/testengine.h"
#include "cf/x/net/testmessage.h"

cf_x_core_message_t *cf_x_net_testmessage_create(int client_socket,
    unsigned long message_type)
{
  return cf_x_core_message_create(client_socket, CF_X_NET_ENGINE_TEST, message_type,
      CF_X_CORE_MESSAGEY_NULL_DATA, CF_X_CORE_MESSAGEY_ZERO_DATA_SIZE);
}
