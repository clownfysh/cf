#include "cf/chage/system.h"
#include "cf/chage/atom.h"

struct cf_chage_system_t {
  cf_chage_atom_t *universe;
  cf_chage_atom_t *focus;
};

cf_chage_system_t *cf_chage_system_create()
{
  cf_chage_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->universe = cf_chage_atom_create(NULL);
    if (system->universe) {
      system->focus = system->universe;
    } else {
      cf_x_trace("chage_atom_create");
      free(system);
      system = NULL;
    }
  } else {
    cf_x_trace("malloc");
  }

  return system;
}

void cf_chage_system_destroy(cf_chage_system_t *system)
{
  assert(system);
  cf_chage_atom_destroy(system->universe);
  free(system);
}

void cf_chage_system_spark(cf_chage_system_t *system)
{
  assert(system);
  cf_chage_interval_t start_interval;
  cf_chage_interval_t interval;
  cf_x_core_bool_t interval_expired = cf_x_core_bool_false;
  unsigned long target_distance = 0;
  unsigned long actual_distance = 0;

  start_interval = cf_chage_determine_interval();
  do {
    if (!cf_chage_atom_spark_distance
        (system->focus, target_distance, actual_distance, start_interval,
            &interval_expired)) {
      cf_x_trace("chage_atom_spark_distance");
    }
    target_distance++;
    interval = cf_chage_determine_interval();
    if (interval != start_interval) {
      interval_expired = cf_x_core_bool_true;
    }
  } while (!interval_expired);
}
