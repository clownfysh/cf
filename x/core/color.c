#include "x/core/standard.h"
#include "x/core/color.h"

void x_core_color_init(x_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green)
{
  assert(color);

  color->red = red;
  color->green = green;
  color->blue = blue;
}
