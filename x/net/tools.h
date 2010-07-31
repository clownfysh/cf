#ifndef cf_x_net_types_h
#define cf_x_net_types_h

#include "cf/x/core/object.h"

enum cf_x_net_maintain_t {
  CF_X_NET_MAINTAIN_CONSTANTLY = 0,
  CF_X_NET_MAINTAIN_2_SECONDS = 2,
  CF_X_NET_MAINTAIN_4_SECONDS = 4,
  CF_X_NET_MAINTAIN_1_MINUTE = 60,
  CF_X_NET_MAINTAIN_1_HOUR = 60 * 60,
};
typedef enum cf_x_net_maintain_t cf_x_net_maintain_t;

enum cf_x_net_message_status_t {
  CF_X_NET_MESSAGE_STATUS_HANDLED,
  CF_X_NET_MESSAGE_STATUS_CANT_HANDLE,
  CF_X_NET_MESSAGE_STATUS_CANT_HANDLE_NOW,
};
typedef enum cf_x_net_message_status_t cf_x_net_message_status_t;

typedef cf_x_net_message_status_t (*cf_x_net_server_system_handle_message_f)
(void *engine_object, void *message_object);

struct cf_x_net_server_system_t;
typedef struct cf_x_net_server_system_t cf_x_net_server_system_t;

#endif
