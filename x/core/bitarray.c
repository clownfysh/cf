#include "x/core/bitarray.h"
#include "x/core/tools.h"

#if defined X_CORE_BITARRAY_COMPRESSED_IMPL
#include "x/core/bitarray.compressed.impl.c"
#elif defined X_CORE_BITARRAY_EXPANDED_IMPL
#include "x/core/bitarray.expanded.impl.c"
#else
#include "x/core/bitarray.expanded.impl.c"
#endif

#define UNSIGNED_CHAR_MAX_PLACE_VALUE 128
#define UNSIGNED_LONG_MAX_PLACE_VALUE 2147483648UL
#define UNSIGNED_SHORT_MAX_PLACE_VALUE 32768

#define SIGNED_CHAR_MAX_PLACE_VALUE 64
#define SIGNED_LONG_MAX_PLACE_VALUE 1073741824
#define SIGNED_SHORT_MAX_PLACE_VALUE 16384

static x_core_bitarray_t *create_from_signed_long(long value,
    unsigned short bits, unsigned long max_place_value);

static x_core_bitarray_t *create_from_unsigned_long
(unsigned long value, unsigned short bits, unsigned long max_place_value);

static long get_signed_long(x_core_bitarray_t *bitarray, unsigned long index,
    unsigned short bits, unsigned long max_place_value);

static unsigned long get_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits, unsigned long max_place_value);

static x_core_bool_t increment(x_core_bitarray_t *bitarray,
    unsigned long position);

x_core_bitarray_t *create_from_signed_long(long value, unsigned short bits,
    unsigned long max_place_value)
{
  assert(bits >= 2);
  x_core_bool_t negative;
  x_core_bitarray_t *bitarray;
  unsigned long place_value;
  unsigned long each_bit;
  unsigned long div;

  bitarray = x_core_bitarray_create(bits);
  if (bitarray) {

    if (value < 0) {
      negative = x_core_bool_true;
    } else {
      negative = x_core_bool_false;
    }
    x_core_bitarray_set_bit(bitarray, 0, negative);
    value = labs(value);

    place_value = max_place_value;
    for (each_bit = 1; each_bit < bits; each_bit++) {
      div = value / place_value;
      if (1 == div) {
        x_core_bitarray_set_bit(bitarray, each_bit, 1);
      }
      value = value % place_value;
      place_value /= 2;
    }

  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray;
}

x_core_bitarray_t *create_from_unsigned_long(unsigned long value,
    unsigned short bits, unsigned long max_place_value)
{
  assert(bits >= 2);
  x_core_bitarray_t *bitarray;
  unsigned long place_value;
  unsigned long each_bit;
  unsigned long div;

  bitarray = x_core_bitarray_create(bits);
  if (bitarray) {
    place_value = max_place_value;
    for (each_bit = 0; each_bit < bits; each_bit++) {
      div = value / place_value;
      if (1 == div) {
        x_core_bitarray_set_bit(bitarray, each_bit, 1);
      }
      value = value % place_value;
      place_value /= 2;
      if (0 == place_value) {
        break;
      }
    }
  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray;
}

long get_signed_long(x_core_bitarray_t *bitarray, unsigned long index,
    unsigned short bits, unsigned long max_place_value)
{
  assert(bitarray);
  assert(bits >= 2);
  assert(x_core_bitarray_get_size(bitarray) >= 2);
  assert((index + bits) <= x_core_bitarray_get_size(bitarray));
  long value;
  unsigned short each_bit;
  unsigned long place_value;
  x_core_bit_t negative;
  x_core_bit_t bit;

  negative = x_core_bitarray_get_bit(bitarray, index);

  value = 0;
  place_value = max_place_value;
  for (each_bit = 1; each_bit < bits; each_bit++) {
    bit = x_core_bitarray_get_bit(bitarray, index + each_bit);
    if (bit) {
      value += place_value;
    }
    place_value /= 2;
  }

  if (negative) {
    value *= -1;
  }

  return value;
}

unsigned long get_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits, unsigned long max_place_value)
{
  assert(bitarray);
  assert(bits >= 2);
  assert(x_core_bitarray_get_size(bitarray) >= 2);
  unsigned long value;
  unsigned short each_bit;
  unsigned long place_value;
  x_core_bit_t bit;

  value = 0;
  place_value = max_place_value;
  for (each_bit = 0; each_bit < bits; each_bit++) {
    bit = x_core_bitarray_get_bit(bitarray, index + each_bit);
    if (bit) {
      value += place_value;
    }
    place_value /= 2;
  }

  return value;
}

x_core_bool_t x_core_bitarray_add_to_message(void *bitarray_object,
    x_core_message_t *message)
{
  assert(bitarray_object);
  assert(message);
  x_core_bool_t success;
  char *bitarray_as_string;
  x_core_bitarray_t *bitarray;

  bitarray = bitarray_object;

  bitarray_as_string = x_core_bitarray_get_as_string(bitarray);
  if (bitarray_as_string) {
    if (x_core_message_add_string(message, bitarray_as_string)) {
      success = x_core_bool_true;
    } else {
      success = x_core_bool_false;
      x_core_trace("x_core_message_add_string");
    }
    free(bitarray_as_string);
  } else {
    success = x_core_bool_false;
    x_core_trace("x_core_bitarray_get_as_string");
  }

  return success;
}

char *x_core_bitarray_as_binary_string(void *bitarray_object)
{
  assert(bitarray_object);
  x_core_bitarray_t *bitarray;
  char *string;
  unsigned long index;
  x_core_bit_t bit;
  unsigned long array_size;

  bitarray = bitarray_object;
  array_size = x_core_bitarray_get_size(bitarray);

  string = malloc(array_size + 1);
  if (string) {
    for (index = 0; index < array_size; index++) {
      bit = x_core_bitarray_get_bit(bitarray, index);
      if (0 == bit) {
        *(string + index) = '0';
      } else {
        *(string + index) = '1';
      }
      *(string + array_size) = '\0';
    }
  } else {
    x_core_trace("malloc");
  }

  return string;
}

x_core_bitarray_t *x_core_bitarray_create_from_char(char value)
{
  return x_core_bitarray_create_from_char_bits(value,
      X_CORE_BITARRAY_BITS_IN_CHAR);
}

x_core_bitarray_t *x_core_bitarray_create_from_char_bits(char value,
    unsigned short bits)
{
  return create_from_signed_long(value, bits, SIGNED_CHAR_MAX_PLACE_VALUE);
}

x_core_bitarray_t *x_core_bitarray_create_from_double(double value)
{
  assert(value < 65536.0);
  return x_core_bitarray_create_from_double_bits(value, 16, 16);
}

x_core_bitarray_t *x_core_bitarray_create_from_double_bits(double value,
    unsigned short integral_bits, unsigned short fractional_bits)
{
  x_core_bitarray_t *bitarray;
  x_core_bool_t negative;
  double integral_part;
  double fractional_part;
  unsigned long integral_long;
  unsigned long fractional_long;
  x_core_bitarray_t *integral_bitarray;
  x_core_bitarray_t *fractional_bitarray;

  bitarray = x_core_bitarray_create(1 + integral_bits + fractional_bits);
  if (bitarray) {

    if (value < 0) {
      negative = x_core_bool_true;
    } else {
      negative = x_core_bool_false;
    }
    x_core_bitarray_set_bit(bitarray, 0, negative);
    value = fabs(value);

    fractional_part = modf(value, &integral_part);
    fractional_part *= 100000;
    integral_long = (unsigned long) integral_part;
    fractional_long = (unsigned long) fractional_part;

    integral_bitarray = create_from_unsigned_long
      (integral_long, 16, 32768);
    if (integral_bitarray) {
      x_core_bitarray_set_bits_from_bitarray
        (bitarray, 1, integral_bitarray, 0, 16);
      x_core_bitarray_destroy(integral_bitarray);
    } else {
      x_core_trace("create_from_unsigned_long");
    }

    fractional_bitarray = create_from_unsigned_long
      (fractional_long, 16, 32768);
    if (fractional_bitarray) {
      x_core_bitarray_set_bits_from_bitarray
        (bitarray, 17, fractional_bitarray, 0, 16);
      x_core_bitarray_destroy(fractional_bitarray);
    } else {
      x_core_trace("create_from_unsigned_long");
    }

  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray;
}

x_core_bitarray_t *x_core_bitarray_create_from_long(long value)
{
  return x_core_bitarray_create_from_long_bits
    (value, X_CORE_BITARRAY_BITS_IN_LONG);
}

x_core_bitarray_t *x_core_bitarray_create_from_long_bits(long value,
    unsigned short bits)
{
  return create_from_signed_long(value, bits, SIGNED_LONG_MAX_PLACE_VALUE);
}

x_core_bitarray_t *x_core_bitarray_create_from_message
(x_core_message_t *message)
{
  assert(message);
  x_core_bitarray_t *bitarray;
  char *string;
  unsigned long string_length;
  unsigned long each_bit;
  char bit_char;
  x_core_bit_t bit;

  string = x_core_message_take_string(message);
  if (string) {
    string_length = strlen(string);
    bitarray = x_core_bitarray_create(string_length);
    if (bitarray) {
      for (each_bit = 0; each_bit < string_length; each_bit++) {
        bit_char = *(string + each_bit);
        if ('0' == bit_char) {
          bit = 0;
        } else {
          bit = 1;
        }
        x_core_bitarray_set_bit(bitarray, each_bit, bit);
      }
    } else {
      x_core_trace("x_core_bitarray_create");
    }
    free(string);
  } else {
    x_core_trace("x_core_message_take_string");
    bitarray = NULL;
  }

  return bitarray;
}

x_core_bitarray_t *x_core_bitarray_create_from_short(short value)
{
  return x_core_bitarray_create_from_short_bits
    (value, X_CORE_BITARRAY_BITS_IN_SHORT);
}

x_core_bitarray_t *x_core_bitarray_create_from_short_bits(short value,
    unsigned short bits)
{
  return create_from_signed_long(value, bits, SIGNED_SHORT_MAX_PLACE_VALUE);
}

x_core_bitarray_t *x_core_bitarray_create_from_string(char *string,
    unsigned long string_length)
{
  return x_core_bitarray_create_from_string_bits(string, string_length,
      string_length * X_CORE_BITARRAY_BITS_IN_CHAR);
}

x_core_bitarray_t *x_core_bitarray_create_from_string_bits(char *string,
    unsigned long string_length, unsigned short bits)
{
  assert(string);
  unsigned short each_char;
  unsigned char c;
  x_core_bitarray_t *bitarray;
  x_core_bitarray_t *char_bitarray;
  unsigned short bit_position;
  unsigned short char_count;

  bitarray = x_core_bitarray_create(bits);
  if (bitarray) {
    char_count = bits / X_CORE_BITARRAY_BITS_IN_CHAR;
    for (each_char = 0; each_char < char_count; each_char++) {
      c = *(string + each_char);
      char_bitarray = x_core_bitarray_create_from_unsigned_char(c);
      if (char_bitarray) {
        bit_position = each_char * X_CORE_BITARRAY_BITS_IN_CHAR;
        x_core_bitarray_set_bits_from_bitarray(bitarray, bit_position,
            char_bitarray, 0, X_CORE_BITARRAY_BITS_IN_CHAR);
        x_core_bitarray_destroy(char_bitarray);
      } else {
        x_core_trace("x_core_bitarray_create_from_unsigned_char");
      }
    }
  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray;
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_char
(unsigned char value)
{
  return x_core_bitarray_create_from_unsigned_char_bits
    (value, X_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR);
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_char_bits
(unsigned char value, unsigned short bits)
{
  return create_from_unsigned_long(value, bits, UNSIGNED_CHAR_MAX_PLACE_VALUE);
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_long
(unsigned long value)
{
  assert(value <= 2147483647);
  return x_core_bitarray_create_from_unsigned_long_bits
    (value, X_CORE_BITARRAY_BITS_IN_UNSIGNED_LONG);
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_long_bits
(unsigned long value, unsigned short bits)
{
  return create_from_unsigned_long(value, bits, UNSIGNED_LONG_MAX_PLACE_VALUE);
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_short
(unsigned short value)
{
  return x_core_bitarray_create_from_unsigned_short_bits
    (value, X_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT);
}

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_short_bits
(unsigned short value, unsigned short bits)
{
  return x_core_bitarray_create_from_short_bits((short) value, bits);
}

x_core_bitarray_t *x_core_bitarray_create_random(unsigned long size)
{
  x_core_bitarray_t *bitarray;

  bitarray = x_core_bitarray_create(size);
  if (bitarray) {
    x_core_bitarray_randomize(bitarray);
  } else {
    x_core_trace("x_core_bitarray_create");
  }

  return bitarray;
}

char *x_core_bitarray_get_as_string(void *bitarray_object)
{
  assert(bitarray_object);
  x_core_bitarray_t *bitarray;
  char *string;
  unsigned long bitarray_size;
  unsigned long each_bit;
  unsigned long char_index;
  unsigned long bit_index;
  x_core_bit_t bit;
  char *c;
  unsigned long string_size;

  bitarray = bitarray_object;

  bitarray_size = x_core_bitarray_get_size(bitarray);

  /*
    for every bit in the bitarray, plus 1 terminator bit, we'll need 2 actual
    bits for the encoded string, plus 1 extra byte at the end in case the
    "/ 8" had a remainder, plus 1 more byte for the terminator
  */
  string_size = (((bitarray_size + 1) * 2) / 8) + 1 + 1;

  string = malloc(string_size);
  if (string) {
    memset(string, 0, string_size);
    for (each_bit = 0; each_bit < bitarray_size; each_bit++) {

      char_index = (each_bit * 2) / 8;
      bit_index = (each_bit * 2) % 8;

      c = string + char_index;

      bit = x_core_bitarray_get_bit(bitarray, each_bit);
      if (bit) {
        x_core_set_bit_in_unsigned_char((unsigned char *) c, bit_index, 1);
        x_core_set_bit_in_unsigned_char((unsigned char *) c, bit_index + 1, 1);
      } else {
        x_core_set_bit_in_unsigned_char((unsigned char *) c, bit_index, 0);
        x_core_set_bit_in_unsigned_char((unsigned char *) c, bit_index + 1, 1);
      }

    }
  } else {
    x_core_trace("malloc");
  }

  return string;
}

x_core_bit_t x_core_bitarray_get_bit(x_core_bitarray_t *bitarray,
    unsigned long virtual_index)
{
  assert(bitarray);
  unsigned long index;
  x_core_bit_t bit;

  index = x_core_wrap_index(virtual_index, bitarray->array_size);
  bit = x_core_bitarray_get_bit_actual(bitarray, index);

  return bit;
}

unsigned long x_core_bitarray_get_actual_index(x_core_bitarray_t *bitarray,
    unsigned long virtual_index)
{
  return x_core_wrap_index(virtual_index, bitarray->array_size);
}

x_core_bool_t x_core_bitarray_get_bool(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_bit(bitarray, index);
}

char x_core_bitarray_get_char(x_core_bitarray_t *bitarray, unsigned long index)
{
  return x_core_bitarray_get_char_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_CHAR);
}

char x_core_bitarray_get_char_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits)
{
  return get_signed_long(bitarray, index, bits, SIGNED_CHAR_MAX_PLACE_VALUE);
}

double x_core_bitarray_get_double(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_double_bits(bitarray, index, 16, 16);
}

double x_core_bitarray_get_double_bits(x_core_bitarray_t *bitarray,
    unsigned long index,
    unsigned short integral_bits,
    unsigned short fractional_bits)
{
  assert(bitarray);
  assert(integral_bits >= 1 && integral_bits <= 16);
  assert(fractional_bits >= 1 && fractional_bits <= 16);
  assert(x_core_bitarray_get_size(bitarray) >= 2);
  assert((index + 1 + integral_bits + fractional_bits)
      <= x_core_bitarray_get_size(bitarray));
  double value;
  x_core_bit_t negative;
  unsigned long integral_long;
  unsigned long fractional_long;
  double fractional_value;

  negative = x_core_bitarray_get_bit(bitarray, index);

  integral_long = get_unsigned_long(bitarray, index + 1, integral_bits,
      pow(2, integral_bits - 1));

  fractional_long = get_unsigned_long(bitarray, index + 1 + integral_bits,
      fractional_bits, pow(2, fractional_bits - 1));
  fractional_value = fractional_long / 100000.0;

  value = integral_long + fractional_value;

  if (negative) {
    value *= -1;
  }

  return value;
}

double x_core_bitarray_get_double_from_bits(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index)
{
  assert(bitarray);
  assert(start_index >= 0);
  assert(end_index < x_core_bitarray_get_size(bitarray));
  assert(start_index <= end_index);
  double dividend;
  double divisor;
  double result;

  dividend = (double) x_core_bitarray_get_unsigned_long_from_bits(bitarray,
      start_index, end_index);
  divisor = pow(2.0, ((double) end_index - (double) start_index + 1.0)) - 1.0;
  if (0 == divisor) {
    result = 0.0;
  } else {
    result = dividend / (double) divisor;
  }

  return result;
}

long x_core_bitarray_get_long(x_core_bitarray_t *bitarray, unsigned long index)
{
  return x_core_bitarray_get_long_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_LONG);
}

long x_core_bitarray_get_long_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits)
{
  return get_signed_long(bitarray, index, bits, SIGNED_LONG_MAX_PLACE_VALUE);
}

short x_core_bitarray_get_short(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_short_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_SHORT);
}

short x_core_bitarray_get_short_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits)
{
  return get_signed_long(bitarray, index, bits, SIGNED_SHORT_MAX_PLACE_VALUE);
}

char *x_core_bitarray_get_string(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits)
{
  assert(bitarray);
  char *string;
  unsigned long string_length;
  unsigned long each_char;
  unsigned long start_bit;
  unsigned char c;

  string_length = bits / X_CORE_BITARRAY_BITS_IN_CHAR;

  string = malloc(string_length + 1);
  if (string) {
    for (each_char = 0; each_char < string_length; each_char++) {
      start_bit = index + (each_char * X_CORE_BITARRAY_BITS_IN_CHAR);
      c = x_core_bitarray_get_unsigned_char(bitarray, start_bit);
      *(string + each_char) = c;
    }
    *(string + string_length) = '\0';
  } else {
    x_core_trace("malloc");
  }

  return string;
}

unsigned char x_core_bitarray_get_unsigned_char(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_unsigned_char_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR);
}

unsigned char x_core_bitarray_get_unsigned_char_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits)
{
  return get_unsigned_long(bitarray, index, bits,
      UNSIGNED_CHAR_MAX_PLACE_VALUE);
}

unsigned long x_core_bitarray_get_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_unsigned_long_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR);
}

unsigned long x_core_bitarray_get_unsigned_long_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits)
{
  return get_unsigned_long(bitarray, index, bits,
      UNSIGNED_LONG_MAX_PLACE_VALUE);
}

unsigned long x_core_bitarray_get_unsigned_long_from_bits
(x_core_bitarray_t *bitarray, unsigned long start_index,
    unsigned long end_index)
{
  assert(bitarray);
  assert(start_index >= 0);
  assert(end_index < x_core_bitarray_get_size(bitarray));
  assert(start_index <= end_index);

  unsigned long value;
  unsigned long place_value;
  unsigned long place;
  unsigned long overplace;
  x_core_bit_t bit;

  value = 0;
  place_value = 1;
  overplace = start_index + (end_index - start_index);

  for (place = start_index; place <= overplace; place++) {
    bit = x_core_bitarray_get_bit(bitarray, place);
    value += (bit * place_value);
    place_value *= 2;
  }

  return value;
}

unsigned short x_core_bitarray_get_unsigned_short(x_core_bitarray_t *bitarray,
    unsigned long index)
{
  return x_core_bitarray_get_unsigned_short_bits(bitarray, index,
      X_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT);
}

unsigned short x_core_bitarray_get_unsigned_short_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits)
{
  return get_unsigned_long(bitarray, index, bits,
      UNSIGNED_SHORT_MAX_PLACE_VALUE);
}

x_core_bool_t x_core_bitarray_increment(x_core_bitarray_t *bitarray)
{
  return increment(bitarray, 0);
}

void x_core_bitarray_print(x_core_bitarray_t *bitarray)
{
  x_core_print(bitarray, x_core_bitarray_as_binary_string);
}

void x_core_bitarray_randomize(x_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long index;
  x_core_bit_t value;
  unsigned long array_size;

  array_size = x_core_bitarray_get_size(bitarray);

  for (index = 0; index < array_size; index++) {
    value = random() % 2;
    x_core_bitarray_set_bit(bitarray, index, value);
  }
}

void x_core_bitarray_flip_bit(x_core_bitarray_t *bitarray, unsigned long index)
{
  assert(bitarray);
  assert(index >= 0);
  assert(index < x_core_bitarray_get_size(bitarray));

  if (x_core_bitarray_get_bit(bitarray, index)) {
    x_core_bitarray_set_bit(bitarray, index, 0);
  } else {
    x_core_bitarray_set_bit(bitarray, index, 1);
  }
}

void x_core_bitarray_set_bit(x_core_bitarray_t *bitarray,
    unsigned long virtual_index, x_core_bit_t value)
{
  assert(bitarray);
  unsigned long index;

  index = x_core_wrap_index(virtual_index, bitarray->array_size);
  x_core_bitarray_set_bit_actual(bitarray, index, value);
}

void x_core_bitarray_set_bits_from_bitarray(x_core_bitarray_t *destination,
    unsigned long destination_index, x_core_bitarray_t *source,
    unsigned long source_index, unsigned long length)
{
  assert(destination);
  assert(source);
  assert(length > 0);
  assert((destination_index + length) <= destination->array_size);
  assert((source_index + length) <= source->array_size);
  unsigned long l;
  x_core_bit_t bit;

  for (l = 0; l < length; l++) {
    bit = x_core_bitarray_get_bit(source, source_index + l);
    x_core_bitarray_set_bit(destination, destination_index + l, bit);
  }
}

/*
  TODO: reimplement to match the pair-of-unsigned-longs way of doing this
*/
void x_core_bitarray_set_double(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, double value)
{
  assert(bitarray);
  assert(start_index < end_index);
  assert(end_index < x_core_bitarray_get_size(bitarray));
  assert(value <= 1.0);
  assert(value >= 0.0);
  unsigned long base = 0;
  unsigned long integer = 0;

  base = pow(2, (end_index - start_index + 1)) - 1;
  integer = (unsigned long) round(value * base);

  return x_core_bitarray_set_unsigned_long
    (bitarray, start_index, end_index, integer);
}

void x_core_bitarray_set_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, unsigned long value)
{
  assert(bitarray);
  assert(start_index <= end_index);
  assert(end_index < x_core_bitarray_get_size(bitarray));
  assert(value < pow(2, ((end_index - start_index) + 1)));
  unsigned long i;
  unsigned long working_value;

  working_value = value;
  for (i = start_index; i <= end_index; i++) {

    if (working_value % 2) {
      x_core_bitarray_set_bit(bitarray, i, 1);
    } else {
      x_core_bitarray_set_bit(bitarray, i, 0);
    }

    working_value /= 2;
  }
}

x_core_bool_t increment(x_core_bitarray_t *bitarray, unsigned long position)
{
  assert(bitarray);
  x_core_bool_t success;
  long each_bit;
  x_core_bit_t bit;
  unsigned long array_size;

  array_size = x_core_bitarray_get_size(bitarray);

  if (position >= array_size) {
    success = x_core_bool_false;
  } else {
    bit = x_core_bitarray_get_bit(bitarray, position);
    if (0 == bit) {
      success = x_core_bool_true;
      x_core_bitarray_set_bit(bitarray, position, 1);
      if (position > 0) {
        for (each_bit = (position - 1); each_bit >= 0; each_bit--) {
          x_core_bitarray_set_bit(bitarray, each_bit, 0);
        }
      }
    } else {
      success = increment(bitarray, position + 1);
    }
  }

  return success;
}
