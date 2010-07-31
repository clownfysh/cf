#ifndef cf_x_file_directory_h
#define cf_x_file_directory_h

#include "cf/x/core/standard.h"
#include "cf/x/core/bool.h"

struct cf_x_file_directory_t;
typedef struct cf_x_file_directory_t cf_x_file_directory_t;

cf_x_file_directory_t *cf_x_file_directory_create();

void cf_x_file_directory_destroy(cf_x_file_directory_t *cf_x_file_directory);

cf_x_core_bool_t cf_x_file_directory_exists(char *directory_name);

cf_x_core_bool_t cf_x_file_directory_make(char *directory_name, mode_t mode);

#endif
