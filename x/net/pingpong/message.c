#include "cf/x/core/message.h"
#include "cf/x/core/imessage.h"
#include "cf/x/net/pingpong/engine.h"
#include "cf/x/net/pingpong/message.h"

cf_x_core_message_t *cf_x_pingpong_message_create(int client_socket,
    unsigned long message_type)
{
  cf_x_core_message_t *message;
  message = cf_x_core_message_create(client_socket, CF_X_NET_ENGINE_PING,
      message_type, CF_X_CORE_IMESSAGE_NULL_DATA, CF_X_CORE_IMESSAGE_ZERO_DATA_SIZE);
  return message;
}
