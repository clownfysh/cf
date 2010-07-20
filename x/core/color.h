#ifndef x_core_color_h
#define x_core_color_h

struct x_core_color_t {
  unsigned long red;
  unsigned long blue;
  unsigned long green;
};
typedef struct x_core_color_t x_core_color_t;

void x_core_color_init(x_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green);

#endif
