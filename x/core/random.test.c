#include "x/core/random.h"
#include "x/core/tools.h"

#define X_CORE_RANDOM_TEST_ITERATIONS 30000

#define X_CORE_RANDOM_TEST_UNIFORM_PRECISION 0.01

#define X_CORE_RANDOM_TEST_POISSON_PRECISION 0.05
#define X_CORE_RANDOM_TEST_POISSON_MEAN 3

#define X_CORE_RANDOM_TEST_GAUSSIAN_PRECISION 0.05
#define X_CORE_RANDOM_TEST_GAUSSIAN_MEAN 5

int main(int argc, char *argv[])
{
  unsigned long ii = 0;
  double value = 0.0;
  double total = 0.0;
  double average = 0.0;

  srand(time(0));

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < X_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = x_core_random_01();
    total += value;
  }
  average = (total / X_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - .5);

  if (value > X_CORE_RANDOM_TEST_UNIFORM_PRECISION) {
    x_core_trace_exit("FAILED: Uniform random average: %f", average);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < X_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = (double) x_core_random_poisson(X_CORE_RANDOM_TEST_POISSON_MEAN);
    total += value;
  }
  average = (total / X_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - X_CORE_RANDOM_TEST_POISSON_MEAN);

  if (value > X_CORE_RANDOM_TEST_POISSON_PRECISION) {
    x_core_trace_exit("FAILED: Poisson random average: %f vs %f", average,
        (double) X_CORE_RANDOM_TEST_POISSON_MEAN);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < X_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = x_core_random_gaussian(X_CORE_RANDOM_TEST_GAUSSIAN_MEAN, 2.0);
    total += value;
  }
  average = (total / X_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - X_CORE_RANDOM_TEST_GAUSSIAN_MEAN);

  if (value > X_CORE_RANDOM_TEST_GAUSSIAN_PRECISION) {
    x_core_trace_exit("FAILED: Gaussian random average: %f vs %f",
        average, (double) X_CORE_RANDOM_TEST_GAUSSIAN_MEAN);
  }

  return 0;
}
