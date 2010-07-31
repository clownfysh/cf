#ifndef cf_x_core_color_h
#define cf_x_core_color_h

struct cf_x_core_color_t {
  unsigned long red;
  unsigned long blue;
  unsigned long green;
};
typedef struct cf_x_core_color_t cf_x_core_color_t;

void cf_x_core_color_init(cf_x_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green);

#endif
