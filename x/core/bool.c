#include "cf/x/core/bool.h"
#include "cf/x/core/message.h"

cf_x_core_bool_t cf_x_core_bool_add_to_message(void *bool_object,
    cf_x_core_message_t *message)
{
  return cf_x_core_message_add_bool(message, bool_object);
}

void *cf_x_core_bool_create_from_message(cf_x_core_message_t *message)
{
  return cf_x_core_message_take_bool(message);
}
