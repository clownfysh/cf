#include "inferno/ccsv/system.h"
#include "h/core/tools.h"

struct inferno_ccsv_system_t {
};

inferno_ccsv_system_t *inferno_ccsv_system_create()
{
  inferno_ccsv_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_ccsv_system_destroy(void *system_void)
{
  assert(system_void);
  inferno_ccsv_system_t *system;

  system = system_void;

  free(system);
}
