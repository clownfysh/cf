#ifndef cf_x_file_basic_h
#define cf_x_file_basic_h

#include "cf/x/case/list.h"
#include "cf/x/file/mode.h"

struct cf_x_file_basic_t;
typedef struct cf_x_file_basic_t cf_x_file_basic_t;

cf_x_file_basic_t *cf_x_file_basic_create(const char *filename,
    cf_x_file_mode_t mode);

void cf_x_file_basic_destroy(cf_x_file_basic_t *file);

cf_x_core_bool_t cf_x_file_basic_exists(const char *filename);

cf_x_core_bool_t cf_x_file_basic_get_as_blob(cf_x_file_basic_t *file,
    char **blob, unsigned long *blob_size);

cf_x_case_list_t *cf_x_file_basic_get_as_line_list(cf_x_file_basic_t *file);

char *cf_x_file_basic_get_as_string(cf_x_file_basic_t *file);

cf_x_case_list_t *cf_x_file_basic_get_as_token_list(cf_x_file_basic_t *file,
    char *delimiters);

FILE *cf_x_file_basic_get_file(cf_x_file_basic_t *file);

cf_x_core_bool_t cf_x_file_basic_get_line_count(cf_x_file_basic_t *file,
    unsigned long *line_count);

cf_x_core_bool_t cf_x_file_basic_write_char(cf_x_file_basic_t *file, int c);

cf_x_core_bool_t cf_x_file_basic_write_string(cf_x_file_basic_t *file,
    char *string);

#endif
