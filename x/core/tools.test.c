#include "x/core/tools.h"

#define REPETITIONS 32768

int main(int argc, char *argv[])
{
  unsigned long eacx_toss;
  unsigned short toss_result;

  if (x_core_bool_false) {
    printf("tossing a coin %d times\n", REPETITIONS);
    for (eacx_toss = 0; eacx_toss < REPETITIONS; eacx_toss++) {
      toss_result = x_core_coin_toss();
    }
  }

  return 0;
}
