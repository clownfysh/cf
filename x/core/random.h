#ifndef x_core_random_h
#define x_core_random_h

double x_core_random_01();

double x_core_random_gaussian(double mean, double stddev);

unsigned long x_core_random_poisson(double mean);

unsigned long x_core_random_unsigned_long(unsigned long range);

#endif
