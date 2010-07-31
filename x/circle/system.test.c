#include "cf/x/circle/system.h"

int main(int argc, char *argv[])
{
  cf_x_circle_system_t *circle;
  unsigned long i;
  unsigned long x;

  circle = cf_x_circle_system_create();
  if (circle) {
    for (i = 0; i < 4000000; i++) {
      /*  printf("%lu\n", cf_x_circle_system_get_long(circle));  */
      x = cf_x_circle_system_get_long(circle);
    }
    cf_x_circle_system_destroy(circle);
  } else {
    cf_x_trace("x_circle_system_create");
  }

  return 0;
}
