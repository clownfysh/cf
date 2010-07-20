#include "inferno/stravel/system.h"
#include "x/core/tools.h"

struct inferno_stravel_system_t {
};

inferno_stravel_system_t *inferno_stravel_system_create()
{
  inferno_stravel_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    x_core_trace("malloc");
  }

  return system;
}

void inferno_stravel_system_destroy(inferno_stravel_system_t *system)
{
  assert(system);
  free(system);
}
