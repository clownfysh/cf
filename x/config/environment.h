#ifndef x_config_environment_h
#define x_config_environment_h

#include "x/case/list.h"

struct x_config_environment_t;
typedef struct x_config_environment_t x_config_environment_t;

x_config_environment_t *x_config_environment_create();

void x_config_environment_destroy(x_config_environment_t *environment);

x_core_bool_t x_config_environment_find(x_config_environment_t *environment,
    char *name);

x_core_bool_t x_config_environment_find_as_double
(x_config_environment_t *environment, char *name, double *value,
    double default_value);

x_core_bool_t x_config_environment_find_as_string
(x_config_environment_t *environment, char *name, char **value,
    char *default_value);

x_core_bool_t x_config_environment_find_as_unsigned_long
(x_config_environment_t *environment, char *name, unsigned long *value,
    unsigned long default_value);

x_core_bool_t x_config_environment_find_as_unsigned_short
(x_config_environment_t *environment, char *name, unsigned short *value,
    unsigned short default_value);

x_core_bool_t x_config_environment_find_list_as_strings
(x_config_environment_t *environment, char *name, x_case_list_t **list);

#endif
