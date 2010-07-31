#include "cf/inferno/ccsv/system.h"
#include "cf/x/core/tools.h"

struct cf_inferno_ccsv_system_t {
};

cf_inferno_ccsv_system_t *cf_inferno_ccsv_system_create()
{
  cf_inferno_ccsv_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    cf_x_core_trace("malloc");
  }

  return system;
}

void cf_inferno_ccsv_system_destroy(void *system_void)
{
  assert(system_void);
  cf_inferno_ccsv_system_t *system;

  system = system_void;

  free(system);
}
