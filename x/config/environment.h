#ifndef cf_x_config_environment_h
#define cf_x_config_environment_h

#include "cf/x/case/list.h"

struct cf_x_config_environment_t;
typedef struct cf_x_config_environment_t cf_x_config_environment_t;

cf_x_config_environment_t *cf_x_config_environment_create();

void cf_x_config_environment_destroy(cf_x_config_environment_t *environment);

cf_x_core_bool_t cf_x_config_environment_find(cf_x_config_environment_t *environment,
    char *name);

cf_x_core_bool_t cf_x_config_environment_find_as_double
(cf_x_config_environment_t *environment, char *name, double *value,
    double default_value);

cf_x_core_bool_t cf_x_config_environment_find_as_string
(cf_x_config_environment_t *environment, char *name, char **value,
    char *default_value);

cf_x_core_bool_t cf_x_config_environment_find_as_unsigned_long
(cf_x_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value);

cf_x_core_bool_t cf_x_config_environment_find_as_unsigned_short
(cf_x_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value);

cf_x_core_bool_t cf_x_config_environment_find_list_as_strings
(cf_x_config_environment_t *environment, char *name, cf_x_case_list_t **list);

#endif
