#ifndef x_container_dumpster_h
#define x_container_dumpster_h

#include "x/container/array.h"
#include "x/container/list.h"
#include "x/container/set.h"
#include "x/core/objectey.h"

struct x_container_dumpster_t;
typedef struct x_container_dumpster_t x_container_dumpster_t;

x_core_bool_t x_container_dumpster_add(x_container_dumpster_t *dumpster,
    void *object);

x_container_dumpster_t *x_container_dumpster_create
(x_core_objectey_t *objectey);

void x_container_dumpster_destroy(x_container_dumpster_t *dumpster);

x_core_bool_t x_container_dumpster_take_objects_from_list
(x_container_dumpster_t *dumpster, x_container_list_t *list);

#endif
