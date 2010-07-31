#include "mbin.h"

#define BECOME_SIMPLE_DIVISOR 2
#define MAX_OBJECTS_PER_BIN 8

#define PRIMES_COUNT 10
static unsigned long primes[PRIMES_COUNT] = {29, 23, 19, 17, 13, 11, 7, 5, 3,
                                             2};

struct cf_x_case_mbin_t {
  cf_x_core_bool_t container;
  void **objects;
  cf_x_case_mbin_t **bins;
  unsigned long object_count;
  unsigned long contained_object_count;
  unsigned long level;
  unsigned long bin_count;
  unsigned long iterator;
  cf_x_core_bool_t iterate_remove;
  cf_x_case_mbin_set_type_t set_type;
  cf_x_core_iobject_t *iobject;
};

static cf_x_case_mbin_t *create(cf_x_core_iobject_t *iobject,
    unsigned long level, cf_x_case_mbin_set_type_t set_type);

static cf_x_core_bool_t create_bins(cf_x_case_mbin_t *mbin);

static cf_x_core_bool_t become_container(cf_x_case_mbin_t *mbin);

static void become_simple(cf_x_case_mbin_t *mbin);

static void destroy_bins(cf_x_case_mbin_t *mbin);

cf_x_case_mbin_t *create(cf_x_core_iobject_t *iobject, unsigned long level,
    cf_x_case_mbin_set_type_t set_type)
{
  assert(iobject);
  assert(iobject->mod);
  assert(iobject->compare_equal);
  assert(level < PRIMES_COUNT);
  cf_x_case_mbin_t *mbin;

  mbin = malloc(sizeof *mbin);
  if (mbin) {
    mbin->container = cf_x_core_bool_false;
    mbin->bins = NULL;
    mbin->object_count = 0;
    mbin->contained_object_count = 0;
    mbin->iobject = iobject;
    mbin->level = level;
    mbin->bin_count = primes[level];
    mbin->set_type = set_type;
    mbin->objects = malloc((sizeof *mbin->objects) * MAX_OBJECTS_PER_BIN);
    if (!mbin->objects) {
      cf_x_trace("malloc");
    }
  } else {
    cf_x_trace("malloc");
  }

  return mbin;
}

cf_x_core_bool_t create_bins(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->bins);
  cf_x_core_bool_t success;
  unsigned long i;

  mbin->bins = malloc((sizeof *mbin->bins) * mbin->bin_count);
  if (mbin->bins) {
    success = cf_x_core_bool_true;
    for (i = 0; i < mbin->bin_count; i++) {
      if (mbin->level < (PRIMES_COUNT - 1)) {
        *(mbin->bins + i) = create(mbin->iobject, mbin->level + 1,
            mbin->set_type);
      } else {
        *(mbin->bins + i)
          = create(mbin->iobject, mbin->level, mbin->set_type);
        cf_x_trace("ran out of primes");
      }
      if (!*(mbin->bins + i)) {
        cf_x_trace("create");
        success = cf_x_core_bool_false;
        break;
      }
    }
  } else {
    cf_x_trace("malloc");
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_core_bool_t become_container(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->bins);
  assert(!mbin->container);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  unsigned long i;
  unsigned long remainder;
  void *object;

  if (create_bins(mbin)) {
    assert(mbin->bins);
    for (i = 0; i < mbin->object_count; i++) {
      object = *(mbin->objects + i);
      remainder = mbin->iobject->mod(object, mbin->bin_count);
      if (!cf_x_case_mbin_add(*(mbin->bins + remainder), object)) {
        cf_x_trace("x_case_mbin_add");
        success = cf_x_core_bool_false;
        break;
      }
    }
    if (success) {
      mbin->object_count = 0;
      mbin->container = cf_x_core_bool_true;
    } else {
      destroy_bins(mbin);
      mbin->bins = NULL;
    }
  } else {
    success = cf_x_core_bool_false;
    cf_x_trace("create_bins");
    assert(!mbin->bins);
    assert(!mbin->container);
  }

  return success;
}

void become_simple(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->bins);
  assert(mbin->container);
  assert(0 == mbin->object_count);
  unsigned long i;
  void *object;

  /*  printf("become_simple()\n");  */

  cf_x_case_mbin_iterate_start(mbin);
  while ((object = cf_x_case_mbin_iterate_next(mbin))) {
    assert(object);
    *(mbin->objects + mbin->object_count) = object;
    mbin->object_count++;
    assert(mbin->object_count <= MAX_OBJECTS_PER_BIN);
  }

  for (i = 0; i < mbin->bin_count; i++) {
    cf_x_case_mbin_dont_destroy_objects(*(mbin->bins + i));
  }
  destroy_bins(mbin);
  mbin->bins = NULL;
  mbin->container = cf_x_core_bool_false;
}

void destroy_bins(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->bins);
  unsigned long i;

  for (i = 0; i < mbin->bin_count; i++) {
    cf_x_case_mbin_destroy(*(mbin->bins + i));
  }
  free(mbin->bins);
}

cf_x_core_bool_t cf_x_case_mbin_add(cf_x_case_mbin_t *mbin, void *object)
{
  assert(mbin);
  assert(object);
  assert(mbin->object_count <= MAX_OBJECTS_PER_BIN);
  cf_x_core_bool_t success = cf_x_core_bool_true;
  unsigned long remainder;
  cf_x_case_mbin_t *bin;

  if (mbin->object_count == MAX_OBJECTS_PER_BIN) {
    if (!become_container(mbin)) {
      success = cf_x_core_bool_false;
      cf_x_trace("become_container");
    }
  }

  if (success) {
    if (mbin->container) {
      remainder = mbin->iobject->mod(object, mbin->bin_count);
      bin = *(mbin->bins + remainder);

      if (CF_X_CASE_MBIN_SET_TYPE_MULTISET == mbin->set_type) {
        if (cf_x_case_mbin_add(bin, object)) {
          mbin->contained_object_count++;
        } else {
          success = cf_x_core_bool_false;
          cf_x_trace("x_case_mbin_add");
        }
      } else if (CF_X_CASE_MBIN_SET_TYPE_SET == mbin->set_type) {
        if (cf_x_case_mbin_add(bin, object)) {
          mbin->contained_object_count++;
        } else {
          success = cf_x_core_bool_false;
        }
      }

    } else {

      if (CF_X_CASE_MBIN_SET_TYPE_MULTISET == mbin->set_type) {
        assert(mbin->object_count < MAX_OBJECTS_PER_BIN);
        *(mbin->objects + mbin->object_count) = object;
        mbin->object_count++;
        mbin->contained_object_count++;
      } else if (CF_X_CASE_MBIN_SET_TYPE_SET == mbin->set_type) {
        if (cf_x_case_mbin_find(mbin, object)) {
          success = cf_x_core_bool_false;
        } else {
          assert(mbin->object_count < MAX_OBJECTS_PER_BIN);
          *(mbin->objects + mbin->object_count) = object;
          mbin->object_count++;
          mbin->contained_object_count++;
        }
      }

    }
  }

  return success;
}

void cf_x_case_mbin_clear(cf_x_case_mbin_t *mbin)
{
  assert(mbin);

  cf_x_case_mbin_iterate_start(mbin);
  while (cf_x_case_mbin_iterate_next(mbin)) {
    cf_x_case_mbin_iterate_remove(mbin);
  }
}

cf_x_case_mbin_t *cf_x_case_mbin_create(cf_x_core_iobject_t *iobject,
    cf_x_case_mbin_set_type_t set_type)
{
  return create(iobject, 0, set_type);
}

void cf_x_case_mbin_destroy(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  unsigned long i;

  if (mbin->container) {
    destroy_bins(mbin);
  } else {
    if (mbin->iobject->destroy) {
      for (i = 0; i < mbin->object_count; i++) {
        mbin->iobject->destroy(*(mbin->objects + i));
      }
    }
  }
  free(mbin->objects);
  free(mbin);
}

void cf_x_case_mbin_dont_destroy_objects(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  unsigned long i;

  mbin->iobject->destroy = NULL;
  if (mbin->container) {
    for (i = 0; i < mbin->bin_count; i++) {
      cf_x_case_mbin_dont_destroy_objects(*(mbin->bins + i));
    }
  }
}

void *cf_x_case_mbin_find(cf_x_case_mbin_t *mbin, void *decoy_object)
{
  assert(mbin);
  assert(decoy_object);
  unsigned long i;
  unsigned long remainder;
  void *object = NULL;

  /*  printf("find()\n");  */

  if (mbin->container) {
    remainder = mbin->iobject->mod(decoy_object, mbin->bin_count);
    object = cf_x_case_mbin_find(*(mbin->bins + remainder), decoy_object);
  } else {
    for (i = 0; i < mbin->object_count; i++) {
      if (mbin->iobject->compare_equal(decoy_object, *(mbin->objects + i))) {
        object = *(mbin->objects + i);
        break;
      }
    }
  }

  return object;
}

cf_x_core_iobject_t *cf_x_case_mbin_get_iobject(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  return mbin->iobject;
}

unsigned long cf_x_case_mbin_get_size(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  return mbin->contained_object_count;
}

void cf_x_case_mbin_iterate_remove(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  mbin->iterate_remove = cf_x_core_bool_true;
}

void cf_x_case_mbin_iterate_start(cf_x_case_mbin_t *mbin)
{
  assert(mbin);

  mbin->iterator = 0;
  mbin->iterate_remove = cf_x_core_bool_false;
  if (mbin->container) {
    cf_x_case_mbin_iterate_start(*(mbin->bins));
  }
}

void *cf_x_case_mbin_iterate_next(cf_x_case_mbin_t *mbin)
{
  assert(mbin);
  void *object;

  if (mbin->container) {
    if (mbin->iterator < mbin->bin_count) {
      if (mbin->iterate_remove) {
        cf_x_case_mbin_iterate_remove(*(mbin->bins + mbin->iterator));
        if ((*(mbin->bins + mbin->iterator))->contained_object_count > 0) {
          mbin->contained_object_count--;
        }
      }
      object = cf_x_case_mbin_iterate_next(*(mbin->bins + mbin->iterator));
      while (!object && (mbin->iterator < (mbin->bin_count - 1))) {
        mbin->iterator++;
        cf_x_case_mbin_iterate_start(*(mbin->bins + mbin->iterator));
        object = cf_x_case_mbin_iterate_next(*(mbin->bins + mbin->iterator));
      }
    } else {
      object = NULL;
    }
  } else {
    if (mbin->iterate_remove) {
      assert(mbin->iterator > 0);
      assert(mbin->object_count > 0);
      assert(mbin->iterator <= mbin->object_count);
      if (mbin->iobject->destroy) {
        mbin->iobject->destroy(*(mbin->objects + (mbin->iterator - 1)));
      }
      if (mbin->object_count > 1) {
        *(mbin->objects + (mbin->iterator - 1))
          = *(mbin->objects + mbin->object_count - 1);
      } else {
        *(mbin->objects + (mbin->iterator - 1)) = NULL;
      }
      mbin->object_count--;
      mbin->contained_object_count--;
      mbin->iterator--;
      mbin->iterate_remove = cf_x_core_bool_false;
    }
    if (mbin->iterator < mbin->object_count) {
      object = *(mbin->objects + mbin->iterator);
      mbin->iterator++;
    } else {
      object = NULL;
    }
  }

  return object;
}

cf_x_core_bool_t cf_x_case_mbin_remove(cf_x_case_mbin_t *mbin,
    void *decoy_object)
{
  assert(mbin);
  assert(decoy_object);
  unsigned long remainder;
  cf_x_core_bool_t success = cf_x_core_bool_false;
  unsigned long i;

  if (mbin->container) {
    remainder = mbin->iobject->mod(decoy_object, mbin->bin_count);
    if (cf_x_case_mbin_remove(*(mbin->bins + remainder), decoy_object)) {
      success = cf_x_core_bool_true;
      mbin->contained_object_count--;
      if (mbin->contained_object_count
          < (MAX_OBJECTS_PER_BIN / BECOME_SIMPLE_DIVISOR)) {
        become_simple(mbin);
      }
    }
  } else {
    for (i = 0; i < mbin->object_count; i++) {
      if (mbin->iobject->compare_equal(decoy_object, *(mbin->objects + i))) {
        success = cf_x_core_bool_true;
        if (mbin->iobject->destroy) {
          mbin->iobject->destroy(*(mbin->objects + i));
        }
        if (mbin->object_count > 0) {
          *(mbin->objects + i) = *(mbin->objects + (mbin->object_count - 1));
          mbin->object_count--;
          mbin->contained_object_count--;
        }
        break;
      }
    }
  }

  return success;
}
