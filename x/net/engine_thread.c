#include "cf/x/core/standard.h"
#include "cf/x/core/types.h"
#include "cf/x/net/engine_thread.h"

cf_x_net_engine_thread_t *cf_x_net_engine_create_thread(void *engine_object,
    unsigned short thread_index)
{
  cf_x_net_engine_thread_t *engine_thread;

  engine_thread = malloc(sizeof *engine_thread);
  if (engine_thread) {
    engine_thread->engine_object = engine_object;
    engine_thread->thread_index = thread_index;
  }

  return engine_thread;
}

void cf_x_net_engine_destroy_thread(cf_x_net_engine_thread_t *engine_thread)
{
  assert(engine_thread);
  free(engine_thread);
}
