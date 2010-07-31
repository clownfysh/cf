#include "cf/x/core/standard.h"
#include "cf/x/core/color.h"

void cf_x_core_color_init(cf_x_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green)
{
  assert(color);

  color->red = red;
  color->green = green;
  color->blue = blue;
}
