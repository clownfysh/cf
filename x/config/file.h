#ifndef cf_x_config_file_h
#define cf_x_config_file_h

#include "cf/x/case/list.h"

struct cf_x_config_file_t;
typedef struct cf_x_config_file_t cf_x_config_file_t;

cf_x_config_file_t *cf_x_config_file_create(char *filename);

void cf_x_config_file_destroy(cf_x_config_file_t *conf);

cf_x_core_bool_t cf_x_config_file_find(cf_x_config_file_t *file, char *name);

cf_x_core_bool_t cf_x_config_file_find_as_double(cf_x_config_file_t *file,
    char *name, double *value, double default_value);

cf_x_core_bool_t cf_x_config_file_find_as_string(cf_x_config_file_t *file, char *name,
    char **value, char *default_value);

cf_x_core_bool_t cf_x_config_file_find_as_unsigned_long(cf_x_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value);

cf_x_core_bool_t cf_x_config_file_find_as_unsigned_short(cf_x_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value);

cf_x_core_bool_t cf_x_config_file_find_list_as_strings(cf_x_config_file_t *file,
    char *name, cf_x_case_list_t **list);

#endif
