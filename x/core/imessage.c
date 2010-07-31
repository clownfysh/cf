#include "cf/x/core/standard.h"
#include "cf/x/core/imessage.h"
#include "cf/x/core/types.h"

void cf_x_core_imessage_init(cf_x_core_imessage_t *messagey,
    cf_x_core_imessage_destroy_f destroy,
    cf_x_core_imessage_get_client_socket_f get_client_socket,
    cf_x_core_imessage_get_engine_id_f get_engine_id,
    cf_x_core_imessage_get_type_f get_type)
{
  assert(messagey);

  messagey->destroy = destroy;
  messagey->get_client_socket = get_client_socket;
  messagey->get_engine_id = get_engine_id;
  messagey->get_type = get_type;
}
