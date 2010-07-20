#include "x/core/standard.h"
#include "x/core/types.h"
#include "x/net/enginey.h"

void x_net_enginey_init(x_net_enginey_t *enginey,
    x_net_enginey_create_f create, x_net_enginey_destroy_f destroy,
    x_net_enginey_get_handler_for_message_f get_handler_for_message,
    x_net_enginey_maintain_f maintain, x_net_enginey_run_f run,
    x_net_enginey_start_f start, x_net_enginey_stop_f stop)
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
