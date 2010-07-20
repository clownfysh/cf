#include "inferno/mind/system.h"
#include "h/core/tools.h"

struct inferno_mind_system_t {
};

inferno_mind_system_t *inferno_mind_system_create()
{
  inferno_mind_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_mind_system_destroy(inferno_mind_system_t *system)
{
  assert(system);
  free(system);
}
