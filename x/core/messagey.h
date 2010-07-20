#ifndef x_core_messagey_h
#define x_core_messagey_h

#include "x/core/engine.h"
#include "x/core/object.h"

#define X_CORE_MESSAGEY_NO_CLIENT_SOCKET -1
#define X_CORE_MESSAGEY_NO_TYPE 0
#define X_CORE_MESSAGEY_NULL_DATA NULL
#define X_CORE_MESSAGEY_ZERO_DATA_SIZE 0

typedef void (*x_core_messagey_destroy_f)(void *message_object);

typedef int (*x_core_messagey_get_client_socket_f)(void *message_object);

typedef x_net_engine_id_t (*x_core_messagey_get_engine_id_f)
(void *message_object);

typedef unsigned long (*x_core_messagey_get_type_f)(void *message_object);

struct x_core_messagey_t {
  x_core_messagey_destroy_f destroy;
  x_core_messagey_get_client_socket_f get_client_socket;
  x_core_messagey_get_engine_id_f get_engine_id;
  x_core_messagey_get_type_f get_type;
};
typedef struct x_core_messagey_t x_core_messagey_t;

void x_core_messagey_init(x_core_messagey_t *messagey,
    x_core_messagey_destroy_f destroy,
    x_core_messagey_get_client_socket_f get_client_socket,
    x_core_messagey_get_engine_id_f get_engine_id,
    x_core_messagey_get_type_f get_type);

#endif
