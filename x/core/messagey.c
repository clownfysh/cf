#include "x/core/standard.h"
#include "x/core/messagey.h"
#include "x/core/types.h"

void x_core_messagey_init(x_core_messagey_t *messagey,
    x_core_messagey_destroy_f destroy,
    x_core_messagey_get_client_socket_f get_client_socket,
    x_core_messagey_get_engine_id_f get_engine_id,
    x_core_messagey_get_type_f get_type)
{
  assert(messagey);

  messagey->destroy = destroy;
  messagey->get_client_socket = get_client_socket;
  messagey->get_engine_id = get_engine_id;
  messagey->get_type = get_type;
}
