#ifndef cf_inferno_box_coordinate_h
#define cf_inferno_box_coordinate_h

struct cf_inferno_box_coordinate_t {
  unsigned short x;
  unsigned short y;
  unsigned short z;
};
typedef struct cf_inferno_box_coordinate_t cf_inferno_box_coordinate_t;

void cf_inferno_box_coordinate_copy(cf_inferno_box_coordinate_t *destination,
    cf_inferno_box_coordinate_t *source);

void cf_inferno_box_coordinate_free(cf_inferno_box_coordinate_t *coordinate);

void cf_inferno_box_coordinate_init_with_coordinate(cf_inferno_box_coordinate_t *coordinate,
    cf_inferno_box_coordinate_t *initial_coordinate);

void cf_inferno_box_coordinate_init_with_random(cf_inferno_box_coordinate_t *coordinate,
    cf_inferno_box_coordinate_t *dimension_coordinate);

void cf_inferno_box_coordinate_init_with_xyz(cf_inferno_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z);

#endif
