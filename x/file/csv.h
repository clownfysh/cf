#ifndef cf_x_file_csv_h
#define cf_x_file_csv_h

#include "cf/x/case/array.h"
#include "cf/x/core/bit.h"
#include "cf/x/core/object.h"

struct cf_x_file_csv_t;
typedef struct cf_x_file_csv_t cf_x_file_csv_t;

cf_x_file_csv_t *cf_x_file_csv_create(char *filename);

cf_x_file_csv_t *cf_x_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object);

void cf_x_file_csv_destroy(void *csv_object);

cf_x_case_array_t *cf_x_file_csv_get_field_by_name_as_array(cf_x_file_csv_t *csv,
    char *field_name);

cf_x_case_array_t *cf_x_file_csv_get_field_by_index_as_array(cf_x_file_csv_t *csv,
    unsigned long field_index);

unsigned long cf_x_file_csv_get_field_count(cf_x_file_csv_t *csv);

cf_x_core_bool_t cf_x_file_csv_get_field_index(cf_x_file_csv_t *csv, char *field_name,
    unsigned long *field_index);

char *cf_x_file_csv_get_field_name(cf_x_file_csv_t *csv, unsigned long field_index);

unsigned long cf_x_file_csv_get_object_count(cf_x_file_csv_t *csv);

cf_x_core_bit_t cf_x_file_csv_get_value_by_index_as_bit(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

cf_x_core_bool_t cf_x_file_csv_get_value_by_index_as_bool(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char cf_x_file_csv_get_value_by_index_as_char(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

double cf_x_file_csv_get_value_by_index_as_double(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

long cf_x_file_csv_get_value_by_index_as_long(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

short cf_x_file_csv_get_value_by_index_as_short(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char *cf_x_file_csv_get_value_by_index_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

unsigned char cf_x_file_csv_get_value_by_index_as_unsigned_char
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned long cf_x_file_csv_get_value_by_index_as_unsigned_long
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned short cf_x_file_csv_get_value_by_index_as_unsigned_short
(cf_x_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

char cf_x_file_csv_get_value_by_name_as_char(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

double cf_x_file_csv_get_value_by_name_as_double(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

long cf_x_file_csv_get_value_by_name_as_long(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

short cf_x_file_csv_get_value_by_name_as_short(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

char *cf_x_file_csv_get_value_by_name_as_string(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned char cf_x_file_csv_get_value_by_name_as_unsigned_char(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned long cf_x_file_csv_get_value_by_name_as_unsigned_long(cf_x_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned short cf_x_file_csv_get_value_by_name_as_unsigned_short
(cf_x_file_csv_t *csv, unsigned long object_index, char *field_name);

#endif
