#include "x/core/standard.h"
#include "x/math/tools.h"

double x_math_tools_calculate_distance(double x1, double y1, double x2,
    double y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

x_core_bool_t x_math_tools_equal_double(double a, double b)
{
  x_core_bool_t equal;

  if (abs(a - b) < 0.0000001) {
    equal = x_core_bool_true;
  } else {
    equal = x_core_bool_false;
  }

  return equal;
}
