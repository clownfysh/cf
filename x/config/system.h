#ifndef x_config_system_h
#define x_config_system_h

#include "x/core/bool.h"
#include "x/core/objects.h"
#include "x/case/list.h"

struct x_config_system_t;
typedef struct x_config_system_t x_config_system_t;

x_config_system_t *x_config_system_create(int argc, char *argv[],
    char *filename, x_core_objects_t *objects);

void x_config_system_destroy(x_config_system_t *system);

x_core_bool_t x_config_system_find(x_config_system_t *system, char *name);

x_core_bool_t x_config_system_find_as_double(x_config_system_t *system,
    char *name, double *value, double default_value);

x_core_bool_t x_config_system_find_as_string(x_config_system_t *system,
    char *name, char **value, char *default_value);

x_core_bool_t x_config_system_find_as_unsigned_long(x_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value);

x_core_bool_t x_config_system_find_as_unsigned_short(x_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value);

x_core_bool_t x_config_system_find_list_as_strings(x_config_system_t *system,
    char *name, x_case_list_t **list);

#endif
