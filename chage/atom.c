#include "chage/atom.h"

struct chage_atom_t {
  chage_atom_t *container;
  chage_atom_t **contained;
  chage_interval_t last_sparked_in_interval;
};

static x_core_bool_t spark_distance_contained(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired);

static x_core_bool_t spark_distance_container(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired);

static x_core_bool_t spark_distance_peers(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired);

chage_atom_t *chage_atom_create(chage_atom_t *container)
{
  chage_atom_t *atom;

  atom = malloc(sizeof *atom);
  if (atom) {
    atom->container = container;
    atom->contained = NULL;
    atom->last_sparked_in_interval = CHAGE_INTERVAL_VOID;
  } else {
    x_trace("malloc");
  }

  return atom;
}

void chage_atom_destroy(chage_atom_t *atom)
{
  assert(atom);
  free(atom);
}

chage_atom_t *chage_atom_get_contained(chage_atom_t *atom, position_t position)
{
  return NULL;
}

chage_atom_t *chage_atom_get_container(chage_atom_t *atom)
{
  return atom->container;
}

x_core_bool_t chage_atom_spark(chage_atom_t *atom)
{
  assert(atom);

  /*  printf("sparking %p\n", atom);  */
  atom->last_sparked_in_interval = chage_determine_interval();

  return x_core_bool_true;
}

x_core_bool_t chage_atom_spark_distance(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  x_core_bool_t success = x_core_bool_true;
  chage_interval_t interval;

  if (target_distance != actual_distance) {

    if (!spark_distance_container(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = x_core_bool_false;
      x_trace("spark_distance_container");
    }
    if (*interval_expired) {
      return success;
    }

    if (!spark_distance_contained(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = x_core_bool_false;
      x_trace("spark_distance_contained");
    }
    if (*interval_expired) {
      return success;
    }

    if (!spark_distance_peers(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = x_core_bool_false;
      x_trace("spark_distance_peers");
    }
    if (*interval_expired) {
      return success;
    }

  } else {
    if (atom->last_sparked_in_interval != start_interval) {
      if (!chage_atom_spark(atom)) {
        success = x_core_bool_false;
        x_trace("chage_atom_spark");
      }
      interval = chage_determine_interval();
      if (interval != start_interval) {
        *interval_expired = x_core_bool_true;
      }
    }
  }

  return success;
}

x_core_bool_t spark_distance_contained(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  x_core_bool_t success = x_core_bool_true;
  chage_atom_t *contained_atom;
  unsigned short i;

  if (atom->contained) {
    for (i = 0; i < CHAGE_CONTAINED_ATOMS; i++) {
      contained_atom = *(atom->contained + i);
      if (contained_atom) {
        if (!chage_atom_spark_distance(contained_atom, target_distance,
                actual_distance + 1, start_interval, interval_expired)) {
          success = x_core_bool_false;
          x_trace("chage_atom_spark_distance");
        }
        if (*interval_expired) {
          break;
        }
      }
    }
  }

  return success;
}

x_core_bool_t spark_distance_container(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  x_core_bool_t success = x_core_bool_true;

  if (atom->container) {
    if (!chage_atom_spark_distance(atom->container, target_distance,
            actual_distance + 1, start_interval, interval_expired)) {
      success = x_core_bool_false;
      x_trace("chage_atom_spark_distance");
    }
  }

  return success;
}

x_core_bool_t spark_distance_peers(chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    chage_interval_t start_interval, x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  x_core_bool_t success = x_core_bool_true;

  if (atom->container) {
    if (!spark_distance_contained(atom->container, target_distance,
            actual_distance, start_interval, interval_expired)) {
      success = x_core_bool_false;
      x_trace("spark_distance_contained");
    }
  }

  return success;
}
