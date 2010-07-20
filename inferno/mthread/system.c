#include "inferno/mthread/system.h"
#include "h/core/tools.h"

struct inferno_mthread_system_t {
};

inferno_mthread_system_t *inferno_mthread_system_create()
{
  inferno_mthread_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_mthread_system_destroy(inferno_mthread_system_t *system)
{
  assert(system);
  free(system);
}
