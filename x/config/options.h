#ifndef cf_x_config_options_h
#define cf_x_config_options_h

#include "cf/x/case/list.h"
#include "cf/x/core/objects.h"

struct cf_x_config_options_t;
typedef struct cf_x_config_options_t cf_x_config_options_t;

cf_x_config_options_t *cf_x_config_options_create(int argc, char *argv[],
    cf_x_core_objects_t *objects);

void cf_x_config_options_destroy(cf_x_config_options_t *options);

cf_x_core_bool_t cf_x_config_options_find(cf_x_config_options_t *options, char *name);

cf_x_core_bool_t cf_x_config_options_find_as_double(cf_x_config_options_t *options,
    char *name, double *value, double default_value);

cf_x_core_bool_t cf_x_config_options_find_as_string(cf_x_config_options_t *options,
    char *name, char **value, char *default_value);

cf_x_core_bool_t cf_x_config_options_find_as_unsigned_short
(cf_x_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value);

cf_x_core_bool_t cf_x_config_options_find_as_unsigned_long
(cf_x_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value);

cf_x_core_bool_t cf_x_config_options_find_list_as_strings
(cf_x_config_options_t *options, char *name, cf_x_case_list_t **list);

#endif
