#ifndef x_xcache_system_h
#define x_xcache_system_h

#include "x/core/object.h"
#include "x/core/uuid.h"

struct x_xcache_system_t;
typedef struct x_xcache_system_t x_xcache_system_t;

x_core_bool_t x_xcache_system_add(x_xcache_system_t *system,
    x_core_uuid_t *object_uuid, void *object,
    x_core_copy_f copy, x_core_destroy_f destroy,
    unsigned long lifetime_seconds);

void x_xcache_system_clear(x_xcache_system_t *system);

x_xcache_system_t *x_xcache_system_create();

void x_xcache_system_destroy(x_xcache_system_t *system);

void *x_xcache_system_find_wait_copy_expire(x_xcache_system_t *system,
    x_core_uuid_t *object_uuid, unsigned long max_wait_seconds);

unsigned long x_xcache_system_get_size(x_xcache_system_t *system);

void x_xcache_system_remove_objects(x_xcache_system_t *system);

#endif
