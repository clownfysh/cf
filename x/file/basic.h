#ifndef x_file_basic_h
#define x_file_basic_h

#include "x/case/list.h"
#include "x/core/types.h"
#include "x/file/mode.h"

struct x_file_basic_t;
typedef struct x_file_basic_t x_file_basic_t;

x_file_basic_t *x_file_basic_create(const char *filename, x_file_mode_t mode);

void x_file_basic_destroy(x_file_basic_t *file);

x_core_bool_t x_file_basic_exists(const char *filename);

x_core_bool_t x_file_basic_get_as_blob(x_file_basic_t *file, char **blob,
    unsigned long *blob_size);

x_case_list_t *x_file_basic_get_as_line_list(x_file_basic_t *file);

char *x_file_basic_get_as_string(x_file_basic_t *file);

x_case_list_t *x_file_basic_get_as_token_list(x_file_basic_t *file,
    char *delimiters);

FILE *x_file_basic_get_file(x_file_basic_t *file);

x_core_bool_t x_file_basic_get_line_count(x_file_basic_t *file,
    unsigned long *line_count);

x_core_bool_t x_file_basic_write_char(x_file_basic_t *file, int c);

x_core_bool_t x_file_basic_write_string(x_file_basic_t *file, char *string);

#endif
