#ifndef cf_x_core_random_h
#define cf_x_core_random_h

double cf_x_core_random_01();

double cf_x_core_random_gaussian(double mean, double stddev);

unsigned long cf_x_core_random_poisson(double mean);

unsigned long cf_x_core_random_unsigned_long(unsigned long range);

#endif
