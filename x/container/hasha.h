#ifndef x_container_hasha_h
#define x_container_hasha_h

#include "x/core/object.h"
#include "x/core/objectey.h"
#include "x/core/tools.h"

struct x_container_hasha_t;
typedef struct x_container_hasha_t x_container_hasha_t;

x_core_bool_t x_container_hasha_add(x_container_hasha_t *hasha, void *object);

void x_container_hasha_clear(x_container_hasha_t *hasha);

x_container_hasha_t *x_container_hasha_create(x_core_objectey_t *objectey);

void x_container_hasha_destroy(x_container_hasha_t *hasha);

void x_container_hasha_dont_destroy_objects(x_container_hasha_t *hasha);

void *x_container_hasha_find(x_container_hasha_t *hasha, void *decoy_object);

x_core_objectey_t *x_container_hasha_get_objectey(x_container_hasha_t *hasha);

unsigned long x_container_hasha_get_size(x_container_hasha_t *hasha);

void x_container_hasha_iterate_remove(x_container_hasha_t *hasha);

void x_container_hasha_iterate_start(x_container_hasha_t *hasha);

void *x_container_hasha_iterate_next(x_container_hasha_t *hasha);

x_core_bool_t x_container_hasha_remove(x_container_hasha_t *hasha,
    void *decoy_object);

#endif
