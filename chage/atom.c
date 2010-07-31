#include "cf/chage/atom.h"

struct cf_chage_atom_t {
  cf_chage_atom_t *container;
  cf_chage_atom_t **contained;
  cf_chage_interval_t last_sparked_in_interval;
};

static cf_x_core_bool_t spark_distance_contained(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired);

static cf_x_core_bool_t spark_distance_container(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired);

static cf_x_core_bool_t spark_distance_peers(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired);

cf_chage_atom_t *cf_chage_atom_create(cf_chage_atom_t *container)
{
  cf_chage_atom_t *atom;

  atom = malloc(sizeof *atom);
  if (atom) {
    atom->container = container;
    atom->contained = NULL;
    atom->last_sparked_in_interval = CF_CHAGE_INTERVAL_VOID;
  } else {
    cf_x_trace("malloc");
  }

  return atom;
}

void cf_chage_atom_destroy(cf_chage_atom_t *atom)
{
  assert(atom);
  free(atom);
}

cf_chage_atom_t *cf_chage_atom_get_contained(cf_chage_atom_t *atom, position_t position)
{
  return NULL;
}

cf_chage_atom_t *cf_chage_atom_get_container(cf_chage_atom_t *atom)
{
  return atom->container;
}

cf_x_core_bool_t cf_chage_atom_spark(cf_chage_atom_t *atom)
{
  assert(atom);

  /*  printf("sparking %p\n", atom);  */
  atom->last_sparked_in_interval = cf_chage_determine_interval();

  return cf_x_core_bool_true;
}

cf_x_core_bool_t cf_chage_atom_spark_distance(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  cf_chage_interval_t interval;

  if (target_distance != actual_distance) {

    if (!spark_distance_container(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = cf_x_core_bool_false;
      cf_x_trace("spark_distance_container");
    }
    if (*interval_expired) {
      return success;
    }

    if (!spark_distance_contained(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = cf_x_core_bool_false;
      cf_x_trace("spark_distance_contained");
    }
    if (*interval_expired) {
      return success;
    }

    if (!spark_distance_peers(atom, target_distance, actual_distance,
            start_interval, interval_expired)) {
      success = cf_x_core_bool_false;
      cf_x_trace("spark_distance_peers");
    }
    if (*interval_expired) {
      return success;
    }

  } else {
    if (atom->last_sparked_in_interval != start_interval) {
      if (!cf_chage_atom_spark(atom)) {
        success = cf_x_core_bool_false;
        cf_x_trace("chage_atom_spark");
      }
      interval = cf_chage_determine_interval();
      if (interval != start_interval) {
        *interval_expired = cf_x_core_bool_true;
      }
    }
  }

  return success;
}

cf_x_core_bool_t spark_distance_contained(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  cf_chage_atom_t *contained_atom;
  unsigned short i;

  if (atom->contained) {
    for (i = 0; i < CF_CHAGE_CONTAINED_ATOMS; i++) {
      contained_atom = *(atom->contained + i);
      if (contained_atom) {
        if (!cf_chage_atom_spark_distance(contained_atom, target_distance,
                actual_distance + 1, start_interval, interval_expired)) {
          success = cf_x_core_bool_false;
          cf_x_trace("chage_atom_spark_distance");
        }
        if (*interval_expired) {
          break;
        }
      }
    }
  }

  return success;
}

cf_x_core_bool_t spark_distance_container(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  cf_x_core_bool_t success = cf_x_core_bool_true;

  if (atom->container) {
    if (!cf_chage_atom_spark_distance(atom->container, target_distance,
            actual_distance + 1, start_interval, interval_expired)) {
      success = cf_x_core_bool_false;
      cf_x_trace("chage_atom_spark_distance");
    }
  }

  return success;
}

cf_x_core_bool_t spark_distance_peers(cf_chage_atom_t *atom,
    unsigned long target_distance, unsigned long actual_distance,
    cf_chage_interval_t start_interval, cf_x_core_bool_t *interval_expired)
{
  assert(atom);
  assert(interval_expired);
  cf_x_core_bool_t success = cf_x_core_bool_true;

  if (atom->container) {
    if (!spark_distance_contained(atom->container, target_distance,
            actual_distance, start_interval, interval_expired)) {
      success = cf_x_core_bool_false;
      cf_x_trace("spark_distance_contained");
    }
  }

  return success;
}
