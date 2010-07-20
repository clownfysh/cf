#include "inferno/cchat/system.h"
#include "h/core/tools.h"

struct inferno_cchat_system_t {
};

inferno_cchat_system_t *inferno_cchat_system_create()
{
  inferno_cchat_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_cchat_system_destroy(inferno_cchat_system_t *system)
{
  assert(system);
  free(system);
}
