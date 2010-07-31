#include "mbin.h"

#define BECOME_SIMPLE_DIVISOR 2
#define MAX_OBJECTS_PER_BIN 8

#define PRIMES_COUNT 10
static unsigned long primes[PRIMES_COUNT] = {29, 23, 19, 17, 13, 11, 7, 5, 3,
                                             2};

struct x_case_mbin_t {
  x_core_bool_t container;
  void **objects;
  x_case_mbin_t **bins;
  unsigned long object_count;
  unsigned long contained_object_count;
  unsigned long level;
  unsigned long bin_count;
  unsigned long iterator;
  x_core_bool_t iterate_remove;
  x_case_mbin_set_type_t set_type;
  x_core_objectey_t *objectey;
};

static x_case_mbin_t *create(x_core_objectey_t *objectey,
    unsigned long level, x_case_mbin_set_type_t set_type);

static x_core_bool_t create_bins(x_case_mbin_t *mbin);

static x_core_bool_t become_container(x_case_mbin_t *mbin);

static void become_simple(x_case_mbin_t *mbin);

static void destroy_bins(x_case_mbin_t *mbin);

x_case_mbin_t *create(x_core_objectey_t *objectey, unsigned long level,
    x_case_mbin_set_type_t set_type)
{
  assert(objectey);
  assert(objectey->mod);
  assert(objectey->equal);
  assert(level < PRIMES_COUNT);
  x_case_mbin_t *mbin;

  mbin = malloc(sizeof *mbin);
  if (mbin) {
    mbin->container = x_core_bool_false;
    mbin->bins = NULL;
    mbin->object_count = 0;
    mbin->contained_object_count = 0;
    mbin->objectey = objectey;
    mbin->level = level;
    mbin->bin_count = primes[level];
    mbin->set_type = set_type;
    mbin->objects = malloc((sizeof *mbin->objects) * MAX_OBJECTS_PER_BIN);
    if (!mbin->objects) {
      x_trace("malloc");
    }
  } else {
    x_trace("malloc");
  }

  return mbin;
}

x_core_bool_t create_bins(x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->bins);
  x_core_bool_t success;
  unsigned long i;

  mbin->bins = malloc((sizeof *mbin->bins) * mbin->bin_count);
  if (mbin->bins) {
    success = x_core_bool_true;
    for (i = 0; i < mbin->bin_count; i++) {
      if (mbin->level < (PRIMES_COUNT - 1)) {
        *(mbin->bins + i) = create(mbin->objectey, mbin->level + 1,
            mbin->set_type);
      } else {
        *(mbin->bins + i)
          = create(mbin->objectey, mbin->level, mbin->set_type);
        x_trace("ran out of primes");
      }
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

x_core_bool_t become_container(x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(!mbin->bins);
  assert(!mbin->container);
  x_core_bool_t success = x_core_bool_true;
  unsigned long i;
  unsigned long remainder;
  void *object;

  if (create_bins(mbin)) {
    assert(mbin->bins);
    for (i = 0; i < mbin->object_count; i++) {
      object = *(mbin->objects + i);
      remainder = mbin->objectey->mod(object, mbin->bin_count);
      if (!x_case_mbin_add(*(mbin->bins + remainder), object)) {
        x_trace("x_case_mbin_add");
        success = x_core_bool_false;
        break;
      }
    }
    if (success) {
      mbin->object_count = 0;
      mbin->container = x_core_bool_true;
    } else {
      destroy_bins(mbin);
      mbin->bins = NULL;
    }
  } else {
    success = x_core_bool_false;
    x_trace("create_bins");
    assert(!mbin->bins);
    assert(!mbin->container);
  }

  return success;
}

void become_simple(x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->bins);
  assert(mbin->container);
  assert(0 == mbin->object_count);
  unsigned long i;
  void *object;

  /*  printf("become_simple()\n");  */

  x_case_mbin_iterate_start(mbin);
  while ((object = x_case_mbin_iterate_next(mbin))) {
    assert(object);
    *(mbin->objects + mbin->object_count) = object;
    mbin->object_count++;
    assert(mbin->object_count <= MAX_OBJECTS_PER_BIN);
  }

  for (i = 0; i < mbin->bin_count; i++) {
    x_case_mbin_dont_destroy_objects(*(mbin->bins + i));
  }
  destroy_bins(mbin);
  mbin->bins = NULL;
  mbin->container = x_core_bool_false;
}

void destroy_bins(x_case_mbin_t *mbin)
{
  assert(mbin);
  assert(mbin->bins);
  unsigned long i;

  for (i = 0; i < mbin->bin_count; i++) {
    x_case_mbin_destroy(*(mbin->bins + i));
  }
  free(mbin->bins);
}

x_core_bool_t x_case_mbin_add(x_case_mbin_t *mbin, void *object)
{
  assert(mbin);
  assert(object);
  assert(mbin->object_count <= MAX_OBJECTS_PER_BIN);
  x_core_bool_t success = x_core_bool_true;
  unsigned long remainder;
  x_case_mbin_t *bin;

  if (mbin->object_count == MAX_OBJECTS_PER_BIN) {
    if (!become_container(mbin)) {
      success = x_core_bool_false;
      x_trace("become_container");
    }
  }

  if (success) {
    if (mbin->container) {
      remainder = mbin->objectey->mod(object, mbin->bin_count);
      bin = *(mbin->bins + remainder);

      if (X_CASE_MBIN_SET_TYPE_MULTISET == mbin->set_type) {
        if (x_case_mbin_add(bin, object)) {
          mbin->contained_object_count++;
        } else {
          success = x_core_bool_false;
          x_trace("x_case_mbin_add");
        }
      } else if (X_CASE_MBIN_SET_TYPE_SET == mbin->set_type) {
        if (x_case_mbin_add(bin, object)) {
          mbin->contained_object_count++;
        } else {
          success = x_core_bool_false;
        }
      }

    } else {

      if (X_CASE_MBIN_SET_TYPE_MULTISET == mbin->set_type) {
        assert(mbin->object_count < MAX_OBJECTS_PER_BIN);
        *(mbin->objects + mbin->object_count) = object;
        mbin->object_count++;
        mbin->contained_object_count++;
      } else if (X_CASE_MBIN_SET_TYPE_SET == mbin->set_type) {
        if (x_case_mbin_find(mbin, object)) {
          success = x_core_bool_false;
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

void x_case_mbin_clear(x_case_mbin_t *mbin)
{
  assert(mbin);

  x_case_mbin_iterate_start(mbin);
  while (x_case_mbin_iterate_next(mbin)) {
    x_case_mbin_iterate_remove(mbin);
  }
}

x_case_mbin_t *x_case_mbin_create(x_core_objectey_t *objectey,
    x_case_mbin_set_type_t set_type)
{
  return create(objectey, 0, set_type);
}

void x_case_mbin_destroy(x_case_mbin_t *mbin)
{
  assert(mbin);
  unsigned long i;

  if (mbin->container) {
    destroy_bins(mbin);
  } else {
    if (mbin->objectey->destroy) {
      for (i = 0; i < mbin->object_count; i++) {
        mbin->objectey->destroy(*(mbin->objects + i));
      }
    }
  }
  free(mbin->objects);
  free(mbin);
}

void x_case_mbin_dont_destroy_objects(x_case_mbin_t *mbin)
{
  assert(mbin);
  unsigned long i;

  mbin->objectey->destroy = NULL;
  if (mbin->container) {
    for (i = 0; i < mbin->bin_count; i++) {
      x_case_mbin_dont_destroy_objects(*(mbin->bins + i));
    }
  }
}

void *x_case_mbin_find(x_case_mbin_t *mbin, void *decoy_object)
{
  assert(mbin);
  assert(decoy_object);
  unsigned long i;
  unsigned long remainder;
  void *object = NULL;

  /*  printf("find()\n");  */

  if (mbin->container) {
    remainder = mbin->objectey->mod(decoy_object, mbin->bin_count);
    object = x_case_mbin_find(*(mbin->bins + remainder), decoy_object);
  } else {
    for (i = 0; i < mbin->object_count; i++) {
      if (mbin->objectey->equal(decoy_object, *(mbin->objects + i))) {
        object = *(mbin->objects + i);
        break;
      }
    }
  }

  return object;
}

x_core_objectey_t *x_case_mbin_get_objectey(x_case_mbin_t *mbin)
{
  assert(mbin);
  return mbin->objectey;
}

unsigned long x_case_mbin_get_size(x_case_mbin_t *mbin)
{
  assert(mbin);
  return mbin->contained_object_count;
}

void x_case_mbin_iterate_remove(x_case_mbin_t *mbin)
{
  assert(mbin);
  mbin->iterate_remove = x_core_bool_true;
}

void x_case_mbin_iterate_start(x_case_mbin_t *mbin)
{
  assert(mbin);

  mbin->iterator = 0;
  mbin->iterate_remove = x_core_bool_false;
  if (mbin->container) {
    x_case_mbin_iterate_start(*(mbin->bins));
  }
}

void *x_case_mbin_iterate_next(x_case_mbin_t *mbin)
{
  assert(mbin);
  void *object;

  if (mbin->container) {
    if (mbin->iterator < mbin->bin_count) {
      if (mbin->iterate_remove) {
        x_case_mbin_iterate_remove(*(mbin->bins + mbin->iterator));
        if ((*(mbin->bins + mbin->iterator))->contained_object_count > 0) {
          mbin->contained_object_count--;
        }
      }
      object = x_case_mbin_iterate_next(*(mbin->bins + mbin->iterator));
      while (!object && (mbin->iterator < (mbin->bin_count - 1))) {
        mbin->iterator++;
        x_case_mbin_iterate_start(*(mbin->bins + mbin->iterator));
        object = x_case_mbin_iterate_next(*(mbin->bins + mbin->iterator));
      }
    } else {
      object = NULL;
    }
  } else {
    if (mbin->iterate_remove) {
      assert(mbin->iterator > 0);
      assert(mbin->object_count > 0);
      assert(mbin->iterator <= mbin->object_count);
      if (mbin->objectey->destroy) {
        mbin->objectey->destroy(*(mbin->objects + (mbin->iterator - 1)));
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
      mbin->iterate_remove = x_core_bool_false;
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

x_core_bool_t x_case_mbin_remove(x_case_mbin_t *mbin,
    void *decoy_object)
{
  assert(mbin);
  assert(decoy_object);
  unsigned long remainder;
  x_core_bool_t success = x_core_bool_false;
  unsigned long i;

  if (mbin->container) {
    remainder = mbin->objectey->mod(decoy_object, mbin->bin_count);
    if (x_case_mbin_remove(*(mbin->bins + remainder), decoy_object)) {
      success = x_core_bool_true;
      mbin->contained_object_count--;
      if (mbin->contained_object_count
          < (MAX_OBJECTS_PER_BIN / BECOME_SIMPLE_DIVISOR)) {
        become_simple(mbin);
      }
    }
  } else {
    for (i = 0; i < mbin->object_count; i++) {
      if (mbin->objectey->equal(decoy_object, *(mbin->objects + i))) {
        success = x_core_bool_true;
        if (mbin->objectey->destroy) {
          mbin->objectey->destroy(*(mbin->objects + i));
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
