#include "x/core/standard.h"
#include "x/core/types.h"
#include "x/net/engine_thread.h"

x_net_engine_thread_t *x_net_engine_create_thread(void *engine_object,
    unsigned short thread_index)
{
  x_net_engine_thread_t *engine_thread;

  engine_thread = malloc(sizeof *engine_thread);
  if (engine_thread) {
    engine_thread->engine_object = engine_object;
    engine_thread->thread_index = thread_index;
  }

  return engine_thread;
}

void x_net_engine_destroy_thread(x_net_engine_thread_t *engine_thread)
{
  assert(engine_thread);
  free(engine_thread);
}
