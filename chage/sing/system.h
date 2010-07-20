#ifndef trf_sing_h
#define trf_sing_h

typedef unsigned char position_t;

struct sing_t;
typedef struct sing_t sing_t;

sing_t *sing_create();

void sing_destroy(sing_t *sing);

void sing_get_closest(sing_t *sing, position_t position, position_t *a);

void sing_get_closest_3(sing_t *sing, position_t position, position_t *a,
    position_t *b, position_t *c);
/*
  this gets the closest 3 *positions* :: some of those might be empty for this
  particular atom...might want sing and atom to work together so the functions
  can return the closest 3 positions with atoms in them for the specified atom
  ...
*/

void sing_get_closest_above(sing_t *sing, position_t position, position_t *a);

void sing_get_closest_3_above(sing_t *sing, position_t position, position_t *a,
    position_t *b, position_t *c);

void sing_get_closest_below(sing_t *sing, position_t position, position_t *a);

void sing_get_closest_3_below(sing_t *sing, position_t position, position_t *a,
    position_t *b, position_t *c);

void sing_get_highest(sing_t *sing, position_t *a);

void sing_get_highest_3(sing_t *sing, position_t *a, position_t *b,
    position_t *c);

void sing_get_lowest(sing_t *sing, position_t *a);

void sing_get_lowest_3(sing_t *sing, position_t *a, position_t *b,
    position_t *c);

void sing_get_northernmost(sing_t *sing, position_t *a);

void sing_get_northernmost_3(sing_t *sing, position_t *a, position_t *b,
    position_t *c);

void sing_get_random(sing_t *sing, position_t *a);

void sing_get_southernmost(sing_t *sing, position_t *a);

void sing_get_southernmost_3(sing_t *sing, position_t *a, position_t *b,
    position_t *c);

#endif
