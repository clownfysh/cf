#include "i/circle/system.h"

int main(int argc, char *argv[])
{
  i_circle_system_t *circle;
  unsigned long i;
  unsigned long x;

  circle = i_circle_system_create();
  if (circle) {
    for (i = 0; i < 4000000; i++) {
      /*  printf("%lu\n", i_circle_system_get_long(circle));  */
      x = i_circle_system_get_long(circle);
    }
    i_circle_system_destroy(circle);
  } else {
    i_trace("i_circle_system_create");
  }

  return 0;
}
