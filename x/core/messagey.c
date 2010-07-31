#include "cf/x/core/standard.h"
#include "cf/x/core/messagey.h"
#include "cf/x/core/types.h"

void cf_x_core_messagey_init(cf_x_core_messagey_t *messagey,
    cf_x_core_messagey_destroy_f destroy,
    cf_x_core_messagey_get_client_socket_f get_client_socket,
    cf_x_core_messagey_get_engine_id_f get_engine_id,
    cf_x_core_messagey_get_type_f get_type)
{
  assert(messagey);

  messagey->destroy = destroy;
  messagey->get_client_socket = get_client_socket;
  messagey->get_engine_id = get_engine_id;
  messagey->get_type = get_type;
}
