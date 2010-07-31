#include "cf/inferno/mind/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_mind_system_t {
};

cf_inferno_mind_system_t *cf_inferno_mind_system_create()
{
  cf_inferno_mind_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    cf_x_core_trace("malloc");
  }

  return system;
}

void cf_inferno_mind_system_destroy(cf_inferno_mind_system_t *system)
{
  assert(system);
  free(system);
}
