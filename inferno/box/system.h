#ifndef inferno_box_system_h
#define inferno_box_system_h

#include "inferno/box/coordinate.h"
#include "x/audit/log.h"

typedef void * (*inferno_box_object_get_cell_f)(void *object);

typedef void (*inferno_box_object_set_cell_f)(void *object, void *cell);

struct inferno_box_system_t;
typedef struct inferno_box_system_t inferno_box_system_t;

void inferno_box_system_add(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate, void *object);

void inferno_box_system_add_random(inferno_box_system_t *system, void *object);

inferno_box_system_t *inferno_box_system_create
(inferno_box_coordinate_t *dimension_coordinate,
    inferno_box_object_get_cell_f get_cell,
    inferno_box_object_set_cell_f set_cell,
    x_core_compare_f compare_objects, x_core_copy_f copy_object,
    x_core_destroy_f destroy_object, x_audit_log_t *log);

void inferno_box_system_destroy(inferno_box_system_t *system);

void *inferno_box_system_find(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);

void *inferno_box_system_find_random(inferno_box_system_t *system);

void *inferno_box_system_find_relative(inferno_box_system_t *system, void *object,
    inferno_box_coordinate_t *relative_coordinate);

unsigned long inferno_box_system_get_volume(inferno_box_system_t *system);

void *inferno_box_system_iterate_next(inferno_box_system_t *system);

void inferno_box_system_iterate_start(inferno_box_system_t *system);

void inferno_box_system_move_absolute(inferno_box_system_t *system, void *object,
    inferno_box_coordinate_t *destination_coordinate);

void inferno_box_system_move_relative(inferno_box_system_t *system, void *object,
    inferno_box_coordinate_t *relative_coordinate);

void inferno_box_system_remove(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);

void inferno_box_system_replace(inferno_box_system_t *system, void *destination_object,
    void *source_object);

void inferno_box_system_replace_random(inferno_box_system_t *system, void *object);

void inferno_box_system_swap(inferno_box_system_t *system, void *object_a,
    void *object_b);

#endif
