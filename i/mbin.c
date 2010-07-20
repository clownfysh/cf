#include "mbin.h"

#define ROOT_MAX_OBJECTS 16
#define ROOT_BIN_COUNT 2

struct i_mbin_t {
  i_bool_t container;
  void **objects;
  i_mbin_t **bins;
  i_mod_f mod;
  i_equal_f equal;
  unsigned long object_count;
  unsigned long max_objects;
  unsigned long bin_count;
};

static i_mbin_t *create(i_mod_f mod, i_equal_f equal,
    unsigned long max_objects, unsigned long bin_count);
static i_bool_t create_bins(i_mbin_t *mbin);
static i_bool_t become_container(i_mbin_t *mbin);
static i_bool_t become_simple(i_mbin_t *mbin);
static void destroy_bins(i_mbin_t *mbin);

i_mbin_t *create(i_mod_f mod, i_equal_f equal, unsigned long max_objects,
    unsigned long bin_count)
{
  assert(mod);
  assert(equal);
  i_mbin_t *mbin;

  mbin = malloc(sizeof *mbin);
  if (mbin) {
    mbin->container = i_bool_false;
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
      i_trace("malloc");
    }
  } else {
    i_trace("malloc");
  }

  return mbin;
}

i_bool_t create_bins(i_mbin_t *mbin)
{
  assert(mbin);
  i_bool_t success;
  unsigned long i;

  mbin->bins = malloc((sizeof *mbin->bins) * mbin->bin_count);
  if (mbin->bins) {
    success = i_bool_true;
    for (i = 0; i < mbin->bin_count; i++) {
      *(mbin->bins + i) = create(mbin->mod, mbin->equal, mbin->max_objects,
          mbin->bin_count * 2);
      if (!*(mbin->bins + i)) {
        i_trace("create");
        success = i_bool_false;
        break;
      }
    }
  } else {
    i_trace("malloc");
    success = i_bool_false;
  }

  return success;
}

i_bool_t become_container(i_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->container);
  i_bool_t success = i_bool_true;
  unsigned long i;
  unsigned long m;

  if (create_bins(mbin)) {
    for (i = 0; i < mbin->object_count; i++) {
      m = mbin->mod(*(mbin->objects + i));
      if (!i_mbin_add(*(mbin->bins + m), *(mbin->objects + i))) {
        i_trace("i_mbin_add");
        success = i_bool_false;
        break;
      }
    }
    memset(mbin->objects, 0, (sizeof *mbin->objects) * mbin->max_objects);
    mbin->object_count = 0;
  } else {
    i_trace("create_bins");
  }

  return success;
}

i_bool_t become_simple(i_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->container);

  destroy_bins(mbin);

  return i_bool_false;
}

void destroy_bins(i_mbin_t *mbin)
{
}

i_bool_t i_mbin_add(i_mbin_t *mbin, void *object)
{

  /*
    if we need to, then become_container
  */
  if (i_bool_false) {
    if (become_container(mbin)) {
    } else {
    }
  }

  return i_bool_false;
}

i_mbin_t *i_mbin_create(i_mod_f mod, i_equal_f equal)
{
  return create(mod, equal, ROOT_MAX_OBJECTS, ROOT_BIN_COUNT);
}

void i_mbin_destroy(i_mbin_t *mbin)
{
}

void *i_mbin_find(i_mbin_t *mbin, void *decoy_object)
{
  return NULL;
}

void i_mbin_iterate_start(i_mbin_t *mbin)
{
}

void *i_mbin_iterate_next(i_mbin_t *mbin)
{
  return NULL;
}

void *i_mbin_remove(i_mbin_t *mbin, void *decoy_object)
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
