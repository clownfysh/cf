#ifndef cf_x_config_system_h
#define cf_x_config_system_h

#include "cf/x/core/bool.h"
#include "cf/x/core/objects.h"
#include "cf/x/case/list.h"

struct cf_x_config_system_t;
typedef struct cf_x_config_system_t cf_x_config_system_t;

cf_x_config_system_t *cf_x_config_system_create(int argc, char *argv[],
    char *filename, cf_x_core_objects_t *objects);

void cf_x_config_system_destroy(cf_x_config_system_t *system);

cf_x_core_bool_t cf_x_config_system_find(cf_x_config_system_t *system, char *name);

cf_x_core_bool_t cf_x_config_system_find_as_double(cf_x_config_system_t *system,
    char *name, double *value, double default_value);

cf_x_core_bool_t cf_x_config_system_find_as_string(cf_x_config_system_t *system,
    char *name, char **value, char *default_value);

cf_x_core_bool_t cf_x_config_system_find_as_unsigned_long(cf_x_config_system_t *system,
    char *name, unsigned long *value, unsigned long default_value);

cf_x_core_bool_t cf_x_config_system_find_as_unsigned_short(cf_x_config_system_t *system,
    char *name, unsigned short *value, unsigned short default_value);

cf_x_core_bool_t cf_x_config_system_find_list_as_strings(cf_x_config_system_t *system,
    char *name, cf_x_case_list_t **list);

#endif
