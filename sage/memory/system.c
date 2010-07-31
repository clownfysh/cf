#include "cf/sage/memory/system.h"
#include "cf/x/core/tools.h"

struct cf_sage_memory_system_t {
};

cf_x_core_bool_t cf_sage_memory_system_add(cf_sage_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return cf_x_core_bool_false;
}

cf_sage_memory_system_t *cf_sage_memory_system_create(unsigned long node_count,
    unsigned short link_density)
{
  cf_sage_memory_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    cf_x_core_trace("malloc");
  }

  return system;
}

void cf_sage_memory_system_destroy(cf_sage_memory_system_t *system)
{
  assert(system);
  free(system);
}

cf_x_core_bool_t cf_sage_memory_system_find(cf_sage_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return cf_x_core_bool_false;
}

cf_x_case_array_t *cf_sage_memory_system_find_cloud(cf_sage_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max)
{
  return NULL;
}

cf_x_core_bool_t cf_sage_memory_system_remove(cf_sage_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return cf_x_core_bool_false;
}
