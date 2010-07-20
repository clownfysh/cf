#ifndef x_config_options_h
#define x_config_options_h

#include "x/container/list.h"
#include "x/core/objects.h"

struct x_config_options_t;
typedef struct x_config_options_t x_config_options_t;

x_config_options_t *x_config_options_create(int argc, char *argv[],
    x_core_objects_t *objects);

void x_config_options_destroy(x_config_options_t *options);

x_core_bool_t x_config_options_find(x_config_options_t *options, char *name);

x_core_bool_t x_config_options_find_as_double(x_config_options_t *options,
    char *name, double *value, double default_value);

x_core_bool_t x_config_options_find_as_string(x_config_options_t *options,
    char *name, char **value, char *default_value);

x_core_bool_t x_config_options_find_as_unsigned_short
(x_config_options_t *options, char *name, unsigned short *value,
    unsigned short default_value);

x_core_bool_t x_config_options_find_as_unsigned_long
(x_config_options_t *options, char *name, unsigned long *value,
    unsigned long default_value);

x_core_bool_t x_config_options_find_list_as_strings
(x_config_options_t *options, char *name, x_container_list_t **list);

#endif
