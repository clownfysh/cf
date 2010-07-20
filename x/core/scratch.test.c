#include "x/core/tools.h"

int main(int argc, char *argv[])
{
  unsigned long n;
  unsigned long m;

  for (n = 0; n < 32; n++) {
    for (m = 0; m < 64; m++) {
      printf("%i", x_core_coin_toss());
    }
    printf("\n");
  }

  return 0;
}
