#ifndef cf_x_core_imessage_h
#define cf_x_core_imessage_h

#include "cf/x/core/engine.h"
#include "cf/x/core/object.h"

#define CF_X_CORE_IMESSAGE_NO_CLIENT_SOCKET -1
#define CF_X_CORE_IMESSAGE_NO_TYPE 0
#define CF_X_CORE_IMESSAGE_NULL_DATA NULL
#define CF_X_CORE_IMESSAGE_ZERO_DATA_SIZE 0

typedef void (*cf_x_core_imessage_destroy_f)(void *message_object);

typedef int (*cf_x_core_imessage_get_client_socket_f)(void *message_object);

typedef cf_x_net_engine_id_t (*cf_x_core_imessage_get_engine_id_f)
(void *message_object);

typedef unsigned long (*cf_x_core_imessage_get_type_f)(void *message_object);

struct cf_x_core_imessage_t {
  cf_x_core_imessage_destroy_f destroy;
  cf_x_core_imessage_get_client_socket_f get_client_socket;
  cf_x_core_imessage_get_engine_id_f get_engine_id;
  cf_x_core_imessage_get_type_f get_type;
};
typedef struct cf_x_core_imessage_t cf_x_core_imessage_t;

void cf_x_core_imessage_init(cf_x_core_imessage_t *messagey,
    cf_x_core_imessage_destroy_f destroy,
    cf_x_core_imessage_get_client_socket_f get_client_socket,
    cf_x_core_imessage_get_engine_id_f get_engine_id,
    cf_x_core_imessage_get_type_f get_type);

#endif
