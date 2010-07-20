#include "inferno/mcontainer/system.h"
#include "h/core/tools.h"

struct inferno_mcontainer_system_t {
};

inferno_mcontainer_system_t *inferno_mcontainer_system_create()
{
  inferno_mcontainer_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_mcontainer_system_destroy(inferno_mcontainer_system_t *system)
{
  assert(system);
  free(system);
}
