#include "cf/x/core/message.h"
#include "cf/x/core/messagey.h"
#include "cf/x/pingpong/engine.h"
#include "cf/x/pingpong/message.h"

cf_x_core_message_t *cf_x_pingpong_message_create(int client_socket,
    unsigned long message_type)
{
  cf_x_core_message_t *message;
  message = cf_x_core_message_create(client_socket, CF_X_NET_ENGINE_PING,
      message_type, CF_X_CORE_MESSAGEY_NULL_DATA, CF_X_CORE_MESSAGEY_ZERO_DATA_SIZE);
  return message;
}
