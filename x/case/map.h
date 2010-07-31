#ifndef x_case_map_h
#define x_case_map_h

#include "x/core/bool.h"
#include "x/core/object.h"
#include "x/core/objectey.h"

#define X_CASE_MAP_DESTROY x_core_bool_true
#define X_CASE_MAP_DONT_DESTROY x_core_bool_false

struct x_case_map_t;
typedef struct x_case_map_t x_case_map_t;

x_core_bool_t x_case_map_add(x_case_map_t *map, void *left,
    void *right);

int x_case_map_compare(void *map_object_a, void *map_object_b);

void *x_case_map_copy(void *map_object);

x_case_map_t *x_case_map_create(x_core_objectey_t *left_objectey,
    x_core_objectey_t *right_objectey, x_core_bool_t destroys);

void x_case_map_destroy(void *map_object);

void *x_case_map_find(x_case_map_t *map, void *left_object_decoy);

void x_case_map_print(x_case_map_t *map);

x_core_bool_t x_case_map_remove(x_case_map_t *map,
    void *left_object_decoy);

#endif
