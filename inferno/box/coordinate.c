#include "inferno/box/coordinate.h"
#include "x/core/standard.h"

void inferno_box_coordinate_copy(inferno_box_coordinate_t *destination,
    inferno_box_coordinate_t *source)
{
  assert(destination);
  assert(source);

  destination->x = source->x;
  destination->y = source->y;
  destination->z = source->z;
}

void inferno_box_coordinate_free(inferno_box_coordinate_t *coordinate) {}

void inferno_box_coordinate_init_witx_coordinate(inferno_box_coordinate_t *coordinate,
    inferno_box_coordinate_t *initial_coordinate)
{
  assert(coordinate);

  coordinate->x = initial_coordinate->x;
  coordinate->y = initial_coordinate->y;
  coordinate->z = initial_coordinate->z;
}

void inferno_box_coordinate_init_witx_random(inferno_box_coordinate_t *coordinate,
    inferno_box_coordinate_t *dimension_coordinate)
{
  assert(coordinate);
  assert(dimension_coordinate);

  coordinate->x = random() % dimension_coordinate->x;
  coordinate->y = random() % dimension_coordinate->y;
  coordinate->z = random() % dimension_coordinate->z;
}

void inferno_box_coordinate_init_witx_xyz(inferno_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z)
{
  assert(coordinate);

  coordinate->x = x;
  coordinate->y = y;
  coordinate->z = z;
}
