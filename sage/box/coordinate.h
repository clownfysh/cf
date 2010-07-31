#ifndef cf_sage_box_coordinate_h
#define cf_sage_box_coordinate_h

struct cf_sage_box_coordinate_t {
  unsigned short x;
  unsigned short y;
  unsigned short z;
};
typedef struct cf_sage_box_coordinate_t cf_sage_box_coordinate_t;

void cf_sage_box_coordinate_copy(cf_sage_box_coordinate_t *destination,
    cf_sage_box_coordinate_t *source);

void cf_sage_box_coordinate_free(cf_sage_box_coordinate_t *coordinate);

void cf_sage_box_coordinate_init_witx_coordinate(cf_sage_box_coordinate_t *coordinate,
    cf_sage_box_coordinate_t *initial_coordinate);

void cf_sage_box_coordinate_init_witx_random(cf_sage_box_coordinate_t *coordinate,
    cf_sage_box_coordinate_t *dimension_coordinate);

void cf_sage_box_coordinate_init_witx_xyz(cf_sage_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z);

#endif
