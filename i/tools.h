#ifndef i_tools_h
#define i_tools_h

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define i_trace(message, ...) fprintf(stderr, message "() in %s:%d\n",  \
      ##__VA_ARGS__, __FILE__, __LINE__)

#define i_disable_test() printf("  %s disabled\n", argv[0]); return 0

enum i_bit_t {
  i_bit_zero = 0,
  i_bit_one = 1,
};
typedef enum i_bit_t i_bit_t;

enum i_bool_t {
  i_bool_false = 0,
  i_bool_true = 1,
};
typedef enum i_bool_t i_bool_t;

unsigned long i_get_microseconds();

unsigned long i_wrap_index(long virtual_index, unsigned long range);

#endif
