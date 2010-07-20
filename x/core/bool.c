#include "x/core/bool.h"
#include "x/core/message.h"

x_core_bool_t x_core_bool_add_to_message(void *bool_object,
    x_core_message_t *message)
{
  return x_core_message_add_bool(message, bool_object);
}

void *x_core_bool_create_from_message(x_core_message_t *message)
{
  return x_core_message_take_bool(message);
}
