#ifndef cf_sage_box_system_h
#define cf_sage_box_system_h

#include "cf/sage/box/coordinate.h"
#include "cf/x/audit/log.h"

typedef void * (*cf_sage_box_object_get_cell_f)(void *object);

typedef void (*cf_sage_box_object_set_cell_f)(void *object, void *cell);

struct cf_sage_box_system_t;
typedef struct cf_sage_box_system_t cf_sage_box_system_t;

void cf_sage_box_system_add(cf_sage_box_system_t *system,
    cf_sage_box_coordinate_t *coordinate, void *object);

void cf_sage_box_system_add_random(cf_sage_box_system_t *system, void *object);

cf_sage_box_system_t *cf_sage_box_system_create
(cf_sage_box_coordinate_t *dimension_coordinate,
    cf_sage_box_object_get_cell_f get_cell,
    cf_sage_box_object_set_cell_f set_cell,
    cf_x_core_compare_f compare_objects, cf_x_core_copy_f copy_object,
    cf_x_core_destroy_f destroy_object, cf_x_audit_log_t *log);

void cf_sage_box_system_destroy(cf_sage_box_system_t *system);

void *cf_sage_box_system_find(cf_sage_box_system_t *system,
    cf_sage_box_coordinate_t *coordinate);

void *cf_sage_box_system_find_random(cf_sage_box_system_t *system);

void *cf_sage_box_system_find_relative(cf_sage_box_system_t *system, void *object,
    cf_sage_box_coordinate_t *relative_coordinate);

unsigned long cf_sage_box_system_get_volume(cf_sage_box_system_t *system);

void *cf_sage_box_system_iterate_next(cf_sage_box_system_t *system);

void cf_sage_box_system_iterate_start(cf_sage_box_system_t *system);

void cf_sage_box_system_move_absolute(cf_sage_box_system_t *system, void *object,
    cf_sage_box_coordinate_t *destination_coordinate);

void cf_sage_box_system_move_relative(cf_sage_box_system_t *system, void *object,
    cf_sage_box_coordinate_t *relative_coordinate);

void cf_sage_box_system_remove(cf_sage_box_system_t *system,
    cf_sage_box_coordinate_t *coordinate);

void cf_sage_box_system_replace(cf_sage_box_system_t *system, void *destination_object,
    void *source_object);

void cf_sage_box_system_replace_random(cf_sage_box_system_t *system, void *object);

void cf_sage_box_system_swap(cf_sage_box_system_t *system, void *object_a,
    void *object_b);

#endif
