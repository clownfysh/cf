#ifndef x_file_csv_h
#define x_file_csv_h

#include "x/container/array.h"
#include "x/core/bit.h"
#include "x/core/object.h"

struct x_file_csv_t;
typedef struct x_file_csv_t x_file_csv_t;

x_file_csv_t *x_file_csv_create(char *filename);

x_file_csv_t *x_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object);

void x_file_csv_destroy(void *csv_object);

x_container_array_t *x_file_csv_get_field_by_name_as_array(x_file_csv_t *csv,
    char *field_name);

x_container_array_t *x_file_csv_get_field_by_index_as_array(x_file_csv_t *csv,
    unsigned long field_index);

unsigned long x_file_csv_get_field_count(x_file_csv_t *csv);

x_core_bool_t x_file_csv_get_field_index(x_file_csv_t *csv, char *field_name,
    unsigned long *field_index);

char *x_file_csv_get_field_name(x_file_csv_t *csv, unsigned long field_index);

unsigned long x_file_csv_get_object_count(x_file_csv_t *csv);

x_core_bit_t x_file_csv_get_value_by_index_as_bit(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

x_core_bool_t x_file_csv_get_value_by_index_as_bool(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char x_file_csv_get_value_by_index_as_char(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

double x_file_csv_get_value_by_index_as_double(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

long x_file_csv_get_value_by_index_as_long(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

short x_file_csv_get_value_by_index_as_short(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char *x_file_csv_get_value_by_index_as_string(x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

unsigned char x_file_csv_get_value_by_index_as_unsigned_char
(x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned long x_file_csv_get_value_by_index_as_unsigned_long
(x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned short x_file_csv_get_value_by_index_as_unsigned_short
(x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

char x_file_csv_get_value_by_name_as_char(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

double x_file_csv_get_value_by_name_as_double(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

long x_file_csv_get_value_by_name_as_long(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

short x_file_csv_get_value_by_name_as_short(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

char *x_file_csv_get_value_by_name_as_string(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned char x_file_csv_get_value_by_name_as_unsigned_char(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned long x_file_csv_get_value_by_name_as_unsigned_long(x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned short x_file_csv_get_value_by_name_as_unsigned_short
(x_file_csv_t *csv, unsigned long object_index, char *field_name);

#endif
