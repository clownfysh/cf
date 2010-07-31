#include "cf/x/core/standard.h"
#include "cf/x/core/types.h"
#include "cf/x/net/enginey.h"

void cf_x_net_enginey_init(cf_x_net_enginey_t *enginey,
    cf_x_net_enginey_create_f create, cf_x_net_enginey_destroy_f destroy,
    cf_x_net_enginey_get_handler_for_message_f get_handler_for_message,
    cf_x_net_enginey_maintain_f maintain, cf_x_net_enginey_run_f run,
    cf_x_net_enginey_start_f start, cf_x_net_enginey_stop_f stop)
{
  assert(enginey);

  enginey->create = create;
  enginey->destroy = destroy;
  enginey->get_handler_for_message = get_handler_for_message;
  enginey->maintain = maintain;
  enginey->run = run;
  enginey->start = start;
  enginey->stop = stop;
}
