#include "cf/x/core/tools.h"
#include "cf/x/case/xcache/item.h"

struct cf_x_case_xcache_item_t {
  time_t birtx_time;
  unsigned long lifespan_seconds;
  cf_x_core_bool_t expired;
  cf_x_core_uuid_t *object_uuid;
  void *object;
  cf_x_core_copy_f copy;
  cf_x_core_destroy_f destroy;
};

int cf_x_case_xcache_item_compare(void *item_object_a,
    void *item_object_b)
{
  assert(item_object_a);
  assert(item_object_b);
  cf_x_case_xcache_item_t *item_a;
  cf_x_case_xcache_item_t *item_b;

  item_a = item_object_a;
  item_b = item_object_b;

  return cf_x_core_uuid_compare(item_a->object_uuid, item_b->object_uuid);
}

void *cf_x_case_xcache_item_copy(void *item_object)
{
  cf_x_core_trace_exit("TODO: implement");
}

cf_x_case_xcache_item_t *cf_x_case_xcache_item_create(cf_x_core_uuid_t *object_uuid,
    void *object, cf_x_core_copy_f copy,
    cf_x_core_destroy_f destroy, unsigned long lifespan_seconds)
{
  assert(object_uuid);
  assert(object);
  assert(copy);
  cf_x_case_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->birtx_time = time(NULL);
    item->object = object;
    item->copy = copy;
    item->destroy = destroy;
    item->lifespan_seconds = lifespan_seconds;
    item->expired = cf_x_core_bool_false;
    item->object_uuid = cf_x_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      cf_x_core_trace("x_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return item;
}

cf_x_case_xcache_item_t *cf_x_case_xcache_item_create_decoy(cf_x_core_uuid_t *object_uuid)
{
  assert(object_uuid);
  cf_x_case_xcache_item_t *item;

  item = malloc(sizeof *item);
  if (item) {
    item->object_uuid = cf_x_core_uuid_copy(object_uuid);
    if (!item->object_uuid) {
      cf_x_core_trace("x_core_uuid_copy");
      free(item);
      item = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return item;
}

void cf_x_case_xcache_item_destroy(void *item_object)
{
  assert(item_object);
  cf_x_case_xcache_item_t *item;

  item = item_object;

  cf_x_core_uuid_destroy(item->object_uuid);
  if (item->destroy) {
    item->destroy(item->object);
  }
  free(item);
}

void cf_x_case_xcache_item_destroy_decoy(void *item_object)
{
  assert(item_object);
  cf_x_case_xcache_item_t *item;

  item = item_object;

  cf_x_core_uuid_destroy(item->object_uuid);
  free(item);
}

void cf_x_case_xcache_item_expire(cf_x_case_xcache_item_t *item)
{
  item->expired = cf_x_core_bool_true;
}

void *cf_x_case_xcache_item_get_object(cf_x_case_xcache_item_t *item)
{
  return item->object;
}

void *cf_x_case_xcache_item_get_object_copy(cf_x_case_xcache_item_t *item)
{
  return item->copy(item->object);
}

cf_x_core_uuid_t *cf_x_case_xcache_item_get_object_uuid(cf_x_case_xcache_item_t *item)
{
  return item->object_uuid;
}

cf_x_core_bool_t cf_x_case_xcache_item_is_expired(void *item_object)
{
  assert(item_object);
  cf_x_case_xcache_item_t *item;
  cf_x_core_bool_t is_expired;
  time_t now;

  item = item_object;

  if (item->expired) {
    is_expired = cf_x_core_bool_true;
  } else {
    now = time(NULL);
    if ((now - item->birtx_time) > item->lifespan_seconds) {
      is_expired = cf_x_core_bool_true;
    } else {
      is_expired = cf_x_core_bool_false;
    }
  }

  return is_expired;
}
