#include "cf/x/case/dumpster.h"
#include "cf/x/core/tools.h"

struct cf_x_case_dumpster_t {
  cf_x_core_iobject_t *iobject;
  cf_x_case_list_t *objects;
};

cf_x_core_bool_t cf_x_case_dumpster_add(cf_x_case_dumpster_t *dumpster,
    void *object)
{
  assert(dumpster);
  assert(object);
  cf_x_core_bool_t success;

  if (cf_x_case_list_add_last(dumpster->objects, object)) {
    success = cf_x_core_bool_true;
  } else {
    success = cf_x_core_bool_false;
  }

  return success;
}

cf_x_case_dumpster_t *cf_x_case_dumpster_create
(cf_x_core_iobject_t *iobject)
{
  assert(iobject);
  cf_x_case_dumpster_t *dumpster;

  dumpster = malloc(sizeof *dumpster);
  if (dumpster) {
    dumpster->iobject = iobject;
    dumpster->objects = cf_x_case_list_create(iobject->compare,
        iobject->copy, iobject->destroy);
    if (!dumpster->objects) {
      free(dumpster);
      dumpster = NULL;
    }
  } else {
    cf_x_core_trace("malloc");
  }

  return dumpster;
}

void cf_x_case_dumpster_destroy(cf_x_case_dumpster_t *dumpster)
{
  assert(dumpster);

  cf_x_case_list_destroy(dumpster->objects);
  free(dumpster);
}

cf_x_core_bool_t cf_x_case_dumpster_take_objects_from_list
(cf_x_case_dumpster_t *dumpster, cf_x_case_list_t *list)
{
  assert(dumpster);
  assert(list);
  cf_x_core_bool_t success;
  void *object;

  success = cf_x_core_bool_true;

  cf_x_case_list_dont_destroy_objects(list);
  cf_x_case_list_iterate_start(list);
  while ((object = cf_x_case_list_iterate_next(list))) {
    if (cf_x_case_dumpster_add(dumpster, object)) {
      cf_x_case_list_iterate_remove(list);
    } else {
      success = cf_x_core_bool_false;
      cf_x_core_trace("x_case_dumpster_add_object");
      break;
    }
  }

  return success;
}
