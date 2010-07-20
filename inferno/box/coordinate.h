#ifndef inferno_box_coordinate_h
#define inferno_box_coordinate_h

struct inferno_box_coordinate_t {
  unsigned short x;
  unsigned short y;
  unsigned short z;
};
typedef struct inferno_box_coordinate_t inferno_box_coordinate_t;

void inferno_box_coordinate_copy(inferno_box_coordinate_t *destination,
    inferno_box_coordinate_t *source);

void inferno_box_coordinate_free(inferno_box_coordinate_t *coordinate);

void inferno_box_coordinate_init_witx_coordinate(inferno_box_coordinate_t *coordinate,
    inferno_box_coordinate_t *initial_coordinate);

void inferno_box_coordinate_init_witx_random(inferno_box_coordinate_t *coordinate,
    inferno_box_coordinate_t *dimension_coordinate);

void inferno_box_coordinate_init_witx_xyz(inferno_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z);

#endif
