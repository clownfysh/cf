#include "chage/system.h"
#include "chage/atom.h"

struct chage_system_t {
  chage_atom_t *universe;
  chage_atom_t *focus;
};

chage_system_t *chage_system_create()
{
  chage_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->universe = chage_atom_create(NULL);
    if (system->universe) {
      system->focus = system->universe;
    } else {
      i_trace("chage_atom_create");
      free(system);
      system = NULL;
    }
  } else {
    i_trace("malloc");
  }

  return system;
}

void chage_system_destroy(chage_system_t *system)
{
  assert(system);
  chage_atom_destroy(system->universe);
  free(system);
}

void chage_system_spark(chage_system_t *system)
{
  assert(system);
  chage_interval_t start_interval;
  chage_interval_t interval;
  i_bool_t interval_expired = i_bool_false;
  unsigned long target_distance = 0;
  unsigned long actual_distance = 0;

  start_interval = chage_determine_interval();
  do {
    if (!chage_atom_spark_distance
        (system->focus, target_distance, actual_distance, start_interval,
            &interval_expired)) {
      i_trace("chage_atom_spark_distance");
    }
    target_distance++;
    interval = chage_determine_interval();
    if (interval != start_interval) {
      interval_expired = i_bool_true;
    }
  } while (!interval_expired);
}
