#ifndef x_container_mbin_h
#define x_container_mbin_h

#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/tools.h"

enum x_container_mbin_set_type_t {
  X_CONTAINER_MBIN_SET_TYPE_MULTISET,
  X_CONTAINER_MBIN_SET_TYPE_SET
};
typedef enum x_container_mbin_set_type_t x_container_mbin_set_type_t;

struct x_container_mbin_t;
typedef struct x_container_mbin_t x_container_mbin_t;

x_core_bool_t x_container_mbin_add(x_container_mbin_t *mbin, void *object);

void x_container_mbin_clear(x_container_mbin_t *mbin);

x_container_mbin_t *x_container_mbin_create(x_core_objectey_t *objectey,
    x_container_mbin_set_type_t set_type);

void x_container_mbin_destroy(x_container_mbin_t *mbin);

void x_container_mbin_dont_destroy_objects(x_container_mbin_t *mbin);

void *x_container_mbin_find(x_container_mbin_t *mbin, void *decoy_object);

x_core_objectey_t *x_container_mbin_get_objectey(x_container_mbin_t *mbin);

unsigned long x_container_mbin_get_size(x_container_mbin_t *mbin);

void x_container_mbin_iterate_remove(x_container_mbin_t *mbin);

void x_container_mbin_iterate_start(x_container_mbin_t *mbin);

void *x_container_mbin_iterate_next(x_container_mbin_t *mbin);

x_core_bool_t x_container_mbin_remove(x_container_mbin_t *mbin,
    void *decoy_object);

#endif
