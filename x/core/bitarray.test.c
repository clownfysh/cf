#include "x/core/bitarray.h"
#include "x/core/tools.h"

static void test_from_char();
static void test_from_double();
static void test_from_long();
static void test_from_short();
static void test_from_string();
static void test_get_functions();
static void test_set_functions();

int main(int argc, char *argv[])
{
  x_disable_test();

  x_core_bitarray_t *bitarray;

  bitarray = x_core_bitarray_create(4);
  if (!bitarray) {
    x_core_trace_exit("x_core_bitarray_create");
  }

  x_core_bitarray_unset_all(bitarray);
  x_core_bitarray_print(bitarray);
  while (x_core_bitarray_increment(bitarray)) {
    x_core_bitarray_print(bitarray);
  }

  x_core_bitarray_destroy(bitarray);

  test_get_functions();
  test_set_functions();
  test_from_char();
  test_from_double();
  test_from_long();
  test_from_short();
  test_from_string();

  return 0;
}

static void test_from_char()
{
  x_core_bitarray_t *bitarray;
  char *string;

  bitarray = x_core_bitarray_create_from_char('a');
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);

  bitarray = x_core_bitarray_create_from_char('b');
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("b = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);

  bitarray = x_core_bitarray_create_from_char('c');
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("c = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_from_double()
{
  x_core_bitarray_t *bitarray;
  char *string;

  bitarray = x_core_bitarray_create_from_double(1.5);
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1.5 = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_from_long()
{
  x_core_bitarray_t *bitarray;
  char *string;

  bitarray = x_core_bitarray_create_from_long(1025);
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1025 = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_from_short()
{
  x_core_bitarray_t *bitarray;
  char *string;

  bitarray = x_core_bitarray_create_from_short(5);
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("5 = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_from_string()
{
  x_core_bitarray_t *bitarray = NULL;
  char *string;

  bitarray = x_core_bitarray_create_from_string("A", strlen("A"));
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("A = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);

  bitarray = x_core_bitarray_create_from_string("abc", strlen("abc"));
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("abc = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);

  bitarray = x_core_bitarray_create_from_string("a@#!2323..~",
      strlen("a@#!2323..~"));
  string = x_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a@#!2323..~ = %s\n", string);
    free(string);
  } else {
    x_core_trace_exit("x_core_bitarray_as_binary_string");
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_get_functions()
{
  double float_value = 0.0;
  unsigned long value = 0;
  x_core_bitarray_t *bitarray = NULL;

  bitarray = x_core_bitarray_create(8);
  if (!bitarray) {
    x_core_trace_exit("x_core_bitarray_create");
  }
  x_core_bitarray_set_all(bitarray);

  value = x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 7);
  if (value != 255) {
    x_core_trace_exit("x_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 255 got %d)", (int) value);
  }

  value = x_core_bitarray_get_unsigned_long_from_bits(bitarray, 2, 4);
  if (value != 7)  {
    x_core_trace_exit("x_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 7 got %d)", (int) value);
  }

  x_core_bitarray_set_bit(bitarray, 0, 0);
  x_core_bitarray_set_bit(bitarray, 2, 0);

  value = x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 3);
  if (value != 10)  {
    x_core_trace_exit("x_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 10 got %d)", (int) value);
  }
  x_core_bitarray_destroy(bitarray);

  bitarray = x_core_bitarray_create(4);
  if (!bitarray) {
    x_core_trace_exit("x_core_bitarray_create");
  }
  x_core_bitarray_set_all(bitarray);

  float_value = x_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (float_value != 1.0) {
    x_core_trace_exit("x_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  float_value = x_core_bitarray_get_double_from_bits(bitarray, 1, 3);
  if (float_value != 1.0) {
    x_core_trace_exit("x_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  x_core_bitarray_set_bit(bitarray, 0, 0);
  x_core_bitarray_set_bit(bitarray, 3, 0);

  float_value = x_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (fabs(float_value - (6.0 / 15.0)) > 0.00001) {
    x_core_trace_exit("x_core_bitarray_get_doublefrom_bits() "
        "failed (expected %f got %f)", (6.0 / 15.0), float_value);
  }
  x_core_bitarray_destroy(bitarray);
}

static void test_set_functions()
{
  double value;
  x_core_bitarray_t *bitarray;

  bitarray = x_core_bitarray_create(16);
  x_core_bitarray_set_unsigned_long(bitarray, 0, 15, 31);
  if (x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 31) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  x_core_bitarray_set_unsigned_long(bitarray, 0, 15, 0);
  if (x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 0) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  x_core_bitarray_set_unsigned_long(bitarray, 0, 15, 1);
  if (x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 1) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_unsigned_long() "
        "failed", __FUNCTION__, __LINE__);
  }

  x_core_bitarray_set_unsigned_long(bitarray, 0, 15,
      (unsigned long) (pow(2, 16) - 1));
  if (x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) !=
      (unsigned long) (pow(2, 16) - 1)) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_unsigned_long() "
        "failed (%lu)", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15));
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 0.0);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.0) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 0.1);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.1) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 0.33);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.33) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15),
        value);
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 0.5);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.5) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 0.6478);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.6478) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  x_core_bitarray_set_double(bitarray, 0, 15, 1.0);
  value = x_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 1.0) > 0.001) {
    x_core_trace_exit("[%s():%d] x_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        x_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  x_core_bitarray_destroy(bitarray);
}
