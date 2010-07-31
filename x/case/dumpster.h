#ifndef x_case_dumpster_h
#define x_case_dumpster_h

#include "x/case/array.h"
#include "x/case/list.h"
#include "x/case/set.h"
#include "x/core/objectey.h"

struct x_case_dumpster_t;
typedef struct x_case_dumpster_t x_case_dumpster_t;

x_core_bool_t x_case_dumpster_add(x_case_dumpster_t *dumpster,
    void *object);

x_case_dumpster_t *x_case_dumpster_create
(x_core_objectey_t *objectey);

void x_case_dumpster_destroy(x_case_dumpster_t *dumpster);

x_core_bool_t x_case_dumpster_take_objects_from_list
(x_case_dumpster_t *dumpster, x_case_list_t *list);

#endif
