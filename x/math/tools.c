#include "cf/x/core/standard.h"
#include "cf/x/math/tools.h"

double cf_x_math_tools_calculate_distance(double x1, double y1, double x2,
    double y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

cf_x_core_bool_t cf_x_math_tools_equal_double(double a, double b)
{
  cf_x_core_bool_t equal;

  if (abs(a - b) < 0.0000001) {
    equal = cf_x_core_bool_true;
  } else {
    equal = cf_x_core_bool_false;
  }

  return equal;
}
