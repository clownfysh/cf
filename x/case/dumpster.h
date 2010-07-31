#ifndef cf_x_case_dumpster_h
#define cf_x_case_dumpster_h

#include "cf/x/case/array.h"
#include "cf/x/case/list.h"
#include "cf/x/case/set.h"
#include "cf/x/core/iobject.h"

struct cf_x_case_dumpster_t;
typedef struct cf_x_case_dumpster_t cf_x_case_dumpster_t;

cf_x_core_bool_t cf_x_case_dumpster_add(cf_x_case_dumpster_t *dumpster,
    void *object);

cf_x_case_dumpster_t *cf_x_case_dumpster_create
(cf_x_core_iobject_t *iobject);

void cf_x_case_dumpster_destroy(cf_x_case_dumpster_t *dumpster);

cf_x_core_bool_t cf_x_case_dumpster_take_objects_from_list
(cf_x_case_dumpster_t *dumpster, cf_x_case_list_t *list);

#endif
