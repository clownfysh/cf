#include "inferno/memory/system.h"
#include "h/core/tools.h"

struct inferno_memory_system_t {
};

h_core_bool_t inferno_memory_system_add(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return h_core_bool_false;
}

inferno_memory_system_t *inferno_memory_system_create(unsigned long node_count,
    unsigned short link_density)
{
  inferno_memory_system_t *system;

  system = malloc(sizeof *system);
  if (system) {

  } else {
    h_core_trace("malloc");
  }

  return system;
}

void inferno_memory_system_destroy(inferno_memory_system_t *system)
{
  assert(system);
  free(system);
}

h_core_bool_t inferno_memory_system_find(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return h_core_bool_false;
}

h_container_array_t *inferno_memory_system_find_cloud(inferno_memory_system_t *system,
    unsigned long base, unsigned long cloud_size_max)
{
  return NULL;
}

h_core_bool_t inferno_memory_system_remove(inferno_memory_system_t *system,
    unsigned long base, unsigned long link, unsigned long corollary)
{
  return h_core_bool_false;
}
