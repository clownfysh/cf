#ifndef cf_x_case_xcache_system_h
#define cf_x_case_xcache_system_h

#include "cf/x/core/object.h"
#include "cf/x/core/uuid.h"

struct cf_x_case_xcache_system_t;
typedef struct cf_x_case_xcache_system_t cf_x_case_xcache_system_t;

cf_x_core_bool_t cf_x_case_xcache_system_add(cf_x_case_xcache_system_t *system,
    cf_x_core_uuid_t *object_uuid, void *object,
    cf_x_core_object_copy_f copy, cf_x_core_object_destroy_f destroy,
    unsigned long lifetime_seconds);

void cf_x_case_xcache_system_clear(cf_x_case_xcache_system_t *system);

cf_x_case_xcache_system_t *cf_x_case_xcache_system_create();

void cf_x_case_xcache_system_destroy(cf_x_case_xcache_system_t *system);

void *cf_x_case_xcache_system_find_wait_copy_expire(cf_x_case_xcache_system_t *system,
    cf_x_core_uuid_t *object_uuid, unsigned long max_wait_seconds);

unsigned long cf_x_case_xcache_system_get_size(cf_x_case_xcache_system_t *system);

void cf_x_case_xcache_system_remove_objects(cf_x_case_xcache_system_t *system);

#endif
