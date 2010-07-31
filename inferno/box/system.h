#ifndef cf_inferno_box_system_h
#define cf_inferno_box_system_h

#include "cf/inferno/box/coordinate.h"
#include "cf/x/core/log.h"

typedef void * (*cf_inferno_box_object_get_cell_f)(void *object);

typedef void (*cf_inferno_box_object_set_cell_f)(void *object, void *cell);

struct cf_inferno_box_system_t;
typedef struct cf_inferno_box_system_t cf_inferno_box_system_t;

void cf_inferno_box_system_add(cf_inferno_box_system_t *system,
    cf_inferno_box_coordinate_t *coordinate, void *object);

void cf_inferno_box_system_add_random(cf_inferno_box_system_t *system, void *object);

cf_inferno_box_system_t *cf_inferno_box_system_create
(cf_inferno_box_coordinate_t *dimension_coordinate,
    cf_inferno_box_object_get_cell_f get_cell,
    cf_inferno_box_object_set_cell_f set_cell,
    cf_x_core_object_compare_f compare_objects, cf_x_core_object_copy_f copy_object,
    cf_x_core_object_destroy_f destroy_object, cf_x_core_log_t *log);

void cf_inferno_box_system_destroy(cf_inferno_box_system_t *system);

void *cf_inferno_box_system_find(cf_inferno_box_system_t *system,
    cf_inferno_box_coordinate_t *coordinate);

void *cf_inferno_box_system_find_random(cf_inferno_box_system_t *system);

void *cf_inferno_box_system_find_relative(cf_inferno_box_system_t *system, void *object,
    cf_inferno_box_coordinate_t *relative_coordinate);

unsigned long cf_inferno_box_system_get_volume(cf_inferno_box_system_t *system);

void *cf_inferno_box_system_iterate_next(cf_inferno_box_system_t *system);

void cf_inferno_box_system_iterate_start(cf_inferno_box_system_t *system);

void cf_inferno_box_system_move_absolute(cf_inferno_box_system_t *system, void *object,
    cf_inferno_box_coordinate_t *destination_coordinate);

void cf_inferno_box_system_move_relative(cf_inferno_box_system_t *system, void *object,
    cf_inferno_box_coordinate_t *relative_coordinate);

void cf_inferno_box_system_remove(cf_inferno_box_system_t *system,
    cf_inferno_box_coordinate_t *coordinate);

void cf_inferno_box_system_replace(cf_inferno_box_system_t *system, void *destination_object,
    void *source_object);

void cf_inferno_box_system_replace_random(cf_inferno_box_system_t *system, void *object);

void cf_inferno_box_system_swap(cf_inferno_box_system_t *system, void *object_a,
    void *object_b);

#endif
