#ifndef x_container_map_h
#define x_container_map_h

#include "x/core/bool.h"
#include "x/core/object.h"
#include "x/core/objectey.h"

#define X_CONTAINER_MAP_DESTROY x_core_bool_true
#define X_CONTAINER_MAP_DONT_DESTROY x_core_bool_false

struct x_container_map_t;
typedef struct x_container_map_t x_container_map_t;

x_core_bool_t x_container_map_add(x_container_map_t *map, void *left,
    void *right);

int x_container_map_compare(void *map_object_a, void *map_object_b);

void *x_container_map_copy(void *map_object);

x_container_map_t *x_container_map_create(x_core_objectey_t *left_objectey,
    x_core_objectey_t *right_objectey, x_core_bool_t destroys);

void x_container_map_destroy(void *map_object);

void *x_container_map_find(x_container_map_t *map, void *left_object_decoy);

void x_container_map_print(x_container_map_t *map);

x_core_bool_t x_container_map_remove(x_container_map_t *map,
    void *left_object_decoy);

#endif
