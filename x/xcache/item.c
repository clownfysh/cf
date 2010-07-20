#include "x/core/tools.h"
#include "x/xcache/item.h"

struct x_xcache_item_t {
  time_t birtx_time;
  unsigned long lifespan_seconds;
  x_core_bool_t expired;
  x_core_uuid_t *object_uuid;
  void *object;
  x_core_copy_f copy;
  x_core_destroy_f destroy;
};

int x_xcache_item_compare(void *item_object_a,
    void *item_object_b)
{
  assert(item_object_a);
  assert(item_object_b);
  x_xcache_item_t *item_a;
  x_xcache_item_t *item_b;

  item_a = item_object_a;
  item_b = item_object_b;

  return x_core_uuid_compare(item_a->object_uuid, item_b->object_uuid);
}

void *x_xcache_item_copy(void *item_object)
{
  x_core_trace_exit("TODO: implement");
}

x_xcache_item_t *x_xcache_item_create(x_core_uuid_t *object_uuid,
    void *object, x_core_copy_f copy,
    x_core_destroy_f destroy, unsigned long lifespan_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  x_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->birtx_time = time(NULL);
    item->object = object;
    item->copy = copy;
    item->destroy = destroy;
    item->lifespan_seconds = lifespan_seconds;
    item->expired = x_core_bool_false;
    item->object_uuid = x_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      x_core_trace("x_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return item;
}

x_xcache_item_t *x_xcache_item_create_decoy(x_core_uuid_t *object_uuid)
{
  assert(object_uuid);
  x_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->object_uuid = x_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      x_core_trace("x_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return item;
}

void x_xcache_item_destroy(void *item_object)
{
  assert(item_object);
  x_xcache_item_t *item;

  item = item_object;

  x_core_uuid_destroy(item->object_uuid);
  if (item->destroy) {
    item->destroy(item->object);
  }
  free(item);
}

void x_xcache_item_destroy_decoy(void *item_object)
{
  assert(item_object);
  x_xcache_item_t *item;

  item = item_object;

  x_core_uuid_destroy(item->object_uuid);
  free(item);
}

void x_xcache_item_expire(x_xcache_item_t *item)
{
  item->expired = x_core_bool_true;
}

void *x_xcache_item_get_object(x_xcache_item_t *item)
{
  return item->object;
}

void *x_xcache_item_get_object_copy(x_xcache_item_t *item)
{
  return item->copy(item->object);
}

x_core_uuid_t *x_xcache_item_get_object_uuid(x_xcache_item_t *item)
{
  return item->object_uuid;
}

x_core_bool_t x_xcache_item_is_expired(void *item_object)
{
  assert(item_object);
  x_xcache_item_t *item;
  x_core_bool_t is_expired;
  time_t now;

  item = item_object;

  if (item->expired) {
    is_expired = x_core_bool_true;
  } else {
    now = time(NULL);
    if ((now - item->birtx_time) > item->lifespan_seconds) {
      is_expired = x_core_bool_true;
    } else {
      is_expired = x_core_bool_false;
    }
  }

  return is_expired;
}
