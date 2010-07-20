#ifndef x_core_bitarray_h
#define x_core_bitarray_h

#include "x/core/bit.h"
#include "x/core/bool.h"
#include "x/core/message.h"
#include "x/core/object.h"
#include "x/core/string.h"

#define X_CORE_BITARRAY_BITS_IN_CHAR 8
#define X_CORE_BITARRAY_BITS_IN_LONG 32
#define X_CORE_BITARRAY_BITS_IN_SHORT 16
#define X_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR 8
#define X_CORE_BITARRAY_BITS_IN_UNSIGNED_LONG 32
#define X_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT 16

struct x_core_bitarray_t;
typedef struct x_core_bitarray_t x_core_bitarray_t;

x_core_bool_t x_core_bitarray_add_to_message(void *bitarray_object,
    x_core_message_t *message);

char *x_core_bitarray_as_binary_string(void *bitarray_object);

int x_core_bitarray_compare(void *bitarray_object_a, void *bitarray_object_b);

void *x_core_bitarray_copy(void *bitarray_object);

x_core_bitarray_t *x_core_bitarray_create(unsigned long size);

x_core_bitarray_t *x_core_bitarray_create_from_char(char value);

x_core_bitarray_t *x_core_bitarray_create_from_char_bits(char value,
    unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_double(double value);

x_core_bitarray_t *x_core_bitarray_create_from_double_bits(double value,
    unsigned short integral_bits, unsigned short fractional_bits);

x_core_bitarray_t *x_core_bitarray_create_from_long(long value);

x_core_bitarray_t *x_core_bitarray_create_from_long_bits(long value,
    unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_message
(x_core_message_t *message);

x_core_bitarray_t *x_core_bitarray_create_from_short(short value);

x_core_bitarray_t *x_core_bitarray_create_from_short_bits(short value,
    unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_string(char *string,
    unsigned long string_length);

x_core_bitarray_t *x_core_bitarray_create_from_string_bits(char *string,
    unsigned long string_length, unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_char
(unsigned char value);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_char_bits
(unsigned char value, unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_long
(unsigned long value);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_long_bits
(unsigned long value, unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_short
(unsigned short value);

x_core_bitarray_t *x_core_bitarray_create_from_unsigned_short_bits
(unsigned short value, unsigned short bits);

x_core_bitarray_t *x_core_bitarray_create_random(unsigned long size);

void x_core_bitarray_destroy(void *bitarray_object);

char *x_core_bitarray_get_as_string(void *bitarray_object);

unsigned long x_core_bitarray_get_actual_index(x_core_bitarray_t *bitarray,
    unsigned long virtual_index);

x_core_bit_t x_core_bitarray_get_bit(x_core_bitarray_t *bitarray,
    unsigned long virtual_index);

x_core_bool_t x_core_bitarray_get_bool(x_core_bitarray_t *bitarray,
    unsigned long index);

char x_core_bitarray_get_char(x_core_bitarray_t *bitarray,
    unsigned long index);

char x_core_bitarray_get_char_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

double x_core_bitarray_get_double(x_core_bitarray_t *bitarray,
    unsigned long index);

double x_core_bitarray_get_double_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short integral_bits,
    unsigned short fractional_bits);

/*
  TODO: deprecate in favor of x_core_bitarray_get_double()
*/
double x_core_bitarray_get_double_from_bits(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index);

long x_core_bitarray_get_long(x_core_bitarray_t *bitarray,
    unsigned long index);

long x_core_bitarray_get_long_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long x_core_bitarray_get_memory_size_bytes
(x_core_bitarray_t *bitarray);

short x_core_bitarray_get_short(x_core_bitarray_t *bitarray,
    unsigned long index);

short x_core_bitarray_get_short_bits(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long x_core_bitarray_get_size(x_core_bitarray_t *bitarray);

char *x_core_bitarray_get_string(x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned char x_core_bitarray_get_unsigned_char(x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned char x_core_bitarray_get_unsigned_char_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

unsigned long x_core_bitarray_get_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned long x_core_bitarray_get_unsigned_long_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

/*
  TODO: deprecate in favor of x_core_bitarray_get_unsigned_long()
*/
unsigned long x_core_bitarray_get_unsigned_long_from_bits
(x_core_bitarray_t *bitarray, unsigned long start_index,
    unsigned long end_index);

unsigned short x_core_bitarray_get_unsigned_short(x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned short x_core_bitarray_get_unsigned_short_bits
(x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

x_core_bool_t x_core_bitarray_increment(x_core_bitarray_t *bitarray);

void x_core_bitarray_print(x_core_bitarray_t *bitarray);

void x_core_bitarray_randomize(x_core_bitarray_t *bitarray);

void x_core_bitarray_set_all(x_core_bitarray_t *bitarray);

void x_core_bitarray_set_bit(x_core_bitarray_t *bitarray, unsigned long index,
    x_core_bit_t value);

void x_core_bitarray_set_bits_from_bitarray(x_core_bitarray_t *destination,
    unsigned long destination_index, x_core_bitarray_t *source,
    unsigned long source_index, unsigned long length);

void x_core_bitarray_flip_bit(x_core_bitarray_t *bitarray,
    unsigned long index);

void x_core_bitarray_set_bit(x_core_bitarray_t *bitarray,
    unsigned long virtual_index, x_core_bit_t value);

void x_core_bitarray_set_double(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, double value);

void x_core_bitarray_set_unsigned_long(x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, unsigned long value);

void x_core_bitarray_unset_all(x_core_bitarray_t *bitarray);

#endif
