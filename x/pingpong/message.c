#include "x/core/message.h"
#include "x/core/messagey.h"
#include "x/pingpong/engine.h"
#include "x/pingpong/message.h"

x_core_message_t *x_pingpong_message_create(int client_socket,
    unsigned long message_type)
{
  x_core_message_t *message;
  message = x_core_message_create(client_socket, X_NET_ENGINE_PING,
      message_type, X_CORE_MESSAGEY_NULL_DATA, X_CORE_MESSAGEY_ZERO_DATA_SIZE);
  return message;
}
