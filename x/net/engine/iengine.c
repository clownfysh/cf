#include "cf/x/core/standard.h"
#include "cf/x/net/engine/iengine.h"

void cf_x_net_engine_iengine_init(cf_x_net_engine_iengine_t *iengine,
    cf_x_net_engine_iengine_create_f create,
    cf_x_net_engine_iengine_destroy_f destroy,
    cf_x_net_engine_iengine_get_handler_for_message_f get_handler_for_message,
    cf_x_net_engine_iengine_maintain_f maintain,
    cf_x_net_engine_iengine_run_f run, cf_x_net_engine_iengine_start_f start,
    cf_x_net_engine_iengine_stop_f stop)
{
  assert(iengine);

  iengine->create = create;
  iengine->destroy = destroy;
  iengine->get_handler_for_message = get_handler_for_message;
  iengine->maintain = maintain;
  iengine->run = run;
  iengine->start = start;
  iengine->stop = stop;
}
