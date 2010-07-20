#include "mbin.h"

#define ROOT_MAX_OBJECTS 16
#define ROOT_BIN_COUNT 2

struct x_mbin_t {
  x_core_bool_t container;
  void **objects;
  x_mbin_t **bins;
  x_core_mod_f mod;
  x_core_equal_f equal;
  unsigned long object_count;
  unsigned long max_objects;
  unsigned long bin_count;
};

static x_mbin_t *create(x_core_mod_f mod, x_core_equal_f equal,
    unsigned long max_objects, unsigned long bin_count);
static x_core_bool_t create_bins(x_mbin_t *mbin);
static x_core_bool_t become_container(x_mbin_t *mbin);
static x_core_bool_t become_simple(x_mbin_t *mbin);
static void destroy_bins(x_mbin_t *mbin);

x_mbin_t *create(x_core_mod_f mod, x_core_equal_f equal, unsigned long max_objects,
    unsigned long bin_count)
{
  assert(mod);
  assert(equal);
  x_mbin_t *mbin;

  mbin = malloc(sizeof *mbin);
  if (mbin) {
    mbin->container = x_core_bool_false;
    mbin->bins = NULL;
    mbin->object_count = 0;
    mbin->mod = mod;
    mbin->equal = equal;
    mbin->max_objects = max_objects;
    mbin->bin_count = bin_count;
    mbin->objects = malloc((sizeof *mbin->objects) * max_objects);
    if (mbin->objects) {
      memset(mbin->objects, 0, (sizeof *mbin->objects) * max_objects);
    } else {
      x_trace("malloc");
    }
  } else {
    x_trace("malloc");
  }

  return mbin;
}

x_core_bool_t create_bins(x_mbin_t *mbin)
{
  assert(mbin);
  x_core_bool_t success;
  unsigned long i;

  mbin->bins = malloc((sizeof *mbin->bins) * mbin->bin_count);
  if (mbin->bins) {
    success = x_core_bool_true;
    for (i = 0; i < mbin->bin_count; i++) {
      *(mbin->bins + i) = create(mbin->mod, mbin->equal, mbin->max_objects,
          mbin->bin_count * 2);
      if (!*(mbin->bins + i)) {
        x_trace("create");
        success = x_core_bool_false;
        break;
      }
    }
  } else {
    x_trace("malloc");
    success = x_core_bool_false;
  }

  return success;
}

x_core_bool_t become_container(x_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->container);
  x_core_bool_t success = x_core_bool_true;
  unsigned long i;
  unsigned long m;

  if (create_bins(mbin)) {
    for (i = 0; i < mbin->object_count; i++) {
      m = mbin->mod(*(mbin->objects + i));
      if (!x_mbin_add(*(mbin->bins + m), *(mbin->objects + i))) {
        x_trace("x_mbin_add");
        success = x_core_bool_false;
        break;
      }
    }
    memset(mbin->objects, 0, (sizeof *mbin->objects) * mbin->max_objects);
    mbin->object_count = 0;
  } else {
    x_trace("create_bins");
  }

  return success;
}

x_core_bool_t become_simple(x_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->container);

  destroy_bins(mbin);

  return x_core_bool_false;
}

void destroy_bins(x_mbin_t *mbin)
{
}

x_core_bool_t x_mbin_add(x_mbin_t *mbin, void *object)
{

  /*
    if we need to, then become_container
  */
  if (x_core_bool_false) {
    if (become_container(mbin)) {
    } else {
    }
  }

  return x_core_bool_false;
}

x_mbin_t *x_mbin_create(x_core_mod_f mod, x_core_equal_f equal)
{
  return create(mod, equal, ROOT_MAX_OBJECTS, ROOT_BIN_COUNT);
}

void x_mbin_destroy(x_mbin_t *mbin)
{
}

void *x_mbin_find(x_mbin_t *mbin, void *decoy_object)
{
  return NULL;
}

void x_mbin_iterate_start(x_mbin_t *mbin)
{
}

void *x_mbin_iterate_next(x_mbin_t *mbin)
{
  return NULL;
}

void *x_mbin_remove(x_mbin_t *mbin, void *decoy_object)
{

  /*
    the object count has to dip below, like max_objects / 2 before we do
    become_simple
  */

  if (become_simple(mbin)) {
  } else {
  }

  return NULL;
}
