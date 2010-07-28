#include "x/container/dumpster.h"
#include "x/core/tools.h"

struct x_container_dumpster_t {
  x_core_objectey_t *objectey;
  x_container_list_t *objects;
};

x_core_bool_t x_container_dumpster_add(x_container_dumpster_t *dumpster,
    void *object)
{
  assert(dumpster);
  assert(object);
  x_core_bool_t success;

  if (x_container_list_add_last(dumpster->objects, object)) {
    success = x_core_bool_true;
  } else {
    success = x_core_bool_false;
  }

  return success;
}

x_container_dumpster_t *x_container_dumpster_create
(x_core_objectey_t *objectey)
{
  assert(objectey);
  x_container_dumpster_t *dumpster;

  dumpster = malloc(sizeof *dumpster);
  if (dumpster) {
    dumpster->objectey = objectey;
    dumpster->objects = x_container_list_create(objectey->compare,
        objectey->copy, objectey->destroy);
    if (!dumpster->objects) {
      free(dumpster);
      dumpster = NULL;
    }
  } else {
    x_core_trace("malloc");
  }

  return dumpster;
}

void x_container_dumpster_destroy(x_container_dumpster_t *dumpster)
{
  assert(dumpster);

  x_container_list_destroy(dumpster->objects);
  free(dumpster);
}

x_core_bool_t x_container_dumpster_take_objects_from_list
(x_container_dumpster_t *dumpster, x_container_list_t *list)
{
  assert(dumpster);
  assert(list);
  x_core_bool_t success;
  void *object;

  success = x_core_bool_true;

  x_container_list_dont_destroy_objects(list);
  x_container_list_iterate_start(list);
  while ((object = x_container_list_iterate_next(list))) {
    if (x_container_dumpster_add(dumpster, object)) {
      x_container_list_iterate_remove(list);
    } else {
      success = x_core_bool_false;
      x_core_trace("x_container_dumpster_add_object");
      break;
    }
  }

  return success;
}
