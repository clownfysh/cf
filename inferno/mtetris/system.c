#include "inferno/mtetris/system.h"
#include "h/core/tools.h"

struct inferno_mtetris_system_t {
};

inferno_mtetris_system_t *inferno_mtetris_system_create()
{
  inferno_mtetris_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_mtetris_system_destroy(inferno_mtetris_system_t *system)
{
  assert(system);
  free(system);
}
