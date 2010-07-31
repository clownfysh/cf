#ifndef cf_x_case_map_h
#define cf_x_case_map_h

#include "cf/x/core/bool.h"
#include "cf/x/core/object.h"
#include "cf/x/core/objectey.h"

#define CF_X_CASE_MAP_DESTROY cf_x_core_bool_true
#define CF_X_CASE_MAP_DONT_DESTROY cf_x_core_bool_false

struct cf_x_case_map_t;
typedef struct cf_x_case_map_t cf_x_case_map_t;

cf_x_core_bool_t cf_x_case_map_add(cf_x_case_map_t *map, void *left,
    void *right);

int cf_x_case_map_compare(void *map_object_a, void *map_object_b);

void *cf_x_case_map_copy(void *map_object);

cf_x_case_map_t *cf_x_case_map_create(cf_x_core_objectey_t *left_objectey,
    cf_x_core_objectey_t *right_objectey, cf_x_core_bool_t destroys);

void cf_x_case_map_destroy(void *map_object);

void *cf_x_case_map_find(cf_x_case_map_t *map, void *left_object_decoy);

void cf_x_case_map_print(cf_x_case_map_t *map);

cf_x_core_bool_t cf_x_case_map_remove(cf_x_case_map_t *map,
    void *left_object_decoy);

#endif
