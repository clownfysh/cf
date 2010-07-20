#ifndef x_xcache_item_h
#define x_xcache_item_h

#include "x/core/object.h"
#include "x/core/uuid.h"

struct x_xcache_item_t;
typedef struct x_xcache_item_t x_xcache_item_t;

int x_xcache_item_compare(void *item_object_a, void *item_object_b);

void *x_xcache_item_copy(void *item_object);

x_xcache_item_t *x_xcache_item_create(x_core_uuid_t *object_uuid,
    void *object, x_core_copy_f copy,
    x_core_destroy_f destroy, unsigned long lifespan_seconds);

x_xcache_item_t *x_xcache_item_create_decoy(x_core_uuid_t *object_uuid);

void x_xcache_item_destroy(void *item_object);

void x_xcache_item_destroy_decoy(void *item_object);

void x_xcache_item_expire(x_xcache_item_t *item);

void *x_xcache_item_get_object(x_xcache_item_t *item);

void *x_xcache_item_get_object_copy(x_xcache_item_t *item);

x_core_uuid_t *x_xcache_item_get_object_uuid(x_xcache_item_t *item);

x_core_bool_t x_xcache_item_is_expired(void *item_object);

#endif
