#include "inferno/box/system.h"
#include "x/case/array.h"

struct inferno_box_system_t {
  inferno_box_coordinate_t dimension_coordinate;
  unsigned long volume;
  x_case_array_t *cells;
  inferno_box_object_get_cell_f get_cell;
  inferno_box_object_set_cell_f set_cell;
  x_core_destroy_f destroy_object;
  x_audit_log_t *log;
};

struct inferno_box_cell_t;
typedef struct inferno_box_cell_t inferno_box_cell_t;

struct inferno_box_cell_t {
  inferno_box_system_t *system;
  inferno_box_coordinate_t coordinate;
  inferno_box_cell_t *neighbors[3][3][3];
  void *object;
};

#ifdef CF_BUILD_DEVELOPMENT
static x_core_bool_t coordinate_in_range(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);
#endif

static x_core_bool_t create_cells(inferno_box_system_t *system);

static inferno_box_cell_t *find_cell(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);

static unsigned long get_cells_array_index(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);

static int inferno_box_cell_compare(void *cell_a_void, void *cell_b_void);

static void *inferno_box_cell_copy(void *cell_void);

static inferno_box_cell_t *inferno_box_cell_create(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate);

static void inferno_box_cell_destroy(void *cell_void);

/*
#ifdef CF_BUILD_DEVELOPMENT
static x_core_bool_t relative_coordinate_in_range
(inferno_box_coordinate_t *relative_coordinate);
#endif
*/

static void set_neighbor_references(inferno_box_system_t *system);

static void set_neighbor_references_for_cell(inferno_box_system_t *system,
    inferno_box_cell_t *cell);

#ifdef CF_BUILD_DEVELOPMENT
x_core_bool_t coordinate_in_range(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  x_core_bool_t in_range;

  if ((coordinate->x <= system->dimension_coordinate.x)
      && (coordinate->y <= system->dimension_coordinate.y)
      && (coordinate->z <= system->dimension_coordinate.z)) {
    in_range = x_core_bool_true;
  } else {
    in_range = x_core_bool_false;
  }

  return in_range;
}
#endif

x_core_bool_t create_cells(inferno_box_system_t *system)
{
  assert(system);
  x_core_bool_t success;
  inferno_box_coordinate_t coordinate;
  inferno_box_cell_t *cell;
  unsigned long cells_array_index;

  success = x_core_bool_true;

  system->cells = x_case_array_create(system->volume,
      inferno_box_cell_compare, inferno_box_cell_copy, inferno_box_cell_destroy);
  if (system->cells) {
    for (coordinate.x = 0; coordinate.x < system->dimension_coordinate.x;
         coordinate.x++) {
      for (coordinate.y = 0; coordinate.y < system->dimension_coordinate.y;
           coordinate.y++) {
        for (coordinate.z = 0; coordinate.z < system->dimension_coordinate.z;
             coordinate.z++) {
          cell = inferno_box_cell_create(system, &coordinate);
          if (cell) {
            cells_array_index = get_cells_array_index(system, &coordinate);
            x_case_array_add(system->cells, cells_array_index, cell);
          } else {
            success = x_core_bool_false;
            x_audit_log_trace(system->log, "box", "inferno_box_cell_create");
          }
        }
      }
    }
  } else {
    success = x_core_bool_false;
    x_audit_log_trace(system->log, "box", "x_case_array_create");
  }

  return success;
}

inferno_box_cell_t *find_cell(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  unsigned long cells_array_index;

  cells_array_index = get_cells_array_index(system, coordinate);
  return x_case_array_find(system->cells, cells_array_index);
}

unsigned long get_cells_array_index(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  return (coordinate->x * system->dimension_coordinate.y
      * system->dimension_coordinate.z)
    + (coordinate->y * system->dimension_coordinate.z)
    + coordinate->z;
}

int inferno_box_cell_compare(void *cell_a_void, void *cell_b_void)
{
  x_core_trace_exit("TODO: implement");
  return 0;
}

void *inferno_box_cell_copy(void *cell_void)
{
  x_core_trace_exit("TODO: implement");
  return NULL;
}

inferno_box_cell_t *inferno_box_cell_create(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  inferno_box_cell_t *cell;
  unsigned char each_x;
  unsigned char each_y;
  unsigned char each_z;

  cell = malloc(sizeof *cell);
  if (cell) {
    cell->system = system;
    inferno_box_coordinate_init_witx_coordinate(&cell->coordinate, coordinate);
    cell->object = NULL;
    for (each_x = 0; each_x < 3; each_x++) {
      for (each_y = 0; each_y < 3; each_y++) {
        for (each_z = 0; each_z < 3; each_z++) {
          cell->neighbors[each_x][each_y][each_z] = NULL;
        }
      }
    }
  } else {
    x_core_trace("malloc");
  }

  return cell;
}

void inferno_box_cell_destroy(void *cell_void)
{
  assert(cell_void);
  inferno_box_cell_t *cell;
  inferno_box_system_t *system;

  cell = cell_void;
  system = cell->system;

  if (cell->object) {
    system->set_cell(cell->object, NULL);
    if (system->destroy_object) {
      system->destroy_object(cell->object);
    }
  }

  free(cell_void);
}

void inferno_box_system_add(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate, void *object)
{
  assert(system);
  assert(coordinate);
  assert(coordinate_in_range(system, coordinate));
  assert(object);
  inferno_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  cell->object = object;
  system->set_cell(object, cell);
}

void inferno_box_system_add_random(inferno_box_system_t *system, void *object)
{
  assert(system);
  assert(object);
  inferno_box_coordinate_t coordinate;

  inferno_box_coordinate_init_witx_random(&coordinate,
      &system->dimension_coordinate);
  inferno_box_system_add(system, &coordinate, object);
}

inferno_box_system_t *inferno_box_system_create
(inferno_box_coordinate_t *dimension_coordinate,
    inferno_box_object_get_cell_f get_cell, inferno_box_object_set_cell_f set_cell,
    x_core_compare_f compare_objects, x_core_copy_f copy_object,
    x_core_destroy_f destroy_object, x_audit_log_t *log)
{
  assert(dimension_coordinate);
  assert(dimension_coordinate->x >= 1);
  assert(dimension_coordinate->y >= 1);
  assert(dimension_coordinate->z >= 1);
  assert(get_cell);
  assert(set_cell);
  assert(compare_objects);
  assert(copy_object);
  assert(log);
  inferno_box_system_t *system;
  x_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    inferno_box_coordinate_copy(&system->dimension_coordinate,
        dimension_coordinate);
    system->get_cell = get_cell;
    system->set_cell = set_cell;
    system->destroy_object = destroy_object;
    system->log = log;
    system->volume = dimension_coordinate->x * dimension_coordinate->y
      * dimension_coordinate->z;
    if (create_cells(system)) {
      so_far_so_good = x_core_bool_true;
      set_neighbor_references(system);
    } else {
      so_far_so_good = x_core_bool_false;
      x_audit_log_trace(log, "box", "create_cells");
    }
  } else {
    so_far_so_good = x_core_bool_false;
    x_core_trace("malloc");
  }

  if (!so_far_so_good && system) {
    if (system->cells) {
      x_case_array_destroy(system->cells);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void inferno_box_system_destroy(inferno_box_system_t *system)
{
  assert(system);
  x_case_array_destroy(system->cells);
  free(system);
}

void *inferno_box_system_find(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  assert(coordinate_in_range(system, coordinate));
  inferno_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  return cell->object;
}

void *inferno_box_system_find_random(inferno_box_system_t *system)
{
  assert(system);
  inferno_box_cell_t *cell;

  cell = x_case_array_find_random(system->cells);
  return cell->object;
}

void *inferno_box_system_find_relative(inferno_box_system_t *system,
    void *object, inferno_box_coordinate_t *relative_coordinate)
{
  assert(system);
  assert(object);
  assert(relative_coordinate);
  /*
  assert(relative_coordinate_in_range(relative_coordinate));
  */
  inferno_box_cell_t *cell;
  inferno_box_cell_t *neighbor;
  void *neighbor_object;

  cell = system->get_cell(object);
  neighbor = cell->neighbors[relative_coordinate->x][relative_coordinate->y]
    [relative_coordinate->z];
  if (neighbor) {
    neighbor_object = neighbor->object;
  } else {
    neighbor_object = NULL;
  }

  return neighbor_object;
}

unsigned long inferno_box_system_get_volume(inferno_box_system_t *system)
{
  return system->volume;
}

void *inferno_box_system_iterate_next(inferno_box_system_t *system)
{
  assert(system);
  inferno_box_cell_t *cell;
  void *object;

  cell = x_case_array_iterate_next(system->cells);
  if (cell) {
    object = cell->object;
  } else {
    object = NULL;
  }

  return object;
}

void inferno_box_system_iterate_start(inferno_box_system_t *system)
{
  x_case_array_iterate_start(system->cells);
}

void inferno_box_system_move_absolute(inferno_box_system_t *system, void *object,
    inferno_box_coordinate_t *destination_coordinate)
{
  assert(system);
  assert(object);
  assert(destination_coordinate);
  inferno_box_cell_t *source_cell;
  inferno_box_cell_t *destination_cell;

  source_cell = system->get_cell(object);
  destination_cell = find_cell(system, destination_coordinate);

  if (source_cell != destination_cell) {
    inferno_box_system_remove(system, destination_coordinate);
    inferno_box_system_add(system, destination_coordinate, object);
    source_cell->object = NULL;
  }
}

void inferno_box_system_move_relative(inferno_box_system_t *system, void *object,
    inferno_box_coordinate_t *relative_coordinate)
{
  assert(system);
  assert(object);
  assert(relative_coordinate);
  inferno_box_cell_t *source_cell;
  inferno_box_cell_t *destination_cell;

  source_cell = system->get_cell(object);
  destination_cell = source_cell->neighbors[relative_coordinate->x]
    [relative_coordinate->y][relative_coordinate->z];

  if (source_cell != destination_cell) {
    inferno_box_system_remove(system, &destination_cell->coordinate);
    inferno_box_system_add(system, &destination_cell->coordinate, object);
    source_cell->object = NULL;
  }
}

void inferno_box_system_remove(inferno_box_system_t *system,
    inferno_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  inferno_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  if (cell->object) {
    system->set_cell(cell->object, NULL);
    if (system->destroy_object) {
      system->destroy_object(cell->object);
    }
    cell->object = NULL;
  }
}

void inferno_box_system_replace(inferno_box_system_t *system, void *destination_object,
    void *source_object)
{
  assert(system);
  assert(destination_object);
  assert(source_object);
  inferno_box_cell_t *destination_cell;

  destination_cell = system->get_cell(destination_object);

  inferno_box_system_remove(system, &destination_cell->coordinate);
  inferno_box_system_add(system, &destination_cell->coordinate, source_object);
}

void inferno_box_system_replace_random(inferno_box_system_t *system, void *object)
{
  assert(system);
  assert(object);
  inferno_box_coordinate_t coordinate;

  inferno_box_coordinate_init_witx_random(&coordinate,
      &system->dimension_coordinate);

  inferno_box_system_remove(system, &coordinate);
  inferno_box_system_add(system, &coordinate, object);
}

void inferno_box_system_swap(inferno_box_system_t *system, void *object_a,
    void *object_b)
{
  assert(system);
  assert(object_a);
  assert(object_b);
  inferno_box_cell_t *cell_a;
  inferno_box_cell_t *cell_b;

  cell_a = system->get_cell(object_a);
  cell_b = system->get_cell(object_b);

  cell_a->object = object_b;
  cell_b->object = object_a;

  system->set_cell(object_a, cell_b);
  system->set_cell(object_b, cell_a);
}

/*
#ifdef X_BUILD_DEVELOPMENT
x_core_bool_t relative_coordinate_in_range
(inferno_box_coordinate_t *relative_coordinate)
{
  assert(relative_coordinate);
  x_core_bool_t in_range;

  if ((relative_coordinate->x < 3) && (relative_coordinate->y < 3)
      && (relative_coordinate->z < 3)) {
    in_range = x_core_bool_true;
  } else {
    in_range = x_core_bool_false;
  }

  return in_range;
}
#endif
*/

void set_neighbor_references(inferno_box_system_t *system)
{
  assert(system);
  inferno_box_cell_t *cell;
  unsigned long each_cell;

  for (each_cell = 0; each_cell < system->volume; each_cell++) {
    cell = x_case_array_find(system->cells, each_cell);
    set_neighbor_references_for_cell(system, cell);
  }
}

void set_neighbor_references_for_cell(inferno_box_system_t *system,
    inferno_box_cell_t *cell)
{
  assert(system);
  assert(cell);
  inferno_box_coordinate_t neighbor_coordinate;
  inferno_box_coordinate_t relative_coordinate;
  inferno_box_cell_t *neighbor;

  for (relative_coordinate.x = 0;
       relative_coordinate.x < 3;
       relative_coordinate.x++) {
    for (relative_coordinate.y = 0;
         relative_coordinate.y < 3;
         relative_coordinate.y++) {
      for (relative_coordinate.z = 0;
           relative_coordinate.z < 3;
           relative_coordinate.z++) {
        neighbor_coordinate.x = x_core_wrap_index
          (cell->coordinate.x + (relative_coordinate.x - 1),
              system->dimension_coordinate.x);
        neighbor_coordinate.y = x_core_wrap_index
          (cell->coordinate.y + (relative_coordinate.y - 1),
              system->dimension_coordinate.y);
        neighbor_coordinate.z = x_core_wrap_index
          (cell->coordinate.z + (relative_coordinate.z - 1),
              system->dimension_coordinate.z);
        neighbor = find_cell(system, &neighbor_coordinate);
        cell->neighbors[relative_coordinate.x][relative_coordinate.y]
          [relative_coordinate.z] = neighbor;
      }
    }
  }
}
