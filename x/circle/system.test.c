#include "x/circle/system.h"

int main(int argc, char *argv[])
{
  x_circle_system_t *circle;
  unsigned long i;
  unsigned long x;

  circle = x_circle_system_create();
  if (circle) {
    for (i = 0; i < 4000000; i++) {
      /*  printf("%lu\n", x_circle_system_get_long(circle));  */
      x = x_circle_system_get_long(circle);
    }
    x_circle_system_destroy(circle);
  } else {
    x_trace("x_circle_system_create");
  }

  return 0;
}
