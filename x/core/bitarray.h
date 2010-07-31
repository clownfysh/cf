#ifndef cf_x_core_bitarray_h
#define cf_x_core_bitarray_h

#include "cf/x/core/bit.h"
#include "cf/x/core/bool.h"
#include "cf/x/core/message.h"
#include "cf/x/core/object.h"
#include "cf/x/core/string.h"

#define CF_X_CORE_BITARRAY_BITS_IN_CHAR 8
#define CF_X_CORE_BITARRAY_BITS_IN_LONG 32
#define CF_X_CORE_BITARRAY_BITS_IN_SHORT 16
#define CF_X_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR 8
#define CF_X_CORE_BITARRAY_BITS_IN_UNSIGNED_LONG 32
#define CF_X_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT 16

struct cf_x_core_bitarray_t;
typedef struct cf_x_core_bitarray_t cf_x_core_bitarray_t;

cf_x_core_bool_t cf_x_core_bitarray_add_to_message(void *bitarray_object,
    cf_x_core_message_t *message);

char *cf_x_core_bitarray_as_binary_string(void *bitarray_object);

int cf_x_core_bitarray_compare(void *bitarray_object_a, void *bitarray_object_b);

void *cf_x_core_bitarray_copy(void *bitarray_object);

cf_x_core_bitarray_t *cf_x_core_bitarray_create(unsigned long size);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_char(char value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_char_bits(char value,
    unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_double(double value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_double_bits(double value,
    unsigned short integral_bits, unsigned short fractional_bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_long(long value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_long_bits(long value,
    unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_message
(cf_x_core_message_t *message);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_short(short value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_short_bits(short value,
    unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_string(char *string,
    unsigned long string_length);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_string_bits(char *string,
    unsigned long string_length, unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_char
(unsigned char value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_char_bits
(unsigned char value, unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_long
(unsigned long value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_long_bits
(unsigned long value, unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_short
(unsigned short value);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_from_unsigned_short_bits
(unsigned short value, unsigned short bits);

cf_x_core_bitarray_t *cf_x_core_bitarray_create_random(unsigned long size);

void cf_x_core_bitarray_destroy(void *bitarray_object);

char *cf_x_core_bitarray_get_as_string(void *bitarray_object);

unsigned long cf_x_core_bitarray_get_actual_index(cf_x_core_bitarray_t *bitarray,
    unsigned long virtual_index);

cf_x_core_bit_t cf_x_core_bitarray_get_bit(cf_x_core_bitarray_t *bitarray,
    unsigned long virtual_index);

cf_x_core_bool_t cf_x_core_bitarray_get_bool(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

char cf_x_core_bitarray_get_char(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

char cf_x_core_bitarray_get_char_bits(cf_x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

double cf_x_core_bitarray_get_double(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

double cf_x_core_bitarray_get_double_bits(cf_x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short integral_bits,
    unsigned short fractional_bits);

/*
  TODO: deprecate in favor of cf_x_core_bitarray_get_double()
*/
double cf_x_core_bitarray_get_double_from_bits(cf_x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index);

long cf_x_core_bitarray_get_long(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

long cf_x_core_bitarray_get_long_bits(cf_x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long cf_x_core_bitarray_get_memory_size_bytes
(cf_x_core_bitarray_t *bitarray);

short cf_x_core_bitarray_get_short(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

short cf_x_core_bitarray_get_short_bits(cf_x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long cf_x_core_bitarray_get_size(cf_x_core_bitarray_t *bitarray);

char *cf_x_core_bitarray_get_string(cf_x_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned char cf_x_core_bitarray_get_unsigned_char(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned char cf_x_core_bitarray_get_unsigned_char_bits
(cf_x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

unsigned long cf_x_core_bitarray_get_unsigned_long(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned long cf_x_core_bitarray_get_unsigned_long_bits
(cf_x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

/*
  TODO: deprecate in favor of cf_x_core_bitarray_get_unsigned_long()
*/
unsigned long cf_x_core_bitarray_get_unsigned_long_from_bits
(cf_x_core_bitarray_t *bitarray, unsigned long start_index,
    unsigned long end_index);

unsigned short cf_x_core_bitarray_get_unsigned_short(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

unsigned short cf_x_core_bitarray_get_unsigned_short_bits
(cf_x_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

cf_x_core_bool_t cf_x_core_bitarray_increment(cf_x_core_bitarray_t *bitarray);

void cf_x_core_bitarray_print(cf_x_core_bitarray_t *bitarray);

void cf_x_core_bitarray_randomize(cf_x_core_bitarray_t *bitarray);

void cf_x_core_bitarray_set_all(cf_x_core_bitarray_t *bitarray);

void cf_x_core_bitarray_set_bit(cf_x_core_bitarray_t *bitarray, unsigned long index,
    cf_x_core_bit_t value);

void cf_x_core_bitarray_set_bits_from_bitarray(cf_x_core_bitarray_t *destination,
    unsigned long destination_index, cf_x_core_bitarray_t *source,
    unsigned long source_index, unsigned long length);

void cf_x_core_bitarray_flip_bit(cf_x_core_bitarray_t *bitarray,
    unsigned long index);

void cf_x_core_bitarray_set_bit(cf_x_core_bitarray_t *bitarray,
    unsigned long virtual_index, cf_x_core_bit_t value);

void cf_x_core_bitarray_set_double(cf_x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, double value);

void cf_x_core_bitarray_set_unsigned_long(cf_x_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, unsigned long value);

void cf_x_core_bitarray_unset_all(cf_x_core_bitarray_t *bitarray);

#endif
