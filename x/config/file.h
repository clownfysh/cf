#ifndef x_config_file_h
#define x_config_file_h

#include "x/case/list.h"

struct x_config_file_t;
typedef struct x_config_file_t x_config_file_t;

x_config_file_t *x_config_file_create(char *filename);

void x_config_file_destroy(x_config_file_t *conf);

x_core_bool_t x_config_file_find(x_config_file_t *file, char *name);

x_core_bool_t x_config_file_find_as_double(x_config_file_t *file,
    char *name, double *value, double default_value);

x_core_bool_t x_config_file_find_as_string(x_config_file_t *file, char *name,
    char **value, char *default_value);

x_core_bool_t x_config_file_find_as_unsigned_long(x_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value);

x_core_bool_t x_config_file_find_as_unsigned_short(x_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value);

x_core_bool_t x_config_file_find_list_as_strings(x_config_file_t *file,
    char *name, x_case_list_t **list);

#endif
