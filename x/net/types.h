#ifndef x_net_types_h
#define x_net_types_h

#include "x/core/object.h"

enum x_net_maintain_t {
  X_NET_MAINTAIN_CONSTANTLY = 0,
  X_NET_MAINTAIN_2_SECONDS = 2,
  X_NET_MAINTAIN_4_SECONDS = 4,
  X_NET_MAINTAIN_1_MINUTE = 60,
  X_NET_MAINTAIN_1_HOUR = 60 * 60,
};
typedef enum x_net_maintain_t x_net_maintain_t;

enum x_net_message_status_t {
  X_NET_MESSAGE_STATUS_HANDLED,
  X_NET_MESSAGE_STATUS_CANT_HANDLE,
  X_NET_MESSAGE_STATUS_CANT_HANDLE_NOW,
};
typedef enum x_net_message_status_t x_net_message_status_t;

typedef x_net_message_status_t (*x_net_server_handle_message_f)
(void *engine_object, void *message_object);

struct x_net_server_t;
typedef struct x_net_server_t x_net_server_t;

#endif
